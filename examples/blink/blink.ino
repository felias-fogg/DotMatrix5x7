// -*- c++ -*-
// Simple sketch to display a character and a string with blinking
#include <DotMatrix5x7.h>

void setup()
{
  Dot5x7.begin(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
  Dot5x7.setFramesPerSecond(100);      // display 50 frames per second (default value)			 
}	 

void loop()
{
  Dot5x7.setBlinkFrames(40,40);
  Dot5x7.show('X'); // display 'X'
  delay(2000);      // for 1 second
  Dot5x7.clear();   // clear display
  delay(1000);      // show empty display for 1 second
  Dot5x7.setBlinkFrames(10,5);
  Dot5x7.showString("Hello", 700, 100); // display string, with 0.7sec time for showing each char
                                        // and 0.1sec pause between two chars
  Dot5x7.setBlinkFrames(30,10);
  Dot5x7.scrollRightString("World!", 700, 60, 1);
  delay(3000);
}
