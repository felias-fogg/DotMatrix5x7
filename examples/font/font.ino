// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>


#define SCROLLTIME 70
#define SHOWTIME 500

void setup(void)
{
  Dot5x7.begin(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  Dot5x7.setFramesPerSecond(100);
  Dot5x7.setUpsideDown(false);
}

void loop(void)
{
  int c;
  for (c=0; c<256; c++) {
    Dot5x7.show(c);	
    delay(SHOWTIME);
    for (byte i=0; i<NUMCOLS+1+1; i++) {
      Dot5x7.scrollRight(c,c+1,1,i);
      delay(SCROLLTIME);
    }
  }
  delay(5000);
}
