/******************************************************************************
 *
 * thmlstrongs -	SWFilter decendant to hide or show strongs number
 *			in a ThML module.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thmlstrongs.h>
#include <swmodule.h>
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

char ThMLStrongs::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	char *to, *from, token[2048]; // cheese.  Fix.
	int tokpos = 0;
	bool intoken = false;
	int len;
	bool lastspace = false;
	int word = 1;

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
			token[0] = 0;
			token[1] = 0;
			token[2] = 0;
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;
			if (!strnicmp(token, "sync type=\"Strongs\" ", 20)) {	// Strongs
				char val[128];
				char wordstr[5];
				char *valto = val;
				for (unsigned int i = 27; token[i] != '\"' && i < 150; i++)
					*valto++ = token[i];
				*valto = 0;
				sprintf(wordstr, "%03d", word++);
				module->getEntryAttributes()["Word"][wordstr]["Strongs"] = val;

				if (!option) {	// if we don't want strongs
					if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
						if (lastspace)
							to--;
					}
					continue;
				}
			}
			// if not a strongs token, keep token in text
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
	return 0;
}
