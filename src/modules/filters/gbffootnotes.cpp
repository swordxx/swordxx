/******************************************************************************
 *
 * gbffootnotes -	SWFilter descendant to hide or show footnotes
 *			in a GBF module.
 */


#include <stdlib.h>
#include <gbffootnotes.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Footnotes";
const char oTip[] = "Toggles Footnotes On and Off if they exist";

const SWBuf choices[2] = {"On", "Off"};
const StringList oValues(&choices[0], &choices[1]);


GBFFootnotes::GBFFootnotes() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


GBFFootnotes::~GBFFootnotes() {
}


char GBFFootnotes::processText (SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want footnotes
		char token[4096]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		int len;
		bool hide = false;

	const char *from;
	SWBuf orig = text;
	from = orig.c_str();
	for (text = ""; *from; from++) {
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
					text += '<';
					text += token;
					text += '>';
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
					text += *from;
				}
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
