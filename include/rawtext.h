/******************************************************************************
 *  rawtext.h   - code for class 'RawText'- a module that reads raw text files:
 *		  ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id: rawtext.h,v 1.3 2000/08/19 03:36:46 scribe Exp $
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

#ifndef RAWTEXT_H
#define RAWTEXT_H

#include <rawverse.h>
#include <rawstr.h>
#include <swtext.h>

class RawText : public SWText, public RawVerse {
	char *versebuf;
	RawStr *fastSearch[2];

public:
	RawText(const char *ipath, const char *iname = 0, const char *idesc = 0, SWDisplay *idisp = 0);
	virtual ~RawText();
	virtual operator char*();
	virtual char createSearchFramework();
	virtual bool hasSearchFramework() { return true; }
	virtual RawText &operator =(POSITION);
	virtual ListKey &Search(const char *istr, int searchType = 0, int flags = 0, SWKey *scope = 0, bool *justCheckIfSupported = 0, void (*percent)(char, void *) = &SWModule::nullPercent, void *percentUserData = 0);
};


#endif
