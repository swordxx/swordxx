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

const char OSISRedLetterWords::on[] = "On";
const char OSISRedLetterWords::off[] = "Off";
const char OSISRedLetterWords::optName[] = "Words of Christ in Red";
const char OSISRedLetterWords::optTip[] = "Toggles Red Coloring for Words of Christ On and Off if they are marked";


OSISRedLetterWords::OSISRedLetterWords() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


OSISRedLetterWords::~OSISRedLetterWords() {
}

void OSISRedLetterWords::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISRedLetterWords::getOptionValue()
{
	return (option) ? on:off;
}

char OSISRedLetterWords::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	SWBuf token;
	bool intoken    = false;
	char buf[254];

	SWBuf orig = text;
	const char *from = orig.c_str();

	if (!option)
	for (text = ""; *from; from++) {
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;

			XMLTag tag(token);
			if (!stricmp(tag.getName(), "q")) {
				if ((tag.getAttribute("who")) && (!stricmp(tag.getAttribute("who"), "Jesus"))) {
					tag.setAttribute("who", 0);
					text += token;	// token toString already has < and >
					continue;
				}
			}
			// if we haven't modified, still use original token instead of tag, so we don't reorder attribs and stuff.  It doesn't really matter, but this is less intrusive to the original markup.
			text += '<';
			text.append(token);
			text += '>';
			continue;
		}
		if (intoken) { //copy token
			token += *from;
		}
		else { //copy text which is not inside a token
			text += *from;
		}
	}
	return 0;
}

SWORD_NAMESPACE_END

