/******************************************************************************
 *  rawgbf.h   - code for class 'RawGBF'- a module that reads raw text files:
 *		  ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id: rawgbf.h,v 1.2 2001/02/08 09:20:48 chrislit Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef RAWGBF_H
#define RAWGBF_H

#include <rawverse.h>
#include <swtext.h>
#include <swfilter.h>

#include <defs.h>

class SWDLLEXPORT RawGBF : public SWText, public RawVerse {
	char *versebuf;

public:
	RawGBF(const char *ipath, const char *iname = 0, const char *idesc = 0, SWDisplay *idisp = 0);
	virtual ~RawGBF();
	virtual operator char*();
};


#endif
