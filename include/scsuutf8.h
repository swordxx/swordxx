/******************************************************************************
 *
 *  scsuutf8.h - SWFilter descendant to convert a SCSU character to UTF-8
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SCSUUTF8_H
#define SCSUUTF8_H

#include <swfilter.h>

SWORD_NAMESPACE_START

/** This filter converts SCSU compressed (encoded) text to UTF-8
 */
class SWDLLEXPORT SCSUUTF8 : public SWFilter {
  unsigned long c, d;
  unsigned char* UTF8Output(unsigned long, unsigned char* text);
  
public:
	SCSUUTF8();
	virtual char processText(SWBuf &text, const SWKey *key = 0, const SWModule *module = 0);
};

SWORD_NAMESPACE_END
#endif
