/******************************************************************************
 *  rawcom.h   - code for class 'RawCom'- a module that reads raw commentary
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id: rawcom.h,v 1.2 2000/10/15 11:25:31 scribe Exp $
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

#ifndef RAWCOM_H
#define RAWCOM_H

#include <rawverse.h>
#include <swcom.h>

class RawCom : public RawVerse, public SWCom {
	char *versebuf;

public:
	RawCom(const char *ipath, const char *iname = 0, const char *idesc = 0, SWDisplay *idisp = 0);
	virtual ~RawCom();
	virtual char *getRawEntry();

	virtual SWModule &operator +=(int increment);
	virtual SWModule &operator -=(int decrement) { return this->operator +=(-decrement); }
};


#endif
