/*
 *
 * $Id: cryptfilt.h,v 1.1 1999/09/05 02:21:59 scribe Exp $
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

#ifndef CRYPTFILT_H
#define CRYPTFILT_H

#include <swfilter.h>
#include <swcrypt.h>

class CryptFilter : public SWFilter {
	SWCrypt *crypt;
public:
	CryptFilter(const char *key);
	virtual ~CryptFilter();
	virtual char ProcessText(char *text, int maxlen = -1);
};

#endif
