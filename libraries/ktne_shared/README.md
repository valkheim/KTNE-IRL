# KTNE module shared library

This library implements all the features shared between the master and the slaves :
* use of the card 74HC595

## Core function(s)

* `void shiftOut(int myDataPin, int myClockPin, byte data)` :
Change the state of the card 74HC595 by communicate with myDataPin and myClockPin.
The card's output is set using the information contain in the bits of data.

## Installation

RTFM : [https://www.arduino.cc/en/Guide/Libraries](https://www.arduino.cc/en/Guide/Libraries)
