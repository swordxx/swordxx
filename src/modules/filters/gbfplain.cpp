/******************************************************************************
 *
 * gbfplain -	SWFilter decendant to strip out all GBF tags or convert to
 *		ASCII rendered symbols.
 */


#include <stdlib.h>
#include <string.h>
#include <gbfplain.h>


GBFPlain::GBFPlain() {
}


char GBFPlain::ProcessText(char *text, int maxlen, const SWKey *key)
{
	char *to, *from, token[2048];
	int tokpos = 0;
	bool intoken = false;
	int len;

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;							// -------------------------------
	
	for (to = text; *from; from++) {
		if (*from == '<') {
			intoken = true;
			tokpos = 0;
			memset(token, 0, 2048);
			continue;
		}
		if (*from == '>') {
			intoken = false;
						// process desired tokens
			switch (*token) {
			case 'W':	// Strongs
				switch(token[1]) {
					case 'G':               // Greek
					case 'H':               // Hebrew
						*to++ = ' ';
						*to++ = '<';
						for (unsigned int i = 2; i < strlen(token); i++)
							*to++ = token[i];
						*to++ = '>';
						*to++ = ' ';
						continue;

					case 'T':               // Tense
						*to++ = ' ';
						*to++ = '(';
						for (unsigned int i = 3; i < strlen(token); i++)
							*to++ = token[i];
						*to++ = ')';
						*to++ = ' ';
						continue;
				}
				break;
			case 'R':
				switch(token[1]) {
				case 'F':               // footnote begin
					*to++ = ' ';
					*to++ = '[';
					continue;
				case 'f':               // footnote end
					*to++ = ']';
					*to++ = ' ';
					continue;
				}
				break;
			case 'C':
				switch(token[1]) {
				case 'A':               // ASCII value
					*to++ = (char)atoi(&token[2]);
					continue;
				case 'G':
					*to++ = '>';
					continue;
/*								Bug in WEB
				case 'L':
					*to++ = '<';
					continue;
*/
				case 'L':	//        Bug in WEB.  Use above entry when fixed
				case 'N':               // new line
					*to++ = '\n';
					continue;
				case 'M':               // new paragraph
					*to++ = '\n';
					*to++ = '\n';
					continue;
				}
				break;
			}
			continue;
		}
		if (intoken) {
			if (tokpos < 2047)
				token[tokpos++] = *from;
		}
		else	*to++ = *from;
	}
	*to = 0;
	return 0;
}
