/******************************************************************************
 *
 * greeklexattribs -	SWFilter decendant to set entry attributes for greek
 * 	lexicons
 */


#include <stdlib.h>
#include <string.h>
#include <greeklexattribs.h>


GreekLexAttribs::GreekLexAttribs() {
}


char GreekLexAttribs::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module) {
	char *to, *from;
	int len;

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;							// -------------------------------

	for (to = text; *from; from++) {
		*to++ = *from;
	}
	*to++ = 0;
	*to = 0;

	return 0;
}


