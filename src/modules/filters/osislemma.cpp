/******************************************************************************
 *
 * osislemma          -	SWFilter descendant to hide or show lemmata
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osislemma.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Lemmas";
const char oTip[] = "Toggles Lemmas On and Off if they exist";

const SWBuf choices[3] = {"On", "Off", ""};
const StringList oValues(&choices[0], &choices[2]);

OSISLemma::OSISLemma() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


OSISLemma::~OSISLemma() {
}


char OSISLemma::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (!option) {	// if we don't want lemmas
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
								
				/*XMLTag tag(token);
				if ((!strcmp(tag.getName(), "w")) && (!tag.isEndTag())) {	// Lemma
					SWBuf lemma = tag.getAttribute("lemma");
					if (lemma.length()) {
						tag.setAttribute("lemma", 0);
					 	tag.setAttribute("savlm", lemma.c_str());
					}
				}
				*/
				
				if (*token == 'w') {
					const int len = strlen(token);
					
					const char* start = strstr(token, "lemma=\"");
					const char* end = start ? strchr(start+6, '"') : 0;
					if (start && end) { //we wan to leave out the moprh attribute
						text.append(token, start-token); //the text before the morph attr
						text.append(end, len-(end - token)); //text after the morph attr
					}
				}
				else {
					text.append(token);
				}
				
				// keep tag, possibly with the lemma removed
				//text += tag;
				
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
