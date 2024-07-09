# DotMatrix5x7
[![License: LGPL v3](https://img.shields.io/badge/License-LGPLv3-blue.svg)](https://www.gnu.org/licenses/lgpl-3.0)
[![Commits since latest](https://img.shields.io/github/commits-since/felias-fogg/DotMatrix5x7/latest?include_prereleases)](https://github.com/felias-fogg/DotMatrix5x7/commits/master)
![Hit Counter](https://visitor-badge.laobi.icu/badge?page_id=felias-fogg_DotMatrix5x7)
[![Build Status](https://github.com/felias-fogg/DotMatrix5x7/workflows/LibraryBuild/badge.svg)](https://github.com/felias-fogg/DotMatrix5x7/actions)

Drive a single 5x7 dot matrix display directly by Arduino pins (12 needed!)

Version: 1.5.2

Date:  31.05.2023

## Features

* Supports 5x7 dot matrix displays such as TA07-11SEKWA
* Assumes only Arduino pins, no external controller
* Pins and polarity are user definable
* Interrupt driven (using timer 0, optionally timer 1)
* Can scroll vertically and horizontally
* One can use regular strings and strings in flash memory
* It is possible to let characters blink
* You can provide your own delay function (in order to save power or to do something in parallel)

## Usage

This library can be used to control one 5x7 dot matrix display. 12 Arduino pins are necessary to control it (5 column pins, 7 row pins). The rows are multiplexed, which means one needs just 5 resistors for the columns. The display of characters is done in an interrupt driven way. This means you can do other things after you have requested to display a character.

Usually, TIMER0 (the timer for millis and delays) is used for generating the interrupts. Note that this does not interfere with `delay()` or `millis()` since we use a different interrupt! However, it could lead to an interrupt conflict if other parts of the program use the `TIMER0_COMPA` interrupt. For this reason, there exists the compile time option `USETIMER0`, which when defined to be 0 in DotMatrix5x7.h disables the TIMER0 interrupt.

Instead or additionally, the `TimerOne` library by Paul Stoffregen could be used, provided the MCU type is supported by this library.  If you want to use the TimerOne library, `USETIMER1` needs to be 1 (it is 0 by default). Note that TimerOne does not support all MCUs and will generate compilation errors if the MCU is not supported.

At least one of those timers must be usable for the library to function. If both timers are available, one can select which one to use when calling the `begin` method. 

When you want to display a single character `c`, call `Dot5x7.show(c)`. A string can be displayed, e.g., as follows: `Dot5x7.showString("Hello World!", 500, 100)`. This displays the string "Hello World!", where each character is displayed 500 msec and there is a pause of 100 msec between characters. One can also use the `F()` notation to pass a string that is stored in flash memory, i.e., `Dot5x7.showString(F("Hello World!"), 500, 100)`. In addition, one might pass a string stored in flash (which has been defined with PROGMEM) using the `_P`-variant of the method, i.e.: `Dot5x7.showString_P(str, 500, 100)`, provided `str` has been defined as `const char PROGMEM str[] = "Hello World!";`.

In addition, there are methods that scroll a string through the display. Scrolling can be done vertically up or down or horizontally left or right. For example, one can scroll the Hello World string by calling `Dot5x7.scrollLeftString("Hello World!", 300, 50, 1)`. This will scroll the string to the left, displaying each character for 300 msec in the middle of the display and using 50 msec for each shifting step. The final argument of 1 means that one blank column is used between two consecutive characters. There are also methods for scrolling right, up, or down. Further, one can, of course, use the `F()` notation and there are also the `_P` variants of the methods. 

## Configuration

The row and column pins can be specified when calling the `begin` method. In addition, you can specify the polarity of when the row and column pins are active, respectively. Default is that row pins are active LOW and column pins are active HIGH. This corresponds to driving a column anode display directly from the pins. Finally, as a last (default) argument, one can specify the timer (0 or 1). As mentioned, this is only significant if both timers are available for interrupts, i.e., both the compile time constants `USETIMER1` and `USETIMER0` have to be set to 1 in DotMatrix5x7.h.

In addition, there are a few methods that control the interface:

- `setUpsideDown(bool enable)` controls whether the characters are displayed upside-down.
- `setFont(const byte *f)` can be used to select an alternative font.
- `setFramesPerSecond(int fps)` is used to control how often a complete matrix is displayed. The default value is 50 and one should not select values below 42. This is only effective if Timer1 is used.
- `setBlinkFrames(int blinkon, int blinkoff)` controls blinking by specifying the number of frames where the character is displayed and the number of frames when the display is off. If either value is 0, no blinking will happening.
- `setDelayFunction(void (*f) (long unsigned int))` enables one to specify a custom delay function that is to be used when the `showString` and `scrollXXXString` methods are used. This may be used in order to minimize power consumption or to communicate with peripherals.
- `sleep()` stops timer and switches off all LEDs.
- `wakeup()` restarts timer.



## Adding new symbols

If you would like to create your own symbols, you can use the website http://dotmatrixtool.com/. Select the following parameters:

- Width: 6
- Height: 8
- Byte Order: Column Major
- Endian: Little Endian

Now you can can left click (to mark) and right click (to  unmark) and produce your symbol in the left five columns and the upper  seven rows. Press "Generate", and you get the code you have to fill into the font table (the first five bytes). 

Now, you can either use one of the entries in the standard font table in `dotfont5x7.h` or you can setup your own private font  table, which needs to be a PROGMEM char array similar to the `font5x7`  array, and then use the setFont method in order to switch between the  standard table and your new table. Let us assume, you have defined a Yen symbol, which leads to the five-byte code `0x29, 0x2a, 0x7c, 0x2a, 0x29`. Then you could, e.g., declare and use this symbol as follows:

```c++
const unsigned char  money[] PROGMEM = {0x29, 0x2a, 0x7c, 0x2a, 0x29};
...
Dot5x7.setFont(money);  // select custom font
Dot5x7.show(0);         // show the entry in custom font at location 0
delay(1000);
Dot5x7.setFont();       // switch back to system font
...
```



## Example

```c++
// Simple sketch for displaying a character and a string
#include <DotMatrix5x7.h>

void setup()
{
    Dot5x7.begin(0, 1, 2, 3, 4,         // column pins
	         5, 6, 7, 8, 9, 10, 11,     // row pins
	         LOW,                       // value when row pin is active (default value)
		 	 HIGH);                     // value when column pin is active (default value)
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
	                                      // and 0.1sec pause between two chars.
}
```

This software is published under the [LGPL](http://www.gnu.org/licenses/lgpl-3.0.html)
