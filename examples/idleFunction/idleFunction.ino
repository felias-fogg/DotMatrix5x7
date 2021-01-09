// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>
#include <LowPower.h>

char str[]="Hello World!";

void myIdle(long unsigned int period)
{
  unsigned long time_now = millis();
  while (millis() < time_now + period)
    LowPower.idle(SLEEP_FOREVER); // go into idel mode until next timer interrupt
}

void setup(void)
{
  Dot5x7.begin(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
  Dot5x7.setFramesPerSecond(100);
  Dot5x7.setDelayFunction(myIdle);
}

void loop(void)
{
  int i;
  Dot5x7.setUpsideDown(false);
  Dot5x7.showString(str, 600, 100);
  myIdle(3000);
}
