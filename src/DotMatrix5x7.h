
#ifndef DOTMATRIX5X7_H
#define DOTMATRIX5X7_H

#define USETIMER0 1
#define USETIMER1 0

#include <Arduino.h>
#if USETIMER1
#include <TimerOne.h>
#endif

#if !USETIMER0 && !defined(TIMER1_A_PIN)
#error "Either Timer 0 or Timer 1 needs to be available for generating interrupts for DotMatrix5x7"
#endif

#define NUMCOLS 5
#define NUMROWS 7
#define UPDIR 0
#define DOWNDIR 1
#define RIGHTDIR 2
#define LEFTDIR 3


// Codes for German Umlauts:
#define aUML "\x84" // ä = 0x84
#define oUML "\x94" // ö = 0x94
#define uUML "\x81" // ü = 0x81
#define sZET "\xE0" // ß = 0xE1
#define AUML "\x8E" // Ä = 0x8E
#define OUML "\x99" // Ö = 0x99
#define UUML "\x9A" // Ü = 0x9A

#define DEGREE "\xF8" 

class DotMatrix5x7 {
 public:
  void begin(byte col1, byte col2, byte col3, byte col4, byte col5, byte row1, byte row2, byte row3,
	     byte row4, byte row5, byte row6, byte row7, byte timer=0);
  void begin(byte col1, byte col2, byte col3, byte col4, byte col5, byte row1, byte row2, byte row3,
	     byte row4, byte row5, byte row6, byte row7, byte rowactive, byte columnactive, byte timer=0);
  void setFont(const byte *f);
  void setFont(void);
  byte getTimer(void);
  void setFramesPerSecond(int fps);
  void setUpsideDown(bool enable);
  void setDelayFunction(void (*f) (long unsigned int));
  void setBlinkFrames(int blinkon, int blinkoff);
  void sleep();
  void wakeup();
  void clear(void);
  void show(byte c);
  void showString(const char *str, int ontime, int offtime);
  void showString(const __FlashStringHelper *str, int ontime, int offtime);
  void showString_P(const char *str, int ontime, int offtime);
  void scrollUp(byte curr, byte next, byte offset, byte step);
  void scrollUpString(char const *str, int showtime, int scrolltime,  int offset);
  void scrollUpString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset);
  void scrollUpString_P(char const *str,  int showtime, int scrolltime, int offset);
  void scrollDown(byte curr, byte next, byte offset, byte step);
  void scrollDownString(char const *str, int showtime, int scrolltime,  int offset);
  void scrollDownString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset);
  void scrollDownString_P(char const *str,  int showtime, int scrolltime, int offset);
  void scrollLeft(byte curr, byte next, byte offset, byte step);
  void scrollRight(byte curr, byte next, byte offset, byte step);
  void scrollRightString(char const *str,  int showtime, int scrolltime, int offset);
  void scrollRightString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset);
  void scrollRightString_P(char const *str,  int showtime, int scrolltime, int offset);
  void scrollLeftString(char const *str,  int showtime, int scrolltime, int offset);
  void scrollLeftString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset);
  void scrollLeftString_P(char const *str,  int showtime, int scrolltime, int offset);
  void scrollString(char const *str,  int showtime, int scrolltime, int offset, byte dir, bool flash=false);
  static void displayRow(void);

 private:
  void showStringWorker(char const *str, int ontime, int offtime, bool flash);
  void scrollDisplayDown(byte curr, byte next, byte offset, byte step);
  void scrollDisplayUp(byte curr, byte next, byte offset, byte step);
  void scrollDisplayRight(byte curr, byte next, byte offset, byte step);
  void scrollDisplayLeft(byte curr, byte next, byte offset, byte step);
  void transposePattern(byte cols[], volatile byte rows[]);
  bool _upsidedown = false;
  unsigned long _period;
  byte _rowpin[NUMROWS];
  byte _colpin[NUMCOLS];
  byte *_font;
  byte _rowactive = LOW;
  byte _columnactive = HIGH;
  byte _timer = 0;
  void (*_delay) (long unsigned int) = delay;
  volatile byte _row[NUMROWS];
  volatile byte _currrow = 0;
  volatile bool _blocked = false;
  volatile byte _blank = 0;
  volatile int _blinkon = 0;
  volatile int _blinkoff = 0;
  volatile int _blinkcnt = 0;
};

extern DotMatrix5x7 Dot5x7;
#endif
