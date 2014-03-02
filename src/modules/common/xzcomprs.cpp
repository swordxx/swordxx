/******************************************************************************
 *
 *  xzcomprs.cpp -	XzCompress, a driver class that provides xz (LZMA2)
 *			compression
 *				
 * $Id$
 *
 * Copyright 2000-2014 CrossWire Bible Society (http://www.crosswire.org)
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
#include <stdio.h>
#include <xzcomprs.h>

SWORD_NAMESPACE_START

/******************************************************************************
 * XzCompress Constructor - Initializes data for instance of XzCompress
 *
 */

XzCompress::XzCompress() : SWCompress() {
}


/******************************************************************************
 * XzCompress Destructor - Cleans up instance of XzCompress
 */

XzCompress::~XzCompress() {
}


/******************************************************************************
 * XzCompress::Encode - This function "encodes" the input stream into the
 *			output stream.
 *			The GetChars() and SendChars() functions are
 *			used to separate this method from the actual
 *			i/o.
 * 		NOTE:	must set zlen for parent class to know length of
 * 			compressed buffer.
 */

void XzCompress::Encode(void)
{
	direct = 0;	// set direction needed by parent [Get|Send]Chars()

	// get buffer
	char chunk[1024];
	char *buf = (char *)calloc(1, 1024);
	char *chunkbuf = buf;
	unsigned long chunklen;
	unsigned long len = 0;
	while((chunklen = GetChars(chunk, 1023))) {
		memcpy(chunkbuf, chunk, chunklen);
		len += chunklen;
		if (chunklen < 1023)
			break;
		else	buf = (char *)realloc(buf, len + 1024);
		chunkbuf = buf+len;
	}


	zlen = (long) (len*1.001)+15;  // TODO: check that this is sufficient
	char *zbuf = new char[zlen+1];
	size_t zpos = 0;
	if (len)
	{
		//printf("Doing compress\n");
		if (lzma_block_buffer_encode(&block, NULL, (const uint8_t*)buf, (size_t)len, (uint8_t*)zbuf, &zpos, (size_t)zlen) != LZMA_OK)
		{
			printf("ERROR in compression\n");
		}
		else {
			SendChars(zbuf, zlen);
		}
	}
	else
	{
		fprintf(stderr, "ERROR: no buffer to compress\n");
	}
	delete [] zbuf;
	free (buf);
}


/******************************************************************************
 * XzCompress::Decode - This function "decodes" the input stream into the
 *			output stream.
 *			The GetChars() and SendChars() functions are
 *			used to separate this method from the actual
 *			i/o.
 */

void XzCompress::Decode(void)
{
	// get buffer
	char chunk[1024];
	char *zbuf = (char *)calloc(1, 1024);
	char *chunkbuf = zbuf;
	int chunklen;
	unsigned long zlen = 0;
	while((chunklen = GetChars(chunk, 1023))) {
		memcpy(chunkbuf, chunk, chunklen);
		zlen += chunklen;
		if (chunklen < 1023)
			break;
		else	zbuf = (char *)realloc(zbuf, zlen + 1024);
		chunkbuf = zbuf + zlen;
	}

	//printf("Decoding complength{%ld} uncomp{%ld}\n", zlen, blen);
	if (zlen) {
		unsigned long blen = zlen*20;	// trust compression is less than 1000%
		char *buf = new char[blen]; 
		//printf("Doing decompress {%s}\n", zbuf);
		slen = 0;
		size_t zpos = 0;
		size_t bpos = 0;

		switch (lzma_block_buffer_decode(&block, NULL, (const uint8_t*)zbuf, &zpos, (size_t)zlen, (uint8_t*)buf, &bpos, (size_t)&blen)){
			case LZMA_OK: SendChars(buf, blen); slen = blen; break;
			case LZMA_MEM_ERROR: fprintf(stderr, "ERROR: not enough memory during decompression.\n"); break;
			case LZMA_BUF_ERROR: fprintf(stderr, "ERROR: not enough room in the out buffer during decompression.\n"); break;
			case LZMA_DATA_ERROR: fprintf(stderr, "ERROR: corrupt data during decompression.\n"); break;
			default: fprintf(stderr, "ERROR: an unknown error occured during decompression.\n"); break;
		}
		delete [] buf;
	}
	else {
		fprintf(stderr, "ERROR: no buffer to decompress!\n");
	}
	//printf("Finished decoding\n");
	free (zbuf);
}

SWORD_NAMESPACE_END
