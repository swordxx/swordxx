/******************************************************************************
 *
 * rwprtf -	SWFilter decendant to convert all GBF tags to RTF tags
 */


#include <stdlib.h>
#include <string.h>
#include <rwprtf.h>


RWPRTF::RWPRTF() {

}


char RWPRTF::ProcessText(char *text, int maxlen, const SWKey *key)
{
	char *to, *from;
	bool ingreek = false;
	bool inverse = false;
	int len;

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;							// -------------------------------
	for (to = text; *from; from++) {
		if (*from == '\\') {
			if(!ingreek) {
				ingreek = true;
				*to++ = '[';
				*to++ = '{';
				*to++ = '\\';
				*to++ = 'f';
				*to++ = '8';
				*to++ = ' ';
				continue;
			}
			else {
				ingreek = false;
				*to++ = '}';
				*to++ = ']';
				continue;
			}
		}

		if ((ingreek) && ((*from == 'h') || (*from == 'H')))
			continue;		// 'h's are mostly useless in RWP translitterations.  The greek is more correct without them.

		if (*from == '#') {	// verse markings (e.g. "#Mark 1:1|")
			inverse = true;
			*to++ = '{';
			*to++ = '\\';
			*to++ = 'c';
			*to++ = 'f';
			*to++ = '2';
			*to++ = ' ';
			*to++ = '#';
			continue;
		}
		if ((*from == '|') && (inverse)) {
			inverse = false;
			*to++ = '|';
			*to++ = '}';
			continue;
		}
		
		if (*from == '{') {
			*to++ = '{';
			*to++ = '\\';
			*to++ = 'b';
			*to++ = ' ';
			if ((from - &text[maxlen - len]) > 10) {	// not the beginning of the entry
				*to++ = '\\';
				*to++ = 'p';
				*to++ = 'a';
				*to++ = 'r';
				*to++ = ' ';
			}
			continue;
		}

		if (*from == '}') {
			// this is kinda neat... DO NOTHING
		}
		if ((*from == '\n') && (from[1] == '\n')) {
			*to++ = '\\';
			*to++ = 'p';
			*to++ = 'a';
			*to++ = 'r';
			*to++ = '\\';
			*to++ = 'p';
			*to++ = 'a';
			*to++ = 'r';
			*to++ = ' ';
			continue;
		}

		*to++ = *from;
	}
	*to = 0;
	return 0;
}
