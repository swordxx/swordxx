/******************************************************************************
 *
 * gbffootnotes -	SWFilter descendant to hide or show footnotes
 *			in a GBF module.
 */


#include <stdlib.h>
#include <gbffootnotes.h>
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


GBFFootnotes::GBFFootnotes() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


GBFFootnotes::~GBFFootnotes() {
}


char GBFFootnotes::processText (SWBuf &text, const SWKey *key, const SWModule *module)
{
	
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
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;

			XMLTag tag(token);
			if (!strcmp(tag.getName(), "RF")) {
				refs = "";
				startTag = tag;
				hide = true;
				tagText = "";
				continue;
			}
			else if (!strcmp(tag.getName(), "Rf")) {
				if (module->isProcessEntryAttributes()) {
					if(tagText.length() == 1 || !strcmp(module->Name(), "IGNT")) {
						if (option) { // for ASV marks text in verse then put explanation at end of verse
							text += " <FA>(";
							text.append(tagText);
							text += ")<Fr>";
							hide = false;
							continue;
						}
					}
					SWBuf fc = module->getEntryAttributes()["Footnote"]["count"]["value"];
					footnoteNum = (fc.length()) ? atoi(fc.c_str()) : 0;
					sprintf(buf, "%i", ++footnoteNum);
					module->getEntryAttributes()["Footnote"]["count"]["value"] = buf;
					StringList attributes = startTag.getAttributeNames();
					for (StringList::iterator it = attributes.begin(); it != attributes.end(); it++) {
						module->getEntryAttributes()["Footnote"][buf][it->c_str()] = startTag.getAttribute(it->c_str());
					}
					module->getEntryAttributes()["Footnote"][buf]["body"] = tagText;
					startTag.setAttribute("swordFootnote", buf);
				}
				hide = false;
				if (option) {	
					text += startTag;
					text.append(tagText);
				}
				else	continue;
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
	
	
	
	
	
	
	
	
	/*
	if (!option) {	// if we don't want footnotes
		char token[4096]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		int len;
		bool hide = false;

		const char *from;
		SWBuf orig = text;
		from = orig.c_str();
		for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
//				memset(token, 0, 4096);
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				switch (*token) {
				case 'R':				// Reference
					switch(token[1]) {
					case 'F':               // Begin footnote
						hide = true;
						break;
					case 'f':               // end footnote
						hide = false;
						break;
					}
					continue;	// skip token
				case 'W':
					if (token[1] == 'T') {
						switch (token[2]) {
						case 'P':
						case 'S':
						case 'A':
							continue; // remove this token
						default:
							break;
						}
					}
				}
				// if not a footnote token, keep token in text
				if (!hide) {
					text += '<';
					text += token;
					text += '>';
				}
				continue;
			}
			if (intoken) {
				if (tokpos < 4090)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;	// +2 cuz we init token with 2 extra '0' because of switch statement
			}
			else	{
				if (!hide) {
					text += *from;
				}
			}
		}
	}
	return 0;*/
}

SWORD_NAMESPACE_END
