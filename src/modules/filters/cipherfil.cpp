/******************************************************************************
 *
 * cipherfil -	SWFilter descendant to decipher a module
 */


#include <stdlib.h>
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
	if (text.length() > 2) { //check if it's large enough to substract 2 in the next step.
		unsigned int len = text.length() - 2;
		if (!key) {	// hack, using key to determine encipher, or decipher
			cipher->cipherBuf(&len, text); //set buffer to enciphered text
			//memcpy(text, cipher->Buf(), (len < (unsigned int)(maxlen)) ? len : maxlen);
			text = cipher->Buf(); //get the deciphered buffer
		}
		else if ((unsigned long)key == 1) {
			cipher->Buf(text, len);
			text = cipher->cipherBuf(&len);
			//memcpy(text, cipher->cipherBuf(&len), (len < (unsigned int)(maxlen)) ? len : maxlen);
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
