/******************************************************************************
 *
 * cipherfil -	SWFilter decendant to decipher a module
 */


#include <stdlib.h>
#include <string.h>
#include <cipherfil.h>


CipherFilter::CipherFilter(const char *key) {
	cipher = new SWCipher((unsigned char *)key);
}


CipherFilter::~CipherFilter() {
	delete cipher;
}


SWCipher *CipherFilter::getCipher() {
	return cipher;
}


char CipherFilter::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module) {
	unsigned int len;
//	len = strlen(text);
	len = maxlen;
	if (len > 0) {
		if (!key) {	// hack, using key to determine encipher, or decipher
			cipher->cipherBuf(&len, text);
			memcpy(text, cipher->Buf(), (len < (unsigned int)(maxlen)) ? len : maxlen);
		}
		else if ((unsigned long)key == 1) {
			cipher->Buf(text, len);
			cipher->cipherBuf(&len);
			memcpy(text, cipher->cipherBuf(&len), (len < (unsigned int)(maxlen)) ? len : maxlen);
		}
	}
//	text[maxlen] = 0;
	return 0;
}
