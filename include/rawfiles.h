/******************************************************************************
 *  rawfiles.h - code for class 'RawFiles'- a module that produces HTML HREFs
 *			pointing to actual text desired.  Uses standard
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id: rawfiles.h,v 1.2 2000/10/15 11:25:31 scribe Exp $
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

#ifndef RAWFILES_H
#define RAWFILES_H

#include <rawverse.h>
#include <swcom.h>

class RawFiles : public RawVerse, public SWCom {
	char *versebuf;

	char *getnextfilename();

public:
	RawFiles(const char *ipath, const char *iname = 0, const char *idesc = 0, SWDisplay *idisp = 0);
	virtual ~RawFiles();
	virtual char *getRawEntry();
	virtual SWModule &operator <<(const char *inbuf);
	virtual SWModule &operator <<(const SWKey *inbuf);
	virtual void Delete();
};


#endif
