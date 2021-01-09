// -*- c++ -*-
// Simple sketch to display a character and a string
#include <DotMatrix5x7.h>

void setup()
{
  Dot5x7.begin(2, 3, 4, 5, 6,         // column pins
	       7, 8, 9, 10, 11, 12, 13, // row pins
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
