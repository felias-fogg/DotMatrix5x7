// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>


#define SCROLLTIME 70
#define SHOWTIME 500

void setup(void)
{
  Dot5x7.begin(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
  Dot5x7.setFramesPerSecond(100);
  Dot5x7.setUpsideDown(false);
  Serial.begin(19200);
}

void loop(void)
{
  long c;
  c = Serial.parseInt();
  if (c != 0) 
    Dot5x7.show(c);
}
