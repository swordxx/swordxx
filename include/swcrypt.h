/******************************************************************************
 *  swcrypt.h   - definition of Class SWCrypt used for data en/decryption
 *
 * $Id: swcrypt.h,v 1.1 1999/09/05 02:21:35 scribe Exp $
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

#ifndef SWCRYPT_H
#define SWCRYPT_H

#include "sapphire.h"

class SWCrypt {
	sapphire master;
	sapphire work;
	char *buf;
	bool encrypt;
	int len;
protected:
public:
	SWCrypt(unsigned char *key);
	virtual ~SWCrypt();
	virtual char *Buf(const char *buf = 0);
	virtual char *cryptBuf(unsigned int *len, const char *buf = 0);
	virtual void Encode(void);
	virtual void Decode(void);
};

#endif
