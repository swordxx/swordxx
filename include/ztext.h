/******************************************************************************
 *  ztext.h   - code for class 'zText'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 *
 * $Id: ztext.h,v 1.1 1999/05/04 22:03:36 scribe Exp $
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

#ifndef ZTEXT_H
#define ZTEXT_H

#include <rawverse.h>
#include <swtext.h>
#include <swcomprs.h>

class zText : public RawVerse, public SWText, public SWCompress {
	char *versebuf;
public:
	zText(char *ipath, char *iname = 0, char *idesc = 0, SWDisplay *idisp = 0);
	virtual ~zText();
	virtual operator char*();
};


#endif
