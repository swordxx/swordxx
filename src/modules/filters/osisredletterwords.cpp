/******************************************************************************
 *
 * OSISRedLetterWords -	SWFilter descendant to toggle red coloring for words
 *			of Christ in an OSIS module.
 */


#include <stdlib.h>
#include <osisredletterwords.h>
#include <swmodule.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Words of Christ in Red";
const char oTip[] = "Toggles Red Coloring for Words of Christ On and Off if they are marked";

const SWBuf choices[3] = {"On", "Off", ""};
const StringList oValues(&choices[0], &choices[2]);

OSISRedLetterWords::OSISRedLetterWords() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("On");
}


OSISRedLetterWords::~OSISRedLetterWords() {
}


char OSISRedLetterWords::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	SWBuf token;
	bool intoken    = false;
	char buf[254];

	SWBuf orig = text;
	const char *from = orig.c_str();

	//taken out of the loop
	char* start;
	char* end;
	
	if (!option)
	for (text = ""; *from; from++) {
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;

			//XMLTag tag(token);
			/*if (!stricmp(tag.getName(), "q")) {
				if ((tag.getAttribute("who")) && (!stricmp(tag.getAttribute("who"), "Jesus"))) {
					tag.setAttribute("who", 0);
					text += tag;	// tag toString already has < and >
					continue;
				}
			}*/
			if (*token == 'q') {
				start = strstr(token, "who=\"Jesus\"");
				if (start && (strlen(start) > 11)) {
					end = start+11;
					text.append('<');
					text.append(token, start-token); //the text before the who attr
					text.append(end, strlen(token)-(end - token)); //text after the who attr
					text.append('>');
				}
			}
			else {
				text.append('<');
				text.append(token);
				text.append('>');
			}
			
			
			// if we haven't modified, still use original token instead of tag, so we don't reorder attribs and stuff.  It doesn't really matter, but this is less intrusive to the original markup.
			text.append('<');
			text.append(token);
			text.append('>');
			continue;
		}
		if (intoken) { //copy token
			token.append(*from);
		}
		else { //copy text which is not inside a token
			text.append(*from);
		}
	}
	return 0;
}

SWORD_NAMESPACE_END

