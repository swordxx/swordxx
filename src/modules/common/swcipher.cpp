/******************************************************************************
 *  swcipher.cpp   - code for class 'SWCipher'- a driver class that provides
 *				cipher utilities.
 */

#include <string.h>
#include <stdlib.h>
#include <swcipher.h>


/******************************************************************************
 * SWCipher Constructor - Initializes data for instance of SWCipher
 *
 */

SWCipher::SWCipher(unsigned char *key)
{
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


char *SWCipher::Buf(const char *ibuf)
{
	if (ibuf) {
	
		if (buf)
			free(buf);
			
		buf = (char *) malloc(strlen(ibuf) + 1);
		strcpy(buf, ibuf);
		len = strlen(buf);
		cipher = false;
	}

	Decode();

	return buf;
}


char *SWCipher::cipherBuf(unsigned int *ilen, const char *ibuf)
{
	if (ibuf) {
	
		if (buf)
			free(buf);
			
		buf = (char *) malloc(*ilen);
		memcpy(buf, ibuf, *ilen);
		len = *ilen;
		cipher = true;
	}

	Encode();

	*ilen = (short)len;
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
		for (int i = 0; i < len; i++)
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
		for (int i = 0; i < len; i++)
			buf[i] = work.decrypt(buf[i]);
		cipher = false;
	}
}
