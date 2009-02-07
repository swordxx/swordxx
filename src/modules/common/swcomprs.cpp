/******************************************************************************
 *  swcomprs.cpp   - code for class 'SWCompress'- a driver class that provides
 *				compression utilities.
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swcomprs.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * SWCompress Constructor - Initializes data for instance of SWCompress
 *
 */

SWCompress::SWCompress()
{
	buf = zbuf = 0;
	Init();
}


/******************************************************************************
 * SWCompress Destructor - Cleans up instance of SWCompress
 */

SWCompress::~SWCompress()
{
	if (zbuf)
		free(zbuf);

	if (buf)
		free(buf);
}


void SWCompress::Init()
{
		if (buf)
			free(buf);

		if (zbuf)
			free(zbuf);

		buf    = 0;
		zbuf   = 0;
		direct  = 0;
		zlen    = 0;
		slen    = 0;
		zpos    = 0;
		pos     = 0;
}


char *SWCompress::Buf(const char *ibuf, unsigned long *len) {
	// setting an uncompressed buffer
	if (ibuf) {
		Init();
		slen = (len) ? *len : strlen(ibuf);
		buf = (char *) calloc(slen + 1, 1);
		memcpy(buf, ibuf, slen);
	}

	// getting an uncompressed buffer
	if (!buf) {
		buf = (char *)calloc(1,1); // be sure we at least allocate an empty buf for return;
		direct = 1;
		Decode();
//		slen = strlen(buf);
		if (len)
			*len = slen;
	}
	return buf;
}


char *SWCompress::zBuf(unsigned long *len, char *ibuf)
{
	// setting a compressed buffer
	if (ibuf) {
		Init();
		zbuf = (char *) malloc(*len);
		memcpy(zbuf, ibuf, *len);
		zlen = *len;
	}

	// getting a compressed buffer
	if (!zbuf) {
		direct = 0;
		Encode();
	}

	*len = zlen;
	return zbuf;
}


unsigned long SWCompress::GetChars(char *ibuf, unsigned long len)
{
	if (direct) {
		len = (((zlen - zpos) > (unsigned)len) ? len : zlen - zpos);
		if (len > 0) {
			memmove(ibuf, &zbuf[zpos], len);
			zpos += len;
		}
	}
	else {
//		slen = strlen(buf);
		len = (((slen - pos) > (unsigned)len) ? len : slen - pos);
		if (len > 0) {
			memmove(ibuf, &buf[pos], len);
			pos += len;
		}
	}
	return len;
}
	

unsigned long SWCompress::SendChars(char *ibuf, unsigned long len)
{
	if (direct) {
		if (buf) {
//			slen = strlen(buf);
			if ((pos + len) > (unsigned)slen) {
				buf = (char *) realloc(buf, pos + len + 1024);
				memset(&buf[pos], 0, len + 1024);
			}
		}
		else	buf = (char *)calloc(1, len + 1024);
		memmove(&buf[pos], ibuf, len);
		pos += len;
	}
	else {
		if (zbuf) {
			if ((zpos + len) > zlen) {
				zbuf = (char *) realloc(zbuf, zpos + len + 1024);
				zlen = zpos + len + 1024;
			}
		}
		else {
			zbuf = (char *)calloc(1, len + 1024);
			zlen = len + 1024;
		}
		memmove(&zbuf[zpos], ibuf, len);
		zpos += len;
	}
	return len;
}


/******************************************************************************
 * SWCompress::Encode	- This function "encodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void SWCompress::Encode(void)
{
	cycleStream();
}


/******************************************************************************
 * SWCompress::Decode	- This function "decodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void SWCompress::Decode(void)
{
	cycleStream();
}


void SWCompress::cycleStream() {
	char buf[1024];
	unsigned long len, totlen = 0;

	do {
		len = GetChars(buf, 1024);
		if (len)
			totlen += SendChars(buf, len);
	} while (len == 1024);

	zlen = slen = totlen;
}

SWORD_NAMESPACE_END
