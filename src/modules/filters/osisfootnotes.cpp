/******************************************************************************
 *
 * osisfootnotes -	SWFilter descendant to hide or show footnotes
 *			in an OSIS module.
 */


#include <stdlib.h>
#include <osisfootnotes.h>
#include <swmodule.h>
#include <swbuf.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char OSISFootnotes::on[] = "On";
const char OSISFootnotes::off[] = "Off";
const char OSISFootnotes::optName[] = "Footnotes";
const char OSISFootnotes::optTip[] = "Toggles Footnotes On and Off if they exist";


OSISFootnotes::OSISFootnotes() {
	option = true;
	options.push_back(on);
	options.push_back(off);
}


OSISFootnotes::~OSISFootnotes() {
}

void OSISFootnotes::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISFootnotes::getOptionValue()
{
	return (option) ? on:off;
}

char OSISFootnotes::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
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
			if (!strcmp(tag.getName(), "note") && strcmp(tag.getAttribute("type"), "crossReference")) {
				if (!tag.isEndTag() && (!tag.isEmpty())) {
					startTag = tag;
					hide = true;
					tagText = "";
					if (option) {	// we want the tag in the text
						text += '<';
						text.append(token);
						text += '>';
					}
					continue;
				}
				if (hide && tag.isEndTag()) {
					if (module->isProcessEntryAttributes() && option) {
						sprintf(buf, "%i", footnoteNum++);
						ListString attributes = startTag.getAttributeNames();
						for (ListString::iterator it = attributes.begin(); it != attributes.end(); it++) {
							module->getEntryAttributes()["Footnote"][buf][it->c_str()] = startTag.getAttribute(it->c_str());
						}
						module->getEntryAttributes()["Footnote"][buf]["body"] = tagText;
						if (option) {	// we want the tag in the text
							text.append(tagText);
						}
					}
					hide = false;
					if (!option) {	// we don't want the tag in the text anymore
						continue;
					}
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
		else tagText += *from;
	}
	return 0;
}

SWORD_NAMESPACE_END

