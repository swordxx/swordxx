/******************************************************************************
 *
 * cipherfil -	SWFilter decendant to decipher a module
 */


#include <stdlib.h>
#include <string.h>
#include <cipherfil.h>

SWORD_NAMESPACE_START

CipherFilter::CipherFilter(const char *key) {
	cipher = new SWCipher((unsigned char *)key);
}


CipherFilter::~CipherFilter() {
	delete cipher;
}


SWCipher *CipherFilter::getCipher() {
	return cipher;
}


char CipherFilter::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	unsigned int len;
//	len = strlen(text);
	len = text.length();
	if (len > 0) {
		if (!key) {	// hack, using key to determine encipher, or decipher
			cipher->cipherBuf(&len, text);
			//memcpy(text, cipher->Buf(), (len < (unsigned int)(maxlen)) ? len : maxlen);
			text = cipher->Buf();
		}
		else if ((unsigned long)key == 1) {
			cipher->Buf(text, len);
			text = cipher->cipherBuf(&len);
			//memcpy(text, cipher->cipherBuf(&len), (len < (unsigned int)(maxlen)) ? len : maxlen);
		}
	}
//	text[maxlen] = 0;
	return 0;
}

SWORD_NAMESPACE_END
