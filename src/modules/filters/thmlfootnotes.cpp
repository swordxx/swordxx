/******************************************************************************
 *
 * thmlfootnotes -	SWFilter decendant to hide or show footnotes
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlfootnotes.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char ThMLFootnotes::on[] = "On";
const char ThMLFootnotes::off[] = "Off";
const char ThMLFootnotes::optName[] = "Footnotes";
const char ThMLFootnotes::optTip[] = "Toggles Footnotes On and Off if they exist";


ThMLFootnotes::ThMLFootnotes() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLFootnotes::~ThMLFootnotes() {
}

void ThMLFootnotes::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLFootnotes::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLFootnotes::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want footnotes
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
				if (!strncmp(token, "note", 4)) {
				  hide = true;
                                  continue;
				}
				else if (!strncmp(token, "/note", 5)) {
				  hide = false;
                                  continue;
				}

				// if not a footnote token, keep token in text
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

SWORD_NAMESPACE_END
