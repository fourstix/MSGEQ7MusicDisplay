/*
 * Simulates a Volume Unit display using an MSGEQ7 chip and an 8x8 BiColor 
 * LED Matrix.
 * 
 * This version of the equalizer sketch uses off for the lowest range
 * giving us nine levels with seven channels. It also adds peak 
 * detection to show the maximum value in the eighth column.
 * 
 * The scale is displayed in decibels.
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

//Peak detection interval
#define INTERVAL 1000
//Thresholds for EQ graph, CEILING must be more than 7x FLOOR
#define FLOOR 60
#define CEILING 480
// off value for EQ scale
#define EQ_OFF -1

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

int analogPin = 0;    // read from multiplexer using analog input 0
int strobePin = 4;    // strobe is attached to digital pin 4
int resetPin = 5;     // reset is attached to digital pin 5
int spectrumValue[7]; // to hold a2d values
int maxPeak = 0;      // peak value for display

// lookup values for mapping each band
int bandLimits[8];

// with no input, the higher channels are slightly above the floor
// so we can subtract these values to obtain a flat reading.
//
// The quiesent points (q point) were found manually by running the 
// MSGEQ7_FindQ program and observing the average q point values in
// serial statements with no audio input.  
int qPoints[7] = {0, 0, 0, 5, 5, 10, 5};

// flag to print values as Serial messages
boolean debug = false;


void setup() {
  if (debug) {
    Serial.begin(9600);
  } // if debug
  
  matrix.begin(0x70); // pass in address
  
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);
 
  matrix.clear();
  //fill the lookup array
  fillMapLimits();
  
  if (debug) {
    Serial.println("MSGEQ7 VU Meter with Bicolor 8x8 Matrix");
    Serial.print("Band Limits: <");
    for(int i=0; i < 8; i++) {
      Serial.print(bandLimits[i]);
      if (i < 7) {
        Serial.print(", ");
      } else {
        Serial.println("+");
      } // if-else
    } // for    
  }  //if debug
} // setup

void loop() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  
  //read output of each channel and adjust
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin) - qPoints[i];

    if (spectrumValue[i] > maxPeak) {
      maxPeak = spectrumValue[i];
    } // if
    
    // print values for debugging
    if (debug) {
      if (spectrumValue[i] < 10) {
        Serial.print(" ");
        Serial.print(spectrumValue[i]);
      } else if (spectrumValue[i] < 100 ) {
        Serial.print(" ");
        Serial.print(spectrumValue[i]);
      } else {
        Serial.print(" ");
        Serial.print(spectrumValue[i]);
      } //if-else if-else
    } // if debug
  
    digitalWrite(strobePin, HIGH);
    } // for
    
    if (debug) {
      Serial.print(" Peak: ");
      Serial.println(maxPeak);
      Serial.println();
    }

  drawDisplay();
  // The Serial statements cause a little delay in sampling
  // which reduces jittering in the display.  When debug is 
  // disabled, the readings are a bit too fast.
  if (!debug) {
    delay(75);  // wait a little bit to reduce jitters
  } // if !debug
  
  // ever so often, reset the peak value to take new sample
  resample();
} // loop

// Draw EQualization bars in color for each band, plus peak value
void drawDisplay() {
  int height, x, y, color;
  
  matrix.clear();

  for (int i = 0; i < 7; i++) {
    //draw each line with proportional height
    y = calculateY(spectrumValue[i]);

    //EQ_OFF or -1 means no bar
    if (y > EQ_OFF) {
      // y value is from 0 to 7
      //draw lines with lowest value on left
      x = 7 - i;
      color = getColor(y);
      matrix.drawLine(x,0, x,y, color);    
    } // if height > 0
  } //for
  
  //set the peak value in last column
  y = calculateY(maxPeak);

  if (y > EQ_OFF) {
    color = getColor(y);
    matrix.drawPixel(0, y, color);
  } // if height > 0
  
  matrix.writeDisplay();  // write the changes we just made to the display  
} // drawDisplay

int getColor(int y) {
  int color = LED_GREEN;      
  //adjust color for high and mid range
  if (y > 5) {
    color = LED_RED;
  } else if (y > 2) {
    color = LED_YELLOW;
  } // if-else if  
  return color;
}

// Calculate height for EQ value in a band
// EQ_OFF if value under threshold, otherwise 0 to 7
int calculateY(int value) {
  int height = EQ_OFF;

  for(int i=0; i < 8; i++) {
    if(value < bandLimits[i]) {
      break;
    } // if < bandLimit
    height = i;
  } // for
  return height;
} // calculateU

// Every so often reset the peak for a new sample
void resample() {
  //a static variable is only initialized once, ever
  static unsigned long tick = millis();
  //get current time as unsigned long
  unsigned long tock = millis();

  if (tock - tick > INTERVAL) {
    tick = tock;
    maxPeak = 0;
  }  // if
}  // resample

// Calculates the nth value of a decibel mapping of the over-all
// gain into 7 bands.  The decibel equation is 20 Log10(V1/V0), 
// so the nth band would be 20Log10(CEILING/FLOOR)/7 *n), setting 
// this equal to 20Log10(X/FLOOR) and solving for x, yields
// x = 10^(n * Log10(CEILING/FLOOR) / 7) * FLOOR
//
// Note: using 10Log10(P1/P0) formula would yield the same result
// that's one of the nice things about decibels.  Also the ratio
// Ceiling/Floor must be greater than 7 to avoid the first band
// being less than the floor value.
int decibelMap(int n) {
  int result =EQ_OFF; //invalid value for debugging
  // 0 is FLOOR and 7 is CEILING, so don't calculate those values
  if (n < 1) {
    result = FLOOR;
  } else if (n >= 7) {
    result = CEILING;
  } else {
    //The real multipliers are to coerce double arithmetic from int
    double gainPerBand = (1.0 * CEILING) / (FLOOR * 7.0);
    double dbGain      = log10(gainPerBand * n);
    //Common Anti-Log is 10^x or pow(10, x)
    result = pow(10, dbGain) * (FLOOR * 1.0);      
  } // if-else if
  return result;
}

// fill the lookup array with the limits for each band
void fillMapLimits() {
  for(int i=0; i < 8; i++) {
    bandLimits[i] = decibelMap(i);
  } // for
} // fill MapLimits
