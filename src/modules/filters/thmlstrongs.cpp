/******************************************************************************
 *
 * thmlstrongs -	SWFilter decendant to hide or show strongs number
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlstrongs.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char ThMLStrongs::on[] = "On";
const char ThMLStrongs::off[] = "Off";
const char ThMLStrongs::optName[] = "Strong's Numbers";
const char ThMLStrongs::optTip[] = "Toggles Strong's Numbers On and Off if they exist";


ThMLStrongs::ThMLStrongs() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLStrongs::~ThMLStrongs() {
}

void ThMLStrongs::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLStrongs::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLStrongs::ProcessText(char *text, int maxlen, const SWKey *key)
{
	if (!option) {	// if we don't want strongs
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
		else	from = text;
		
		// -------------------------------

		for (to = text; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				memset(token, 0, 2048);
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				if (!strnicmp(token, "sync type=\"Strongs\" ", 20)) {	// Strongs
				  if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
				    if (lastspace)
				      to--;
				  }
				  continue;
				}
				// if not a strongs token, keep token in text
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
		*to++ = 0;
		*to = 0;
	}
	return 0;
}
