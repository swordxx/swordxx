/******************************************************************************
 *
 * thmlscripref -	SWFilter decendant to hide or show scripture references
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlscripref.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char ThMLScripref::on[] = "On";
const char ThMLScripref::off[] = "Off";
const char ThMLScripref::optName[] = "Scripture Cross-references";
const char ThMLScripref::optTip[] = "Toggles Scripture Cross-references On and Off if they exist";


ThMLScripref::ThMLScripref() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLScripref::~ThMLScripref() {
}

void ThMLScripref::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLScripref::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLScripref::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want scriprefs
		char *to, *from, token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		int len;
		bool hide = false;

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
				if (!strnicmp(token, "scripRef", 8)) {
				  hide = true;
                                  continue;
				}
				else if (!strnicmp(token, "/scripRef", 9)) {
				  hide = false;
                                  continue;
				}

				// if not a scripref token, keep token in text
				if (!hide) {
					*to++ = '<';
					for (unsigned int i = 0; i < strlen(token); i++)
						*to++ = token[i];
					*to++ = '>';
				}
				continue;
			}
			if (intoken) {
				if (tokpos < 2047)
					token[tokpos++] = *from;
			}
			else	{
				if (!hide) {
					*to++ = *from;
				}
			}
		}
		*to++ = 0;
		*to = 0;
	}
	return 0;
}
