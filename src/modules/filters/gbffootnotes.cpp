/******************************************************************************
 *
 * gbffootnotes -	SWFilter decendant to hide or show footnotes
 *			in a GBF module.
 */


#include <stdlib.h>
#include <string.h>
#include <gbffootnotes.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char GBFFootnotes::on[] = "On";
const char GBFFootnotes::off[] = "Off";
const char GBFFootnotes::optName[] = "Footnotes";
const char GBFFootnotes::optTip[] = "Toggles Footnotes On and Off if they exist";


GBFFootnotes::GBFFootnotes() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


GBFFootnotes::~GBFFootnotes() {
}

void GBFFootnotes::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *GBFFootnotes::getOptionValue()
{
	return (option) ? on:off;
}

char GBFFootnotes::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want footnotes
		char *to, *from, token[4096]; // cheese.  Fix.
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
//				memset(token, 0, 4096);
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				switch (*token) {
				case 'R':				// Reference
					switch(token[1]) {
					case 'F':               // Begin footnote
						hide = true;
						break;
					case 'f':               // end footnote
						hide = false;
						break;
					}
					continue;	// skip token
				case 'W':
					if (token[1] == 'T') {
						switch (token[2]) {
						case 'P':
						case 'S':
						case 'A':
							continue; // remove this token
						default:
							break;
						}
					}
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
				if (tokpos < 4090)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;	// +2 cuz we init token with 2 extra '0' because of switch statement
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
