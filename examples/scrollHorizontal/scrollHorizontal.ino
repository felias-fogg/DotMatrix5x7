// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>


#define SCROLLTIME 250
#define SHOWTIME 1000
char str[]="Hello World!";

void setup(void)
{
  Dot5x7.begin(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
  Dot5x7.setFramesPerSecond(100);
}

void loop(void)
{
  Dot5x7.setUpsideDown(false);
  Dot5x7.show('A');
  delay(500);
  for (byte i=0; i<NUMCOLS+1+1; i++) {
    Dot5x7.scrollRight('A','B',1,i);
    delay(500);
  }
  delay(1000);
  Dot5x7.clear();
  Dot5x7.show('A');
  delay(500);
  for (byte i=0; i<NUMCOLS+1+1; i++) {
    Dot5x7.scrollLeft('A','B',1,i);
    delay(500);
  }
  delay(1000);
  Dot5x7.clear();
  Dot5x7.scrollRightString(str,500,70,0);
  delay(2000);
}
