/******************************************************************************
 *
 * thmlmorph -	SWFilter decendant to hide or show morph tags
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlmorph.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char ThMLMorph::on[] = "On";
const char ThMLMorph::off[] = "Off";
const char ThMLMorph::optName[] = "Morphological Tags";
const char ThMLMorph::optTip[] = "Toggles Morphological Tags On and Off if they exist";


ThMLMorph::ThMLMorph() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLMorph::~ThMLMorph() {
}

void ThMLMorph::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLMorph::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLMorph::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
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
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				if (!strnicmp(token, "sync type=\"morph\" ", 18)) {	// Morph
				  if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
				    if (lastspace)
				      to--;
				  }
				  continue;
				}
				// if not a morph tag token, keep token in text
				*to++ = '<';
				for (char *tok = token; *tok; tok++)
					*to++ = *tok;
				*to++ = '>';
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;
			}
			else	{
				*to++ = *from;
				lastspace = (*from == ' ');
			}
		}
		*to++ = 0;
		*to = 0;
	}
	return 0;
}
