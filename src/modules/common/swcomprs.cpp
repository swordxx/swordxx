/******************************************************************************
 *  swcomprs.cpp   - code for class 'SWCompress'- a driver class that provides
 *				compression utilities.
 */

#include <string.h>
#include <stdlib.h>
#include <swcomprs.h>


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
		zpos    = 0;
		pos     = 0;
}


char *SWCompress::Buf(char *ibuf)
{
	// setting an uncompressed buffer
	if (ibuf) {
		Init();
		buf = (char *) malloc(strlen(ibuf) + 1);
		strcpy(buf, ibuf);
	}

	// getting an uncompressed buffer
	if (!buf)
		Decode();

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
	if (!zbuf)
		Encode();

	*len = (short)zlen;
	return zbuf;
}


int SWCompress::GetChars(char *ibuf, int len)
{
	int slen;

	if (direct) {
		len = (((zlen - zpos) > len) ? len : zlen - zpos);
		if (len > 0) {
			memmove(ibuf, &zbuf[zpos], len);
			zpos += len;
		}
	}
	else {
		slen = strlen(buf);
		len = (((slen - pos) > len) ? len : slen - pos);
		if (len > 0) {
			memmove(ibuf, &buf[pos], len);
			pos += len;
		}
	}
	return len;
}
	

int SWCompress::SendChars(char *ibuf, int len)
{
	int slen;

	if (direct) {
		if (buf) {
			slen = strlen(buf);
			if ((pos + len) > slen) {
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
	int len, totlen = 0;

	do {
		len = GetChars(buf, 1024);
		if (len)
			totlen += SendChars(buf, len);
	} while (len == 1024);

	zlen = totlen;
}
