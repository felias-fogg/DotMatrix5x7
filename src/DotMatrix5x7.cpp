// -*- c++ -*-
/* Class to drive a 5x7 dot matrix display 
   using timer1.
*/

#include "dotfont5x7.h"
#include <DotMatrix5x7.h>
#ifndef USETIMER0
#include <TimerOne.h>
#endif

DotMatrix5x7 Dot5x7; //preinstantiate

void DotMatrix5x7::begin(byte col1, byte col2, byte col3, byte col4, byte col5, byte row1, byte row2, byte row3,
			 byte row4, byte row5, byte row6, byte row7)
{
  _colpin[0] = col1;
  _colpin[1] = col2;
  _colpin[2] = col3;
  _colpin[3] = col4;
  _colpin[4] = col5;
  _rowpin[0] = row1;
  _rowpin[1] = row2;
  _rowpin[2] = row3;
  _rowpin[3] = row4;
  _rowpin[4] = row5;
  _rowpin[5] = row6;
  _rowpin[6] = row7;
  for (byte r=0; r < NUMROWS; r++) {
    pinMode(_rowpin[r], OUTPUT);
    digitalWrite(_rowpin[r], !_rowactive);
  }
  for (byte c=0; c < NUMCOLS; c++) {
    pinMode(_colpin[c], OUTPUT);
    digitalWrite(_colpin[c],  !_columnactive);
  }
  _currrow = 0;
  setFont(font5x7);
  for (_currrow = 0; _currrow < NUMROWS; _currrow++) _row[_currrow] = 0;
  _currrow = 0;
  setFramesPerSecond(50);
}

void DotMatrix5x7::begin(byte col1, byte col2, byte col3, byte col4, byte col5, byte row1, byte row2, byte row3,
			 byte row4, byte row5, byte row6, byte row7, byte rowactive, byte columnactive)
{
  _rowactive = rowactive;
  _columnactive = columnactive;
  begin(col1, col2, col3, col4, col5, row1, row2, row3, row4, row5, row6, row7);
}

void DotMatrix5x7::setDelayFunction(void (*f) (long unsigned int))
{
  _delay = f;
}

void DotMatrix5x7::setUpsideDown(bool enable)
{
  _upsidedown = enable;
}

void DotMatrix5x7::setFont(const byte *f)
{
  _font = (byte*)f;
}

void DotMatrix5x7::setFont(void)
{
  setFont(font5x7);
}

// stop timer and set all output pins to inactive
void DotMatrix5x7::sleep(void) 
{
#ifdef USETIMER0
#ifdef TIMSK0
   TIMSK0 &= ~_BV(OCIE0A); // deactivate compare interrupt of Timer0
#else
   TIMSK &= ~_BV(OCIE0A); // deactivate compare interrupt of Timer0
#endif
#else
   Timer1.stop();
#endif
   for (byte c=0; c < NUMCOLS; c++) digitalWrite(_colpin[c], !_columnactive);
   for (byte r=0; r < NUMROWS; r++) digitalWrite(_rowpin[r], !_rowactive);
}

void DotMatrix5x7::wakeup(void)
{
#ifdef USETIMER0
#ifdef TIMSK0
   TIMSK0 |= _BV(OCIE0A); // deactivate compare interrupt of Timer0
#else
   TIMSK |= _BV(OCIE0A); // deactivate compare interrupt of Timer0
#endif
#else
   Timer1.restart();
#endif
}


#ifdef USETIMER0
#ifdef TIMER0_COMPA_vect
ISR(TIMER0_COMPA_vect)
#else
ISR(TIM0_COMPA_vect)
#endif
{
  Dot5x7.displayRow();
}
#endif

void DotMatrix5x7::setFramesPerSecond(int fps)
{
   _period = 1000000UL/(fps*NUMROWS);
#ifdef USETIMER0
   OCR0A = 0xAF;
#ifdef TIMSK0
   TIMSK0 |= _BV(OCIE0A); // deactivate compare interrupt of Timer0
#else
   TIMSK |= _BV(OCIE0A); // activate compare interrupt that triggers in the middle of a cycle
#endif
#else
  Timer1.initialize(_period);
  Timer1.attachInterrupt(displayRow);
#endif
}

void DotMatrix5x7::setBlinkFrames(int blinkon, int blinkoff)
{
  _blinkon = blinkon;
  _blinkoff = blinkoff;
}

