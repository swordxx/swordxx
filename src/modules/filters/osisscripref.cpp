/******************************************************************************
 *
 * OSISScripref -	SWFilter descendant to hide or show scripture references
 *			in an OSIS module.
 */


#include <stdlib.h>
#include <osisscripref.h>
#include <swmodule.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Cross-references";
const char oTip[] = "Toggles Scripture Cross-references On and Off if they exist";

const SWBuf choices[3] = {"On", "Off", ""};
const StringList oValues(&choices[0], &choices[2]);

OSISScripref::OSISScripref() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("On");
}


OSISScripref::~OSISScripref() {
}


char OSISScripref::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	SWBuf token;
	bool intoken    = false;
	bool hide       = false;
	SWBuf tagText;
	XMLTag startTag;
	int tagTextNum  = 1;
	int footnoteNum = 1;
	char buf[254];

	SWBuf orig = text;
	const char *from = orig.c_str();
	
	XMLTag tag;

	for (text = ""; *from; from++) {
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;

			
			//if (!strcmp(tag.getName(), "note")) {
			if (!strncmp(token, "note", 4)) {
				tag = token;
				if (!tag.isEndTag() && (!tag.isEmpty())) {
					startTag = tag;
					if (tag.getAttribute("type") && !strcmp(tag.getAttribute("type"), "crossReference")) {
						hide = true;
						tagText = "";
						if (option) {	// we want the tag in the text
							text += '<';
							text.append(token);
							text += '>';
						}
						continue;
					}
				}
				if (hide && tag.isEndTag()) {
					hide = false;
					if (option) {	// we want the tag in the text
						text.append(tagText);  // end tag gets added further down
					}
					else	continue;	// don't let the end tag get added to the text
				}
			}

			// if not a heading token, keep token in text
			if (!hide) {
				text.append('<');
				text.append(token);
				text.append('>');
			}
			else {
				tagText.append('<');
				tagText.append(token);
				tagText.append('>');
			}
			continue;
		}
		if (intoken) { //copy token
			token.append(*from);
		}
		else if (!hide) { //copy text which is not inside a token
			text.append(*from);
		}
		else tagText.append(*from);
	}
	return 0;
}

SWORD_NAMESPACE_END
