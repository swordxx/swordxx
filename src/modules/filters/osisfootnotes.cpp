/******************************************************************************
 *
 * osisfootnotes -	SWFilter descendant to hide or show footnotes
 *			in an OSIS module.
 */


#include <stdlib.h>
#include <osisfootnotes.h>
#include <swmodule.h>
#include <swbuf.h>
#include <versekey.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char oName[] = "Footnotes";
const char oTip[] = "Toggles Footnotes On and Off if they exist";

const SWBuf choices[3] = {"On", "Off", ""};
const StringList oValues(&choices[0], &choices[2]);

OSISFootnotes::OSISFootnotes() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


OSISFootnotes::~OSISFootnotes() {
}


char OSISFootnotes::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	SWBuf token;
	bool intoken    = false;
	bool hide       = false;
	SWBuf tagText;
	XMLTag startTag;
	SWBuf refs = "";
	int footnoteNum = 1;
	char buf[254];
	VerseKey parser = key->getText();

	SWBuf orig = text;
	const char *from = orig.c_str();

	for (text = ""; *from; from++) {

		// remove all newlines temporarily to fix kjv2003 module
		if ((*from == 10) || (*from == 13)) {
			if ((text.length()>1) && (text[text.length()-2] != ' ') && (*(from+1) != ' '))
				text += ' ';
			continue;
		}

		
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;

			XMLTag tag(token);
			if (!strcmp(tag.getName(), "note")) {
				if (!tag.isEndTag()) {
					if ((!tag.isEmpty()) || (SWBuf("strongsMarkup") == tag.getAttribute("type"))) {
						refs = "";
						startTag = tag;
						hide = true;
						tagText = "";
						continue;
					}
				}
				if (hide && tag.isEndTag()) {
					if (module->isProcessEntryAttributes()) {
						sprintf(buf, "%i", footnoteNum++);
						StringList attributes = startTag.getAttributeNames();
						for (StringList::iterator it = attributes.begin(); it != attributes.end(); it++) {
							module->getEntryAttributes()["Footnote"][buf][it->c_str()] = startTag.getAttribute(it->c_str());
						}
						module->getEntryAttributes()["Footnote"][buf]["body"] = tagText;
						startTag.setAttribute("swordFootnote", buf);
						if ((startTag.getAttribute("type")) && (!strcmp(startTag.getAttribute("type"), "crossReference"))) {
							if (!refs.length())
								refs = parser.ParseVerseList(tagText.c_str(), parser, true).getRangeText();
							module->getEntryAttributes()["Footnote"][buf]["refList"] = refs.c_str();
						}
					}
					hide = false;
					if ((option) || ((startTag.getAttribute("type") && (!strcmp(startTag.getAttribute("type"), "crossReference"))))) {	// we want the tag in the text; crossReferences are handled by another filter
						text += startTag;
						text.append(tagText);
					}
					else	continue;
				}
			}

			// if not a heading token, keep token in text
			if ((!strcmp(tag.getName(), "reference")) && (!tag.isEndTag())) {
				SWBuf osisRef = tag.getAttribute("osisRef");
				if (refs.length())
					refs += "; ";
				refs += osisRef;
			}
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

