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

char ThMLFootnotes::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want footnotes
		bool intoken = false;
		bool hide = false;

		SWBuf token;
		SWBuf orig = text;
		const char *from = orig.c_str();
		for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				token = "";
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
					text += '<';
					text.append(token);
					text += '>';
				}
				continue;
			}
			if (intoken) {
				token += *from; //copy chars of found token
			}
			else if (!hide) {
        			text += *from;
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
