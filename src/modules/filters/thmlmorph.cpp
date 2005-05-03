/******************************************************************************
 *
 * thmlmorph -	SWFilter descendant to hide or show morph tags
 *			in a ThML module.
 */


#include <stdlib.h>
#include <thmlmorph.h>

SWORD_NAMESPACE_START

const char oName[] = "Morphological Tags";
const char oTip[] = "Toggles Morphological Tags On and Off if they exist";

const SWBuf choices[3] = {"Off", "On", ""};
const StringList oValues(&choices[0], &choices[2]);

ThMLMorph::ThMLMorph() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


ThMLMorph::~ThMLMorph() {
}


char ThMLMorph::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (!option) {	// if we don't want morph tags
		bool intoken = false;

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
				if (!strncmp(token.c_str(), "sync ", 5) && strstr(token.c_str(), "type=\"morph\"")) {	// Morph
				  continue;
				}

				// if not a morph tag token, keep token in text
				text += '<';
				text += token;
				text += '>';
				continue;
			}

			if (intoken) {
				token += *from;
			}
			else	{
				text += *from;
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
