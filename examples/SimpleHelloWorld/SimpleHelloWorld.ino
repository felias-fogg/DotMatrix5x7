// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>

// standard ascii 5x7 font incl. the extensions:
#define aUML "\x84" // ä = 0x84
#define oUML "\x94" // ö = 0x94
#define uUML "\x81" // ü = 0x81
#define sZET "\xE0" // ß = 0xE0 (this is an error, should be 0xE1)
#define AUML "\x8E" // Ä = 0x8E
#define OUML "\x99" // Ö = 0x99
#define UUML "\x9A" // Ü = 0x9A

char str[]="Hello World!";

void setup(void)
{
  Dot5x7.begin(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  Dot5x7.setFramesPerSecond(100);
}

void loop(void)
{
  int i;
  Dot5x7.setUpsideDown(false);
  Dot5x7.clear();
  for (i=0; i< strlen(str); i++) {
    Dot5x7.show(str[i]);
    delay(1500);
    Dot5x7.clear();
    delay(150);
  }
  delay(3000);
  Dot5x7.setUpsideDown(true);
  Dot5x7.showString(str, 600, 100);
  delay(3000);
}
