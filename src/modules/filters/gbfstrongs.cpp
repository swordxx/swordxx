/******************************************************************************
 *
 * gbfstrongs -	SWFilter decendant to hide or show strongs number
 *			in a GBF module.
 */


#include <stdlib.h>
#include <string.h>
#include <gbfstrongs.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char GBFStrongs::on[] = "On";
const char GBFStrongs::off[] = "Off";
const char GBFStrongs::optName[] = "Strong's Numbers";
const char GBFStrongs::optTip[] = "Toggles Strong's Numbers On and Off In Bible Texts If They Exist";


GBFStrongs::GBFStrongs() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


GBFStrongs::~GBFStrongs() {
}

void GBFStrongs::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *GBFStrongs::getOptionValue()
{
	return (option) ? on:off;
}

char GBFStrongs::ProcessText(char *text, int maxlen)
{
	if (!option) {	// if we don't want strongs
		char *to, *from, token[2048]; // cheese.  Fix.
		char tokpos = 0;
		bool intoken = false;
		int len;
		bool lastspace = false;

		len = strlen(text) + 1;	// shift string to right of buffer
		if (len < maxlen) {
			memmove(&text[maxlen - len], text, len);
			from = &text[maxlen - len];
		}
		else	from = text;	// -------------------------------

		for (to = text; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				memset(token, 0, 20);
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				if (*token == 'W') {	// Strongs
					switch(token[1]) {
					case 'T':               // Tense
					case 'G':               // Greek
					case 'H':               // Hebrew
						if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
		    					if (lastspace)
								to--;
						}
						continue;	// skip token
					}
				}
				// if not a strongs token, keep token in text
				*to++ = '<';
				for (unsigned int i = 0; i < strlen(token); i++)
					*to++ = token[i];
				*to++ = '>';
				continue;
			}
			if (intoken)
				token[tokpos++] = *from;
			else	{
				*to++ = *from;
				lastspace = (*from == ' ');
			}
		}
		*to = 0;
	}
	return 0;
}
