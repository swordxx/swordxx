/*
 *
 * $Id: cipherfil.h,v 1.8 2002/07/28 01:48:38 scribe Exp $
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

#ifndef CIPHERFIL_H
#define CIPHERFIL_H

#include <swfilter.h>
#include <swcipher.h>

#include <defs.h>

class SWDLLEXPORT CipherFilter:public SWFilter {
	SWCipher *cipher;
public:
	CipherFilter(const char *key);
	virtual ~CipherFilter();
	virtual char ProcessText(char *text, int maxlen, const SWKey *key, const SWModule * = 0);
	virtual SWCipher *getCipher();
};

#endif
