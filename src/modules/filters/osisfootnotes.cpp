/******************************************************************************
 *
 *osishmlheadings -	SWFilter decendant to hide or show headings
 *			in an OSIS module.
 */


#include <stdlib.h>
#include <string.h>
#include <osisfootnotes.h>
#include <swmodule.h>
#include <swbuf.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char OSISFootnotes::on[] = "On";
const char OSISFootnotes::off[] = "Off";
const char OSISFootnotes::optName[] = "Footnotes";
const char OSISFootnotes::optTip[] = "Toggles Footnotes On and Off if they exist";


OSISFootnotes::OSISFootnotes() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


OSISFootnotes::~OSISFootnotes() {
}

void OSISFootnotes::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISFootnotes::getOptionValue()
{
	return (option) ? on:off;
}

char OSISFootnotes::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
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
			else if (*from == '>') { // process tokens
				intoken = false;
				if (!strncmp(token.c_str(), "note", 4)) {
				  hide = true;
                                  continue;
				}
				else if (!strncmp(token.c_str(), "/note", 5)) {
				  hide = false;
                                  continue;
				}

				// if not a footnote token, keep token in text
				if (!hide) {
					text += '<';
					text += token;
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

