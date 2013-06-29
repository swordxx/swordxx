/******************************************************************************
 *
 *  swcipher.cpp -	code for class 'SWCipher'- a driver class that
 *			provides cipher utilities
 *
 * $Id$
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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


#include <stdlib.h>
#include <string.h>
#include <swcipher.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWCipher Constructor - Initializes data for instance of SWCipher
 *
 */

SWCipher::SWCipher(unsigned char *key) {
	master.initialize(key, strlen((char *)key));
	buf = 0;
}


/******************************************************************************
 * SWCipher Destructor - Cleans up instance of SWCipher
 */

SWCipher::~SWCipher()
{
	if (buf)
		free(buf);
}


char *SWCipher::Buf(const char *ibuf, unsigned long ilen)
{
	if (ibuf) {
	
		if (buf)
			free(buf);

		if (!ilen) {
		        len = strlen(buf);
			ilen = len + 1;
		}
		else len = ilen;

		buf = (char *) malloc(ilen);
		memcpy(buf, ibuf, ilen);
		cipher = false;
	}

	Decode();

	return buf;
}


char *SWCipher::cipherBuf(unsigned long *ilen, const char *ibuf)
{
	if (ibuf) {
	
		if (buf)
			free(buf);
			
		buf = (char *) malloc(*ilen+1);
		memcpy(buf, ibuf, *ilen);
		len = *ilen;
		cipher = true;
	}

	Encode();

	*ilen = len;
	return buf;
}


/******************************************************************************
 * SWCipher::Encode	- This function "encodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void SWCipher::Encode(void)
{
	if (!cipher) {
		work = master;
		for (unsigned long i = 0; i < len; i++)
			buf[i] = work.encrypt(buf[i]);
		cipher = true;
	}
}


/******************************************************************************
 * SWCipher::Decode	- This function "decodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void SWCipher::Decode(void)
{
	if (cipher) {
		work = master;
		unsigned long i;
		for (i = 0; i < len; i++)
			buf[i] = work.decrypt(buf[i]);
		buf[i] = 0;
		cipher = false;
	}
}


/******************************************************************************
 * SWCipher::setCipherKey	- setter for a new CipherKey
 *
 */

void SWCipher::setCipherKey(const char *ikey) {
	unsigned char *key = (unsigned char *)ikey;
	master.initialize(key, strlen((char *)key));
}

SWORD_NAMESPACE_END
