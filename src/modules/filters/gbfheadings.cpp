/******************************************************************************
 *
 * gbfheadings -	SWFilter decendant to hide or show headings
 *			in a GBF module.
 */


#include <stdlib.h>
#include <string.h>
#include <gbfheadings.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char GBFHeadings::on[] = "On";
const char GBFHeadings::off[] = "Off";
const char GBFHeadings::optName[] = "Headings";
const char GBFHeadings::optTip[] = "Toggles Headings On and Off In Bible Texts If They Exist";


GBFHeadings::GBFHeadings() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


GBFHeadings::~GBFHeadings() {
}

void GBFHeadings::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *GBFHeadings::getOptionValue()
{
	return (option) ? on:off;
}

char GBFHeadings::ProcessText(char *text, int maxlen, const SWKey *key)
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
				memset(token, 0, 2048);
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				switch (*token) {
				case 'T':				// Reference
					switch(token[1]) {
					case 'S':               // Begin heading
						hide = true;
						break;
					case 's':               // end heading
						hide = false;
						break;
					}
					continue;	// skip token
				}
				// if not a heading token, keep token in text
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
		*to = 0;
	}
	return 0;
}
