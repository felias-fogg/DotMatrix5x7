// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>

const char PROGMEM str[] = " World";

void setup(void)
{
  Dot5x7.begin(2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);
  Dot5x7.setFramesPerSecond(100);
}

void loop(void)
{
  Dot5x7.showString("Say:",500,100);
  Dot5x7.showString(F("Hello"),500,100);
  Dot5x7.showString_P(str,500,100);
  Dot5x7.showString("!",500,100);
  delay(2000);
  Dot5x7.scrollLeftString("Say:",300,50,1);
  Dot5x7.scrollLeftString(F("Hello"),300,50,1);
  Dot5x7.scrollLeftString_P(str,300,50,1);
  Dot5x7.scrollLeftString("!",300,50,1);
  delay(2000);
  Dot5x7.scrollRightString("Say:",300,50,1);
  Dot5x7.scrollRightString(F("Hello"),300,50,1);
  Dot5x7.scrollRightString_P(str,300,50,1);
  Dot5x7.scrollRightString("!",300,50,1);
  delay(2000);
  Dot5x7.scrollUpString("Say:",300,50,1);
  Dot5x7.scrollUpString(F("Hello"),300,50,1);
  Dot5x7.scrollUpString_P(str,300,50,1);
  Dot5x7.scrollUpString("!",300,50,1);
  delay(2000);
  Dot5x7.scrollDownString("Say:",300,50,1);
  Dot5x7.scrollDownString(F("Hello"),300,50,1);
  Dot5x7.scrollDownString_P(str,300,50,1);
  Dot5x7.scrollDownString("!",300,50,1);
  delay(3000);
}
