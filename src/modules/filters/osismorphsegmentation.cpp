/******************************************************************************
 *
 * osismorphsegmentation - SWFilter descendant to toggle splitting of morphemes
 *	                   (for morpheme segmented Hebrew in the WLC)
 *
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <osismorphsegmentation.h>
#include <stdlib.h>
#include <utilxml.h>
#include <swmodule.h>
#include <swbuf.h>

SWORD_NAMESPACE_START

const char oName[] = "Morpheme Segmentation";
const char oTip[] = "Toggles Morpheme Segmentation On and Off, when present";

const SWBuf choices[3] = {"Off", "On", ""};
const StringList oValues(&choices[0], &choices[2]);

OSISMorphSegmentation::OSISMorphSegmentation() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


OSISMorphSegmentation::~OSISMorphSegmentation() {}


char OSISMorphSegmentation::processText(SWBuf &text, const SWKey * /*key*/, const SWModule *module) {
	SWBuf token;
	bool intoken    = false;
	bool hide       = false;

	SWBuf orig( text );
	const char *from = orig.c_str();

	XMLTag tag;
	SWBuf tagText = "";
	unsigned int morphemeNum = 0;
	bool inMorpheme = false;
	SWBuf buf;

	for (text = ""; *from; ++from) {
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}

		if (*from == '>') { // process tokens
			intoken = false;

			if (!strncmp(token.c_str(), "seg ", 4) || !strncmp(token.c_str(), "/seg", 4)) {
				tag = token;

				if (!tag.isEndTag() && tag.getAttribute("type") && !strcmp("morph", tag.getAttribute("type"))) {  //<seg type="morph"> start tag
					hide = !option; //only hide if option is Off
					tagText = "";
					inMorpheme = true;
				}

				if (tag.isEndTag()) {
						buf.setFormatted("%.3d", morphemeNum++);
						module->getEntryAttributes()["Morpheme"][buf]["body"] = tagText;
						inMorpheme = false;
				}
				if (hide) { //hides start and end tags as long as hide is set

					if (tag.isEndTag()) { //</seg>
						hide = false;
					}

					continue; //leave out the current token
				}
			} //end of seg tag handling

			text.append('<');
			text.append(token);
			text.append('>');

			if (inMorpheme) {
				tagText.append('<');
				tagText.append(token);
				tagText.append('>');
			}

			hide = false;

			continue;
		} //end of intoken part

		if (intoken) { //copy token
			token.append(*from);
		}
		else { //copy text which is not inside of a tag
			text.append(*from);
			if (inMorpheme) {
				tagText.append(*from);
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
