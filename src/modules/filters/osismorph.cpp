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
		char token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		bool lastspace = false;
		SWBuf orig = text;
		const char *from = orig.c_str();

		for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				
				/*XMLTag tag(token);
				if ((!strcmp(tag.getName(), "w")) && (!tag.isEndTag())) {	// Morph
					if (tag.getAttribute("morph"))
						tag.setAttribute("morph", 0);
				}*/
				
				if (*token == 'w') {
					char* start = strstr(token, "morph=\"");
					const int len = strlen(token);
					if (start && (len>8)) {
						char* end = strchr(start+8, '"');
						if (end) { //remove the morph value
							text.append(token, start-token); //the text before the morph attr
							text.append(end, len-(end - token)); //text after the morph attr
						}
					}
				}
				else {
					text.append(token);
				}
				
				// keep tag, possibly with the morph removed
				//text.append(tag);
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos] = 0;
			}
			else	{
				text.append(*from);
				lastspace = (*from == ' ');
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
