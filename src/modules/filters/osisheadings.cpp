/******************************************************************************
 *
 *osisheadings -	SWFilter descendant to hide or show headings
 *			in an OSIS module.
 */


#include <stdlib.h>
#include <osisheadings.h>
#include <swmodule.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Headings";
const char oTip[] = "Toggles Headings On and Off if they exist";

const SWBuf choices[3] = {"On", "Off", ""};
const StringList oValues(&choices[0], &choices[2]);

OSISHeadings::OSISHeadings() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


OSISHeadings::~OSISHeadings() {
}


char OSISHeadings::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	SWBuf token;
	bool intoken    = false;
	bool hide       = false;
	bool preverse   = false;
	SWBuf header;
	int headerNum   = 0;
	int pvHeaderNum = 0;
	char buf[254];

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

			XMLTag tag(token);
			if (!stricmp(tag.getName(), "title")) {
				if ((tag.getAttribute("type")) && (!stricmp(tag.getAttribute("type"), "section"))) {
					if ((tag.getAttribute("subtype")) && (!stricmp(tag.getAttribute("subtype"), "x-preverse"))) {
						hide = true;
						preverse = true;
						header = "";
						continue;
					}
					else {
						hide = true;
						header = "";
						if (option) {	// we want the tag in the text
							text += '<';
							text.append(token);
							text += '>';
						}
						continue;
					}
				}
				if (hide && tag.isEndTag()) {

					if (module->isProcessEntryAttributes() && option) {
						if (preverse) {
							sprintf(buf, "%i", pvHeaderNum++);
							module->getEntryAttributes()["Heading"]["Preverse"][buf] = header;
						}
						else {
							sprintf(buf, "%i", headerNum++);
							module->getEntryAttributes()["Heading"]["Interverse"][buf] = header;
							if (option) {	// we want the tag in the text
								text.append(header);
							}
						}
					}
					hide = false;
					if ((!option) || (preverse)) {	// we don't want the tag in the text anymore
						preverse = false;
						continue;
					}
					preverse = false;
				}
			}

			// if not a heading token, keep token in text
			if (!hide) {
				text += '<';
				text.append(token);
				text += '>';
			}
			continue;
		}
		if (intoken) { //copy token
			token += *from;
		}
		else if (!hide) { //copy text which is not inside a token
			text += *from;
		}
		else header += *from;
	}
	return 0;
}

SWORD_NAMESPACE_END

