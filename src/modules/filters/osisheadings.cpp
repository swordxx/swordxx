/******************************************************************************
 *
 *osishmlheadings -	SWFilter decendant to hide or show headings
 *			in an OSIS module.
 */


#include <stdlib.h>
#include <string.h>
#include <osisheadings.h>
#include <swmodule.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char OSISHeadings::on[] = "On";
const char OSISHeadings::off[] = "Off";
const char OSISHeadings::optName[] = "Headings";
const char OSISHeadings::optTip[] = "Toggles Headings On and Off if they exist";


OSISHeadings::OSISHeadings() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


OSISHeadings::~OSISHeadings() {
}

void OSISHeadings::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISHeadings::getOptionValue()
{
	return (option) ? on:off;
}

char OSISHeadings::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
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
				if (!stricmp(tag.getAttribute("type"), "section")) {
					if (!stricmp(tag.getAttribute("subtype"), "x-preverse")) {
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

