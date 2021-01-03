// -*- c++ -*-
/* Class to drive a 5x7 dot matrix display 
   using timer1.
*/

#include <TimerOne.h>
#include <dotfont5x7.h>
#include <DotMatrix5x7.h>

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
    digitalWrite(_rowpin[r], HIGH);
  }
  for (byte c=0; c < NUMCOLS; c++) {
    pinMode(_colpin[c], OUTPUT);
    digitalWrite(_colpin[c], LOW);
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

void DotMatrix5x7::setUpsideDown(bool enable)
{
  _upsidedown = enable;
}

void DotMatrix5x7::clear(void)
{
  show(' ');
}

void DotMatrix5x7::setFont(const byte *f)
{
  _font = (byte*)f;
}

void DotMatrix5x7::setFramesPerSecond(int fps)
{
   _period = 1000000UL/(fps*NUMROWS);  
  Timer1.initialize(_period);
  Timer1.attachInterrupt(displayRow);
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

void DotMatrix5x7::showString(char const str[], int ontime, int offtime)
{
  int i;
  Dot5x7.clear();
  for (i=0; i< strlen(str); i++) {
    Dot5x7.show(str[i]);
    delay(ontime);
    Dot5x7.clear();
    delay(offtime);
  }
  delay(offtime);
}

void DotMatrix5x7::scrollUp(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayDown(curr, next, offset, step);
  else scrollDisplayUp(curr, next, offset, step);
}

void DotMatrix5x7::scrollUpString(char const str[],  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, UPDIR);
}

void DotMatrix5x7::scrollDown(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayUp(curr, next, offset, step);
  else scrollDisplayDown(curr, next, offset, step);
}

void DotMatrix5x7::scrollDownString(char const str[],  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, DOWNDIR);
}

void DotMatrix5x7::scrollRight(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayLeft(curr, next, offset, step);
  else scrollDisplayRight(curr, next, offset, step);
}

void DotMatrix5x7::scrollRightString(char const str[],  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, RIGHTDIR);
}

void DotMatrix5x7::scrollLeft(byte curr, byte next, byte offset, byte step)
{
  if (_upsidedown) scrollDisplayRight(curr, next, offset, step);
  else scrollDisplayLeft(curr, next, offset, step);
}


void DotMatrix5x7::scrollLeftString(char const str[],  int showtime, int scrolltime, int offset)
{
  scrollString(str, showtime, scrolltime, offset, LEFTDIR);
}

void DotMatrix5x7::scrollString(char const str[],  int showtime, int scrolltime, int offset, byte dir)
{
  clear();
  show(str[0]);
  delay(showtime);
  for (byte i=0; i< strlen(str)-1; i++) {
    for (byte j=0; j<NUMROWS+1+offset; j++) {
      switch (dir) {
      case UPDIR: scrollUp(str[i],str[i+1],offset,j); break;
      case DOWNDIR: scrollDown(str[i],str[i+1],offset,j); break;
      case RIGHTDIR: scrollRight(str[i],str[i+1],offset,j); break;
      case LEFTDIR: scrollLeft(str[i],str[i+1],offset,j); break;
      }
      delay(scrolltime);
    }
    delay(showtime);
  }
  clear();
  delay(showtime);
}


void DotMatrix5x7::scrollDisplayRight(byte curr, byte next, byte offset, byte step)
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

void DotMatrix5x7::scrollDisplayLeft(byte curr, byte next, byte offset, byte step)
{
  if (step > NUMCOLS+offset) step = NUMCOLS+offset;
  scrollDisplayRight(next, curr, offset, (NUMCOLS+offset)-step);
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
  if (Dot5x7._blank >= 8) return;
  digitalWrite(Dot5x7._rowpin[Dot5x7._currrow], !Dot5x7._rowactive);
  Dot5x7._currrow++;
  if (Dot5x7._currrow >= NUMROWS) Dot5x7._currrow = 0;
  if (Dot5x7._blocked) pat = 0;
  else {
    pat = Dot5x7._row[Dot5x7._currrow];
    if (Dot5x7._blank && pat == 0) Dot5x7._blank++;
  }
  for (byte c=0; c < NUMCOLS; c++) {
    digitalWrite(Dot5x7._colpin[c], ((mask&pat) ? Dot5x7._columnactive : !Dot5x7._columnactive));
    mask = mask>>1;
  }
  digitalWrite(Dot5x7._rowpin[Dot5x7._currrow], Dot5x7._rowactive);
}

