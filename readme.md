# DotMatrix5x7
Drive a single 5x7 dot matrix display directly by Arduino pins (12 needed!)

Version: 1.4.0

Date:  13-01-2021

## Features

* Supports 5x7 dot matrix displays such as TA07-11SEKWA
* Assumes only Arduino pins, no external controller
* Pins and polarity are user definable
* Interrupt driven (using timer 1, optionally timer 0)
* Can scroll vertically and horizontally
* One can use regular strings and strings in flash memory
* It is possible to let characters blink
* You can provide your own delay function (in order to save power or to do something in parallel)

## Usage

This library can be used to control one 5x7 dot matrix display. 12 Arduino pins are necessary to control it (5 column pins, 7 row pins). The rows are multiplexed, which means one needs just 5 resistors for the columns. The library makes use of the TimerOne library by Paul Stoffregen in order to display the text in an interrupt driven way. This means you can do other things after you have requested to display a character.

When you want to display a single character `c`, call `Dot5x7.show(c)`. A string can be displayed, e.g., as follows: `Dot5x7.showString("Hello World!", 500, 100)`. This displays the string "Hello World!", where each character is displayed 500 msec and there is a pause of 100 msec between characters. One can also use the `F()` notation to pass a string that is stored in flash memory, i.e., `Dot5x7.showString(F("Hello World!"), 500, 100)`. In addition, one might pass a string stored in flash (which has been defined with PROGMEM) using the `_P`-variant of the method, i.e.: `Dot5x7.showString_P(str, 500, 100)`, provided `str` has been defined as `const char PROGMEM str[] = "Hello World!";`.

In addition, there a methods that scroll a string through the display. Scrolling can be done vertically up or down or horizontally left or right. For example, one can scroll the Hello World string by calling `Dot5x7.scrollLeftString("Hello World!", 300, 50, 1)`. This will scroll the string to the left, displaying it for 300 msec in the middle of the display and using 50 msec for each shifting step. The final argumant of 1 means that one blank column is used between two consecutive characters. There are also methods for scrolling right, up, or down. Further, one can, of course, use the `F()` notation and ther also the `_P` variants of the methods. 

## Configuration

The row and column pins can be specified when calling the `begin` method. In addition, you can specify the polarity of when the row and column pins are active, respectively. Default is that row pins are active LOW and column pins are active HIGH. This corresponds to driving a column anode display directly from the pins.

In addition, there are a few methods that control the interface:
* `setUpsideDown(bool enable)` controls whether the characters are displayed upside-down.
* `setFont(const byte *f)` can be used to select an alternative font.
* `setFramesPerSecond(int fps)` is used to control how often a complete matrix is displayed. The default value is 50 and one should not select values below 42.
* `setBlinkFrames(int blinkon, int blinkoff)` controls blinking by specifying the number of frames where the character is displayed and the number of frames when the display is off. If either value is 0, no blinking will happening.
* `setDelayFunction(void (*f) (long unsigned int))` enables one to specify a custom delay function that is to be used when the `showString` and `scrollXXXString` methods are used. This may be used in order to minimize power consumption or to communicate with peripherals.

Finally, there exists the compile time option `USETIMER0`, which when defined in DotMatrix5x7.h leads to using TIMER0 (the timer for millis and delays) instead of TIMER1. Note that this does not interfere with delay or millis since we use a different interrupt! However, the method `setFramesPerSecond(int fps)` won't have any effect anymore. Moreover, using this option makes only sense when the MCU frequency is 8MHz or higher. With a system clock of 16 MHz, you get a frame rate of 142 fps and with 8 MHz 71 fps. With 4 MHz or less the frame rate is obviously too low.



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
