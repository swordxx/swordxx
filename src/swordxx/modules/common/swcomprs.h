/******************************************************************************
 *
 *  swcomprs.h -    definition of Class SWCompress used for data
 *            compression
 *
 * $Id$
 *
 * Copyright 1996-2014 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifndef SWCOMPRS_H
#define SWCOMPRS_H

#include "../../defs.h"


namespace swordxx {

class SWDLLEXPORT SWCompress
{
  void reset();
  void cycleStream ();
protected:
  char *buf, *zbuf, direct;    // 0 - encode; 1 - decode
  unsigned long zlen, zpos, pos, slen;
  int level;
public:
  SWCompress ();
  virtual ~ SWCompress ();
  virtual char * Buf(char const * buf = nullptr, unsigned long * len = nullptr);
  virtual char * zBuf(unsigned long * len, char * buf = nullptr);
  virtual unsigned long GetChars (char *buf, unsigned long len);    // override for other than buffer compression
  virtual unsigned long SendChars (char *buf, unsigned long len);    // override for other than buffer compression
  virtual void Encode ();    // override to provide compression algorythm
  virtual void Decode ();    // override to provide compression algorythm
  virtual void setLevel(int l) {level = l;};
  virtual int getLevel() {return level;};
};

} /* namespace swordxx */
#endif
