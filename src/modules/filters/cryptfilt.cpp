/******************************************************************************
 *
 * cryptfilt -	SWFilter decendant to strip out all GBF tags or convert to
 *		ASCII rendered symbols.
 */


#include <stdlib.h>
#include <string.h>
#include <cryptfilt.h>


CryptFilter::CryptFilter(const char *key) {
	crypt = new SWCrypt((unsigned char *)key);
}


CryptFilter::~CryptFilter() {
	delete crypt;
}


char CryptFilter::ProcessText(char *text, int maxlen) {
	unsigned int len;
//	len = strlen(text);
	len = maxlen - 1;
	crypt->cryptBuf(&len, text);
	strncpy(text, crypt->Buf(), (len < (unsigned int)maxlen) ? len : maxlen);
	text[maxlen] = 0;
	return 0;
}
