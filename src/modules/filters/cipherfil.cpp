/******************************************************************************
 *
 * cipherfil -	SWFilter descendant to decipher a module
 */


#include <stdlib.h>
#include <cipherfil.h>
#include <swcipher.h>
#include <swbuf.h>

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
		unsigned long len = text.length();
		if (!key) {	// hack, using key to determine encipher, or decipher
			cipher->cipherBuf(&len, text.getRawData()); //set buffer to enciphered text
			memcpy(text.getRawData(), cipher->Buf(), len);
//			text = cipher->Buf(); //get the deciphered buffer
		}
		else if ((unsigned long)key == 1) {
			cipher->Buf(text.getRawData(), len);
			memcpy(text.getRawData(), cipher->cipherBuf(&len), len);
//			text = cipher->cipherBuf(&len);
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
