/******************************************************************************
 *
 * thmlheadings -	SWFilter decendant to hide or show headings
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlheadings.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char ThMLHeadings::on[] = "On";
const char ThMLHeadings::off[] = "Off";
const char ThMLHeadings::optName[] = "Headings";
const char ThMLHeadings::optTip[] = "Toggles Headings On and Off if they exist";


ThMLHeadings::ThMLHeadings() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLHeadings::~ThMLHeadings() {
}

void ThMLHeadings::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLHeadings::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLHeadings::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want headings
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
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				if (!strnicmp(token, "div class=\"sechead\"", 19)) {
						hide = true;
						continue;
				}
				if (!strnicmp(token, "div class=\"title\"", 17)) {
						hide = true;
						continue;
				}
				else if (hide && !strnicmp(token, "/div", 4)) {
               				        hide = false;
                                                continue;
				}

				// if not a heading token, keep token in text
				if (!hide) {
					*to++ = '<';
					for (char *tok = token; *tok; tok++)
						*to++ = *tok;
					*to++ = '>';
				}
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;
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
