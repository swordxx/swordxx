/******************************************************************************
 *  rawld.cpp - code for class 'RawLD'- a module that reads raw lexicon and
 *				dictionary files: *.dat *.idx
 *
 * $Id: rawld.h,v 1.2 2000/10/15 11:25:31 scribe Exp $
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

#ifndef RAWLD_H
#define RAWLD_H

#include <rawstr.h>
#include <swld.h>

class RawLD : public RawStr, public SWLD {
	void strongsPad(char *buf);
	char getEntry(long away = 0);

public:
	RawLD(const char *ipath, const char *iname = 0, const char *idesc = 0, SWDisplay *idisp = 0);
	virtual ~RawLD();
	virtual char *getRawEntry();

	virtual SWModule &operator =(POSITION pos);
	virtual SWModule &operator +=(int increment);
	virtual SWModule &operator -=(int decrement) { return this->operator +=(-decrement); }
};


#endif
