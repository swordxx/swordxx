/******************************************************************************
 *  swcrypt.cpp   - code for class 'SWCrypt'- a driver class that provides
 *				cryption utilities.
 */

#include <string.h>
#include <stdlib.h>
#include <swcrypt.h>


/******************************************************************************
 * SWCrypt Constructor - Initializes data for instance of SWCrypt
 *
 */

SWCrypt::SWCrypt(unsigned char *key)
{
	master.initialize(key, strlen((char *)key));
	buf = 0;
}


/******************************************************************************
 * SWCrypt Destructor - Cleans up instance of SWCrypt
 */

SWCrypt::~SWCrypt()
{
	if (buf)
		free(buf);
}


char *SWCrypt::Buf(const char *ibuf)
{
	if (ibuf) {
		buf = (char *) malloc(strlen(ibuf) + 1);
		strcpy(buf, ibuf);
		len = strlen(buf);
		encrypt = false;
	}

	Decode();

	return buf;
}


char *SWCrypt::cryptBuf(unsigned int *ilen, const char *ibuf)
{
	if (ibuf) {
		buf = (char *) malloc(*ilen);
		memcpy(buf, ibuf, *ilen);
		len = *ilen;
		encrypt = true;
	}

	Encode();

	*ilen = (short)len;
	return buf;
}


/******************************************************************************
 * SWCompress::Encode	- This function "encodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void SWCrypt::Encode(void)
{
	if (!encrypt) {
		work = master;
		for (int i = 0; i < len; i++)
			buf[i] = work.encrypt(buf[i]);
		encrypt = true;
	}
}


/******************************************************************************
 * SWCrypt::Decode	- This function "decodes" the input stream into the
 *						output stream.
 *						The GetChars() and SendChars() functions are
 *						used to separate this method from the actual
 *						i/o.
 */

void SWCrypt::Decode(void)
{
	if (encrypt) {
		work = master;
		for (int i = 0; i < len; i++)
			buf[i] = work.decrypt(buf[i]);
		encrypt = false;
	}
}
