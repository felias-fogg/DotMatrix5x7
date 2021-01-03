
#ifndef DOTMATRIX5X7_H
#define DOTMATRIX5X7_H

#include <Arduino.h>

#define NUMCOLS 5
#define NUMROWS 7
#define UPDIR 0
#define DOWNDIR 1
#define RIGHTDIR 2
#define LEFTDIR 3

class DotMatrix5x7 {
 public:
  void begin(byte col1, byte col2, byte col3, byte col4, byte col5, byte row1, byte row2, byte row3,
	    byte row4, byte row5, byte row6, byte row7);
  void clear(void);
  void show(byte c);
  void showString(const char str[], int ontime, int offtime);
  void setFont(const byte *f);
  void setFramesPerSecond(int fps);
  void setUpsideDown(bool enable);
  void scrollUp(byte curr, byte next, byte offset, byte step);
  void scrollDown(byte curr, byte next, byte offset, byte step);
  void scrollDownString(char const str[], int showtime, int scrolltime,  int offset);
  void scrollUpString(char const str[], int showtime, int scrolltime,  int offset);
  void scrollLeft(byte curr, byte next, byte offset, byte step);
  void scrollRight(byte curr, byte next, byte offset, byte step);
  void scrollLeftString(char const str[],  int showtime, int scrolltime, int offset);
  void scrollRightString(char const str[],  int showtime, int scrolltime, int offset);
  void scrollString(char const str[],  int showtime, int scrolltime, int offset, byte dir);
  static void displayRow(void);

 private:
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
  volatile byte _row[NUMROWS];
  volatile byte _currrow = 0;
  volatile bool _blocked = false;
  volatile byte _blank = 0;
};

extern DotMatrix5x7 Dot5x7;
#endif
