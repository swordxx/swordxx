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

const char OSISScripref::on[] = "On";
const char OSISScripref::off[] = "Off";
const char OSISScripref::optName[] = "Scripture Cross-references";
const char OSISScripref::optTip[] = "Toggles Scripture Cross-references On and Off if they exist";

OSISScripref::OSISScripref() {
	option = true;
	options.push_back(on);
	options.push_back(off);
}


OSISScripref::~OSISScripref() {
}

void OSISScripref::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISScripref::getOptionValue()
{
	return (option) ? on:off;
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

	for (text = ""; *from; from++) {
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;

			XMLTag tag(token);
			if (!strcmp(tag.getName(), "note")) {
				if (!tag.isEndTag() && (!tag.isEmpty())) {
					startTag = tag;
					if ((tag.getAttribute("type")) &&	(!strcmp(tag.getAttribute("type"), "crossReference"))) {
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
				text += '<';
				text.append(token);
				text += '>';
			}
			else {
				tagText += '<';
				tagText.append(token);
				tagText += '>';
			}
			continue;
		}
		if (intoken) { //copy token
			token += *from;
		}
		else if (!hide) { //copy text which is not inside a token
			text += *from;
		}
		else tagText += *from;
	}
	return 0;
}

SWORD_NAMESPACE_END
