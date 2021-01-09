# DotMatrix5x7
A 5x7 dot matrix display driven directly by Arduino pins (12 needed!)

Version: 1.2

Date:  09-01-2021

## Features

* Supports 5x7 dot matrix displays such as TA07-11SEKWA
* Assumes only Arduino pins, no external controller
* Pins and polarity are user definable
* Interrupt driven (using timer 1)
* Can scroll vertically and horizontally
* One can use regular strings and strings in flash memory
* It is possible to let characters blink
* You can provide your own delay function (in order to save power or to do something in parallel)

## Usage

This library has been written for 5x7 dot matrix displays. 12 Arduino pins are necessary to control it (5 column pins, 7 row pins). It uses the TimerOne library by Paul Stoffregen in order to display the text in an interrupt driven way. This means you can do other things after you have requested to display a character. 

## Example

	// Simple sketch to display a character and a string
	#include <DotMatrix5x7.h>

	void setup()
	{
		Dot5x7.begin(0, 1, 2, 3, 4,         // column pins
		             5, 6, 7, 8, 9, 10, 11, // row pins
		             LOW,                   // value when row pin is active (default value)
				     HIGH);                 // value when column pin is active (default value)
	    Dot5x7.setUpsideDown(true);         // display all chars upside down
	    Dot5x7.setFramesPerSecond(50);      // display 50 frames per second (default value)			 
	}	 

	void loop()
	{
		Dot5x7.show('X'); // display 'X'
		delay(1000);      // for 1 second
		Dot5x7.clear();   // clear display
		delay(1000);      // show empty display for 1 second
		Dot5x7.showString("Hello", 700, 100); // display string, with 0.7sec time for showing each char
		                                      // and 0.1sec pause between two chars
	}


This software is published under the [LGPL](http://www.gnu.org/licenses/lgpl-3.0.html)