void DotMatrix5x7::clear(void)
{
  show(' ');
}

void DotMatrix5x7::show(byte c)
{
  byte loccol[NUMCOLS];
  for (byte i=0; i < NUMCOLS; i++)
    loccol[i] = pgm_read_byte(_font+c*NUMCOLS+i);
  _blocked = true;
  transposePattern(loccol,_row);
  if (c == 0x20) {
    if (_blank == 0) _blank = 1;
  } else _blank = 0;
  _blocked = false;
}

void DotMatrix5x7::showString(char const *str, int ontime, int offtime)
{
  showStringWorker(str, ontime, offtime, false);
}

void DotMatrix5x7::showString(const __FlashStringHelper *str, int ontime, int offtime)
{
  showStringWorker(reinterpret_cast<PGM_P>(str), ontime, offtime, true);
}

void DotMatrix5x7::showString_P(char const *str, int ontime, int offtime)
{
  showStringWorker(str, ontime, offtime, true);
}

void DotMatrix5x7::showStringWorker(char const *str, int ontime, int offtime, bool flash)
{
  int i;
  unsigned char c;
  Dot5x7.clear();
  while (true) {
    c = (flash ? pgm_read_byte(str++)  : *str++);
    if (c == 0) break;
    Dot5x7.show(c);
    _delay(ontime);
    Dot5x7.clear();
    _delay(offtime);
  }
  _delay(offtime);
}

void DotMatrix5x7::scrollUp(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayDown(curr, next, offset, step);
  else scrollDisplayUp(curr, next, offset, step);
}

void DotMatrix5x7::scrollUpString(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, UPDIR);
}

void DotMatrix5x7::scrollUpString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset)
{
  scrollString(reinterpret_cast<PGM_P>(str), showtime, scrolltime, offset, UPDIR, true);
}

void DotMatrix5x7::scrollUpString_P(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, UPDIR, true);
}

void DotMatrix5x7::scrollDown(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayUp(curr, next, offset, step);
  else scrollDisplayDown(curr, next, offset, step);
}

void DotMatrix5x7::scrollDownString(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, DOWNDIR);
}

void DotMatrix5x7::scrollDownString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset)
{
  scrollString(reinterpret_cast<PGM_P>(str), showtime, scrolltime, offset, DOWNDIR, true);
}

void DotMatrix5x7::scrollDownString_P(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, DOWNDIR, true);
}

void DotMatrix5x7::scrollRight(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayLeft(curr, next, offset, step);
  else scrollDisplayRight(curr, next, offset, step);
}

void DotMatrix5x7::scrollRightString(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, RIGHTDIR);
}

void DotMatrix5x7::scrollRightString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset)
{
  scrollString(reinterpret_cast<PGM_P>(str), showtime, scrolltime, offset, RIGHTDIR, true);
}

void DotMatrix5x7::scrollRightString_P(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, RIGHTDIR, true);
}

void DotMatrix5x7::scrollLeft(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayRight(curr, next, offset, step);
  else scrollDisplayLeft(curr, next, offset, step);
}


void DotMatrix5x7::scrollLeftString(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, LEFTDIR);
}

void DotMatrix5x7::scrollLeftString(const __FlashStringHelper *str,  int showtime, int scrolltime, int offset)
{
  scrollString(reinterpret_cast<PGM_P>(str), showtime, scrolltime, offset, LEFTDIR, true);
}

void DotMatrix5x7::scrollLeftString_P(char const *str,  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, LEFTDIR, true);
}

void DotMatrix5x7::scrollString(char const *str,  int showtime, int scrolltime, int offset, byte dir, bool flash)
{
  bool stringend = false;
  char curr = ' ', next;
  clear();
  while (true) {
    next = (flash ? pgm_read_byte(str++) : *str++);
    if (next == 0) {
      next = ' ';
      stringend = true;
    }
    for (byte j=1; j<NUMROWS+offset-1; j++) {
      switch (dir) {
      case UPDIR: scrollUp(curr,next,offset,j); break;
      case DOWNDIR: scrollDown(curr,next,offset,j); break;
      case RIGHTDIR: scrollRight(curr,next,offset,j); break;
      case LEFTDIR: scrollLeft(curr,next,offset,j); break;
      }
      _delay(scrolltime);
    }
    if (stringend) break;
    _delay(showtime);
    curr = next;
  }
  clear();
  _delay(showtime);
}


