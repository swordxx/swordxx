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
	char *from;

	for (from = text; *from; from++) {
		if (!strncmp(from, "AV-", 3)) {
		}
	}
	return 0;
}


