/******************************************************************************
 *
 * osismorph -	SWFilter descendant to hide or show morph tags
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osismorph.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Morphological Tags";
const char oTip[] = "Toggles Morphological Tags On and Off if they exist";

const SWBuf choices[3] = {"On", "Off", ""};
const StringList oValues(&choices[0], &choices[2]);

OSISMorph::OSISMorph() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


OSISMorph::~OSISMorph() {
}


char OSISMorph::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (!option) {	// if we don't want morph tags
		const char *from;
		char token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		bool lastspace = false;
		SWBuf orig = text;
		from = orig.c_str();

		for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				XMLTag tag(token);
				if ((tag.getName()) && (!strcmp(tag.getName(), "w")) && (!tag.isEndTag())) {	// Morph
					if (tag.getAttribute("morph"))
						tag.setAttribute("morph", 0);
				}
				// keep tag, possibly with the morph removed
				text += tag;
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos] = 0;
			}
			else	{
				text += *from;
				lastspace = (*from == ' ');
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