void DotMatrix5x7::scrollDisplayLeft(byte curr, byte next, byte offset, byte step)
{
   byte loccol[NUMCOLS];
   byte c, filled = 0;

   if (step > NUMCOLS+offset) step = NUMCOLS+offset;
   for (c = 0; c < NUMCOLS; c++) loccol[c] = 0;
   for (c = 0; c < NUMCOLS-step; c++) {
     loccol[c] = pgm_read_byte(_font+curr*NUMCOLS+c+step);
   }
   for (c = 0; c < NUMCOLS && NUMCOLS+offset-step+c >= 0 && NUMCOLS+offset-step+c < NUMCOLS; c++) 
     loccol[NUMCOLS+offset-step+c] =  pgm_read_byte(_font+next*NUMCOLS+c);
   _blocked = true;
   transposePattern(loccol,_row);
   _blocked = false;
   _blank = 0;
}

void DotMatrix5x7::scrollDisplayRight(byte curr, byte next, byte offset, byte step)
{
  if (step > NUMCOLS+offset) step = NUMCOLS+offset;
  scrollDisplayLeft(next, curr, offset, (NUMCOLS+offset)-step);
}


void DotMatrix5x7::scrollDisplayUp(byte curr, byte next, byte offset, byte step)
{
  byte loccol[NUMCOLS];
  byte crows[NUMROWS], nrows[NUMROWS];
  byte r;
  if (step > NUMROWS+offset) step = NUMROWS+offset;
  for (byte i=0; i < NUMCOLS; i++)
    loccol[i] = pgm_read_byte(_font+curr*NUMCOLS+i);
  transposePattern(loccol, crows);
  for (byte i=0; i < NUMCOLS; i++)
    loccol[i] = pgm_read_byte(_font+next*NUMCOLS+i);
  transposePattern(loccol, nrows);
  _blocked = true;
  for (r=0; r<NUMROWS; r++) 
    if (r+step < NUMROWS) _row[r] = crows[r+step];
    else _row[r] = 0;
  if (step > offset) {
    for (r=0; r<step-offset; r++) 
      _row[NUMROWS+(offset-step)+r] = nrows[r];
  }
  _blank = 0;
  _blocked = false;
}

void DotMatrix5x7::scrollDisplayDown(byte curr, byte next, byte offset, byte step)
{
  if (step > NUMROWS+offset) step = NUMROWS+offset;
  scrollDisplayUp(next, curr, offset, NUMROWS+offset-step);
}


void DotMatrix5x7::transposePattern(byte cols[], volatile byte rows[])
{
  byte cmask;
  byte realr;
  cmask = (_upsidedown ? 0x40 : 1);
  for (byte r=0; r < NUMROWS; r++) {
    rows[r] = 0;
    for (byte c=0; c < NUMCOLS; c++) {
      if (cmask&cols[(_upsidedown ? (NUMCOLS-c-1) : c)]) rows[r] |= 1;
      rows[r] = rows[r] << 1;
    }
    if (_upsidedown) cmask = cmask >> 1;
    else cmask = cmask << 1;
    rows[r] = rows[r] << (7-NUMCOLS);
  }
}

void DotMatrix5x7::displayRow(void)
{
  byte pat;
  byte mask = B10000000;
  if (Dot5x7._blank >= 8 || Dot5x7._blocked) return;
  digitalWrite(Dot5x7._rowpin[Dot5x7._currrow], !Dot5x7._rowactive);
  Dot5x7._currrow++;
  if (Dot5x7._currrow >= NUMROWS) Dot5x7._currrow = 0;
  if (Dot5x7._blinkon == 0 || Dot5x7._blinkoff == 0 || Dot5x7._blinkcnt++ < Dot5x7._blinkon*7) {
    pat = Dot5x7._row[Dot5x7._currrow];
    if (Dot5x7._blank && pat == 0) Dot5x7._blank++;
  } else {
    pat = 0;
  }
  if (Dot5x7._blinkcnt >= (Dot5x7._blinkon+Dot5x7._blinkoff)*7) Dot5x7._blinkcnt = 0;
  for (byte c=0; c < NUMCOLS; c++) {
    digitalWrite(Dot5x7._colpin[c], ((mask&pat) ? Dot5x7._columnactive : !Dot5x7._columnactive));
    mask = mask>>1;
  }
  digitalWrite(Dot5x7._rowpin[Dot5x7._currrow], Dot5x7._rowactive);
}

