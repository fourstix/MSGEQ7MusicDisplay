/*
 * Simple EQ display using an MSGEQ7 chip and an 8x8 BiColor LED Matrix.
 * 
 * Copyright (c) 2019 by Gaston Williams
 * 
 * This project also uses an MSGEQ7 Graphic Equalizer Display filter
 * from Sparkfun available at https://www.sparkfun.com/products/10468
 * 
 * This project uses the Adafruit 8x8 BiColor LED Matrix with I2C Backpack
 * Available from Adafruit at https://www.adafruit.com/product/902
 *  
 * This program uses the Adafruit LED Backpack Library
 * written by Limor Fried/Ladyada for Adafruit Industries.  
 * Copyright (c) 2012 Adafruit Industries
 * 
 * All libraries and hardware designs are copyright their respective authors. 
 * 
 */
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();

int analogPin = 0; // read from multiplexer using analog input 0
int strobePin = 4; // strobe is attached to digital pin 4
int resetPin = 5; // reset is attached to digital pin 5
int spectrumValue[7]; // to hold a2d values


void setup() {
  Serial.begin(9600);
  matrix.begin(0x70); // pass in address
  
  pinMode(analogPin, INPUT);
  pinMode(strobePin, OUTPUT);
  pinMode(resetPin, OUTPUT);
  analogReference(DEFAULT);

  digitalWrite(resetPin, LOW);
  digitalWrite(strobePin, HIGH);

  Serial.println("MSGEQ7 Equalizer with Bicolor 8x8 Matrix");
} // setup

void loop() {
  digitalWrite(resetPin, HIGH);
  digitalWrite(resetPin, LOW);
  
  for (int i = 0; i < 7; i++) {
    digitalWrite(strobePin, LOW);
    delayMicroseconds(30); // to allow the output to settle
    spectrumValue[i] = analogRead(analogPin);
  
    // comment out/remove the serial stuff to go faster
    // - its just here for show
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
  
    digitalWrite(strobePin, HIGH);
    } // for
  Serial.println();
  matrix.clear();
  for (int i = 0; i < 7; i++) {
    //draw each line with proportional height
    int y = map(spectrumValue[i], 0, 1024, 0, 8);
    //draw lines with lowest value on left
    int x = 7 - i;
    int color = LED_GREEN;
    //adjust color for high and mid range
    if (y > 5) {
      color = LED_RED;
    } else  if (y > 2) {
      color = LED_YELLOW;
    }
    matrix.drawLine(x,0, x,y, color);    
  } //for
  
  matrix.writeDisplay();  // write the changes we just made to the display
  //delay(500);
} // loop
