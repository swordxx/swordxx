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


char CipherFilter::ProcessText(char *text, int maxlen) {
	unsigned int len;
//	len = strlen(text);
	len = maxlen - 1;
	cipher->cipherBuf(&len, text);
	strncpy(text, cipher->Buf(), (len < (unsigned int)maxlen) ? len : maxlen);
	text[maxlen] = 0;
	return 0;
}
