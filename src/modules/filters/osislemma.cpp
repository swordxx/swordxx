/******************************************************************************
 *
 * osislemma          -	SWFilter descendant to hide or show lemmata
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osislemma.h>

SWORD_NAMESPACE_START

const char oName[] = "Lemmas";
const char oTip[] = "Toggles Lemmas On and Off if they exist";

const SWBuf choices[3] = {"Off", "On", ""};
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

		//taken out of the loop for speed
		const char* start = 0;
		const char* end = 0;
		
		for (text = ""; *from; ++from) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				
				if ((*token == 'w') && (token[1] == ' ')) {
					start = strstr(token, "lemma=\""); //we leave out the "w " at the start
					end = start ? strchr(start+7, '"') : 0;
					
					if (start && end) { //we want to leave out the morph attribute
						text.append('<');
						text.append(token, start-token); //the text before the morph attr
						text.append(end+1);
						text.append('>');
						
						continue;
					}
				}
				
				text.append('<');
				text.append(token);
				text.append('>');
				
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
