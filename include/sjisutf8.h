/*
 *
 * Copyright 2001 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

#ifndef SJISUTF8_H
#define SJISUTF8_H

#include <swfilter.h>

#include <defs.h>

  /** This filter converts Shift-JIS encoded text to UTF-8
  */
class SWDLLEXPORT SJISUTF8:public SWFilter {
public:
  SJISUTF8();
  virtual char ProcessText (char *text, int maxlen, const SWKey *key);
  unsigned long SJIStoUTF32(unsigned long);
};

#endif
