// -*- c++ -*-
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include <DotMatrix5x7.h>


#define SCROLLTIME 250
#define SHOWTIME 1000
char str[]="Hello World!";
char str1[]="Foo Bar";

void setup(void)
{
  Dot5x7.begin(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
  Dot5x7.setFramesPerSecond(50);
  Dot5x7.setUpsideDown(true);
}

void loop(void)
{
  int i,j;
  bool usd;
  Dot5x7.setUpsideDown(false);
  Dot5x7.scrollDownString(str1, 800, 100, 1);

  for (usd = false; true; usd++) {
    Dot5x7.setUpsideDown(usd);
    Dot5x7.clear();
    Dot5x7.show(str[0]);
    delay(SHOWTIME);
    for (i=0; i< strlen(str)-1; i++) {
      for (j=0; j<8+8; j++) {
	Dot5x7.scrollUp(str[i],str[i+1],0,j);
	delay(SCROLLTIME);
      }
      delay(SHOWTIME);
    }
    delay(SHOWTIME*3);
    if (usd) break;
  }
}
