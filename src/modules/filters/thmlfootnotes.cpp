/******************************************************************************
 *
 * thmlfootnotes -	SWFilter descendant to hide or show footnotes
 *			in a ThML module.
 */


#include <stdlib.h>
#include <thmlfootnotes.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Footnotes";
const char oTip[] = "Toggles Footnotes On and Off if they exist";

const SWBuf choices[2] = {"On", "Off"};
const StringList oValues(&choices[0], &choices[1]);

ThMLFootnotes::ThMLFootnotes() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


ThMLFootnotes::~ThMLFootnotes() {
}


char ThMLFootnotes::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
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
