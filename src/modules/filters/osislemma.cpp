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
		char token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		bool lastspace = false;
		SWBuf orig = text;
		const char *from = orig.c_str();

		int len = 0; //taken out of the loop for speed
		const char* start = 0;
		const char* end = 0;
		
		
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
				
				if ((*token == 'w') && (token[1] == ' ')) {
// 					len = strlen(token);
					
					start = strstr(token, "lemma=\""); //we leave out the "w " at the start
					end = (start && (strlen(start)>7)) ? strchr(start+7, '"') : 0;
					if (start && end) { //we want to leave out the morph attribute
						text.append('<');
						text.append(token, start-token); //the text before the morph attr
						text.append(end+1, strlen(token) - (end+1 - token)); //text after the morph attr
						text.append('>');
					}
				}
				else {
					text.append('<');
					text.append(token);
					text.append('>');
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
