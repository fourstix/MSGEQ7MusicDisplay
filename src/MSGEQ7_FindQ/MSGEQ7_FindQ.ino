/*
 * Find the quiescent points using an MSGEQ7 chip
 * 
 * This routine finds the no-input values for each of the 
 * MSGEQ7 chip's seven channels. It averages 100 readings  
 * and prints the result to Serial.
 * 
 * Run this program without any audio inputs to calculate
 * the channel offsets for the MSGEQ7.
 */

#include <Wire.h>

//Thresholds for EQ graph, CEILING must be more than 7x FLOOR
#define FLOOR 60
#define CEILING 480
// off value for EQ scale
#define EQ_OFF -1

int analogPin = 0;    // read from multiplexer using analog input 0
int strobePin = 4;    // strobe is attached to digital pin 4
int resetPin = 5;     // reset is attached to digital pin 5
int spectrumSum[7]; // to hold a2d values
int sampleSize = 10;


// with no input, the higher channels are slightly above the floor
// so we can subtract these values to obtain a flat reading
// The quiesent points (q point) were found manually by observing the
// channel values in debug statements with no audio input
int qPoints[7] = {0, 0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(9600);

  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
 
  Serial.println("MSGEQ7 Quiescent Points");
} // setup

void loop() {
  clearspectrumSums();
  //read and accumulate sample values for each channel
  for (int i = 0; i < sampleSize; i++) {
    readChannels();
    delay(50); 
  } // for
  
  showQPoints();
} // loop

// clear all the channel values for the next read
void clearspectrumSums() {
    for (int i = 0; i < 7; i++) {
      spectrumSum[i] = 0;
    } // for
} // clearspectrumSums

// read all seven channels and accumulate results
void readChannels() {
  int data = 0;
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  
  //read output of each channel and adjust
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    data = analogRead(analogPin);
    Serial.print(data);
    if (i < 6) {
      Serial.print(", ");
    } else {
      Serial.println();
    } // if-else 
    spectrumSum[i] = spectrumSum[i] + data;   

    digitalWrite(strobePin, HIGH);
    } // for
} // readChannels

// calculate and show q points for all channels
void showQPoints() {
  // calculate q points from spectrum totals, rounding up to nearest int
  for (int i = 0; i < 7; i++) {
    int data = (1.0 * spectrumSum[i]) / (sampleSize * 1.0) + 0.5;

    if (data > FLOOR) {
      qPoints[i] = data - FLOOR;
    } else {
      qPoints[i] = 0;
    } // if-else    
  } // for
  Serial.println("************************************************");
    // print q point values
  for (int j = 0; j < 7; j++) {
      if (qPoints[j] < 10) {
        Serial.print(" ");
        Serial.print(qPoints[j]);
      } else if (qPoints[j] < 100 ) {
        Serial.print(" ");
        Serial.print(qPoints[j]);
      } else {
        Serial.print(" ");
        Serial.print(qPoints[j]);
      } //if-else if-else
    } // for j
    Serial.println();
  Serial.println("************************************************");    
} //showQPoints

//write out accumulated values for all channels
void writespectrumSums() {
  // print values
  for (int i = 0; i < 7; i++) {
      if (spectrumSum[i] < 10) {
        Serial.print(" ");
        Serial.print(spectrumSum[i]);
      } else if (spectrumSum[i] < 100 ) {
        Serial.print(" ");
        Serial.print(spectrumSum[i]);
      } else {
        Serial.print(" ");
        Serial.print(spectrumSum[i]);
      } //if-else if-else
    } // for i
    Serial.println();
}
