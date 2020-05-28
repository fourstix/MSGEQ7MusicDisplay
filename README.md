MSGEQ7MusicDisplay
==================

This repository contains a simple music VU display using an MSGEQ7 and an Adafruit 8x8 BiColor LED Matrix.


Introduction
------------


This code was based on the original MSGEQ7 tutorial by J. Skoba, [archived here.](http://http://nuewire.com/info-archive/msgeq7-by-j-skoba/)  

These programs use an 8x8 BiColor LED Matrix display from Adafruit to display frequency output from the MSGEQ7 Graphic Equalizer
Display filter.

The MSGEQ7 Graphic Equalizer Display filter is available from [Sparkfun.](https://www.sparkfun.com/products/10468)
 
The Adafruit 8x8 BiColor LED Matrix with I2C Backpack is available from [Adafruit.](https://www.adafruit.com/product/902)

Examples
--------
<table class="table table-hover table-striped table-bordered">
  <tr align="center">
   <td><img src="https://github.com/fourstix/MSGEQ7MusicDisplay/blob/master/pics/Schematic.jpg"></td>   
   <td><img src="https://github.com/fourstix/MSGEQ7MusicDisplay/blob/master/pics/MSGEQ7Breadboard.jpg"></td>    
   <td><img src="https://github.com/fourstix/MSGEQ7MusicDisplay/blob/master/pics/MSGEQ7Shield.jpg"></td> 
  </tr>
  <tr align="center">
  <td>Schematic diagram of the MSGEQ7 circuit.</td>
  <td>Breadboard with the MSGEQ7 circut connected to an Arduino with an 8x8 BiColor LED Matrix display.</td>
  <td>MSGEQ7 circuit implemented on an Arduino shield with an 8x8 BiColor LED Matrix display..</td>
  </tr>
 </table>
 
Repository Contents
-------------------
* **/src/MSGEQ7_MatrixBasic/**
  * MSGEQ7_MatrixBasic.ino -- Basic program showing a linear display of the MSGEQ7 Graphic Equalizer on the 8x8 BiColor LED Matrix.
* **/src/MSGEQ7_FindQ/** 
  * MSGEQ7_FindQ.ino -- Program to determine and print the quiescient points of the MSGEQ7 Graphic Equalizer's output.
* **/src/MSGEQ7_VU/**  
  * MSGEQ7_VU.ino -- Program showing a logarithmic or decibel display of the MSGEQ7 Graphic Equalizer on the 8x8 BiColor LED Matrix to
create a Volume Unit or VU display
* **/src/MSGEQ7_MatrixPeak/**  
  * MSGEQ7_MatrixPeak.ino -- Program showing a VU Peak display of the MSGEQ7 Graphic Equalizer on the 8x8 BiColor LED Matrix with 
Peak detection.
* **/pics/** -- Example pictures for README file
* **/docs/**
  * MSGEQ7.pdf -- MSI Seven Band Graphic Equalizer Data Sheet

License Information
-------------------

This code is public domain under the MIT License, but please buy me a beer
if you use this and we meet someday (Beerware).

References to any products, programs or services do not imply
that they will be available in all countries in which their respective owner operates.

Sparkfun, the Sparkfun logo, and other Sparkfun products and services are
trademarks of the Sparkfun Electronics Corporation, in the United States,
other countries or both. 

Adafruit, the Adafruit logo, and other Adafruit products and services are
trademarks of the Adafruit Industries, in the United States,other countries or both. 

Other company, product, or services names may be trademarks or services marks of others.

All libraries used in this code are copyright their respective authors.
 
The Adafruit LED Backpack Library
Copyright (c) 2012 Adafruit Industries
Written by Limor Fried/Ladyada for Adafruit Industries.  
 
Many thanks to the original authors for making their designs and code avaialble as open source.
  
This code, firmware, and software is released under the [MIT License](http://opensource.org/licenses/MIT).
 
The MIT License (MIT)
 
Copyright (c) 2020 by Gaston Williams
 
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
 
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
 
**THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.**
