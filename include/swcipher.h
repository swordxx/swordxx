/******************************************************************************
 *  swcipher.h   - definition of Class SWCipher used for data cipher/decipher
 *
 * $Id: swcipher.h,v 1.1 1999/09/05 21:47:16 scribe Exp $
 *
 * Copyright 1999 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWCIPHER_H
#define SWCIPHER_H

#include "sapphire.h"

class SWCipher {

	sapphire master;
	sapphire work;

	char *buf;
	bool cipher;
	int len;
protected:
public:
	SWCipher(unsigned char *key);
	virtual ~SWCipher();
	virtual char *Buf(const char *buf = 0);
	virtual char *cipherBuf(unsigned int *len, const char *buf = 0);
	virtual void Encode(void);
	virtual void Decode(void);
};

#endif
