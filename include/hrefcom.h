/******************************************************************************
 *  hrefcom.h - code for class 'HREFCom'- a module that produces HTML HREFs
 *			pointing to actual text desired.  Uses standard
 *			files:	ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id: hrefcom.h,v 1.3 2001/02/08 09:20:48 chrislit Exp $
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

#ifndef HREFCOM_H
#define HREFCOM_H

#include <rawverse.h>
#include <swcom.h>

#include <defs.h>

class SWDLLEXPORT HREFCom : public RawVerse, public SWCom {
	char *versebuf;
	char *prefix;

public:
	HREFCom(const char *ipath, const char *prefix, const char *iname = 0, const char *idesc = 0, SWDisplay *idisp = 0);
	virtual ~HREFCom();
	virtual char *getRawEntry();
};


#endif
