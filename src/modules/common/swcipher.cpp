/******************************************************************************
 *  swcipher.cpp   - code for class 'SWCipher'- a driver class that provides
 *				cipher utilities.
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


char *SWCipher::Buf(const char *ibuf, unsigned int ilen)
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


char *SWCipher::cipherBuf(unsigned int *ilen, const char *ibuf)
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
		int i;
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
