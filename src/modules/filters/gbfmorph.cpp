/******************************************************************************
 *
 * gbfmorph -	SWFilter decendant to hide or show morph tags
 *			in a GBF module.
 */


#include <stdlib.h>
#include <string.h>
#include <gbfmorph.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char GBFMorph::on[] = "On";
const char GBFMorph::off[] = "Off";
const char GBFMorph::optName[] = "Morphological Tags";
const char GBFMorph::optTip[] = "Toggles Morphological Tags On and Off In Bible Texts If They Exist";


GBFMorph::GBFMorph() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


GBFMorph::~GBFMorph() {
}

void GBFMorph::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *GBFMorph::getOptionValue()
{
	return (option) ? on:off;
}

char GBFMorph::ProcessText(char *text, int maxlen, const SWKey *key)
{
	if (!option) {	// if we don't want morph tags
		char *to, *from, token[2048]; // cheese.  Fix.
		int tokpos = 0;
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
				memset(token, 0, 2048);
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				if (*token == 'W') {	// Morph
					switch(token[1]) {
					case 'T':               // Tense
						if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
		    					if (lastspace)
								to--;
						}
						continue;	// skip token
					}
				}
				// if not a morph tag token, keep token in text
				*to++ = '<';
				for (unsigned int i = 0; i < strlen(token); i++)
					*to++ = token[i];
				*to++ = '>';
				continue;
			}
			if (intoken) {
				if (tokpos < 2047)
					token[tokpos++] = *from;
			}
			else	{
				*to++ = *from;
				lastspace = (*from == ' ');
			}
		}
		*to = 0;
	}
	return 0;
}
