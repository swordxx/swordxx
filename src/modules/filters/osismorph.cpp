/******************************************************************************
 *
 * osismorph -	SWFilter decendant to hide or show morph tags
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <string.h>
#include <osismorph.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char OSISMorph::on[] = "On";
const char OSISMorph::off[] = "Off";
const char OSISMorph::optName[] = "Morphological Tags";
const char OSISMorph::optTip[] = "Toggles Morphological Tags On and Off if they exist";


OSISMorph::OSISMorph() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


OSISMorph::~OSISMorph() {
}

void OSISMorph::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISMorph::getOptionValue()
{
	return (option) ? on:off;
}

char OSISMorph::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want morph tags
		const char *from;
		char token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		bool lastspace = false;
		SWBuf orig = text;
		from = orig.c_str();

		for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				XMLTag tag(token);
				if ((!strcmp(tag.getName(), "w")) && (!tag.isEndTag())) {	// Morph
					if (tag.getAttribute("morph"))
						tag.setAttribute("morph", 0);
				}
				// keep tag, possibly with the morph removed
				text += tag;
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos] = 0;
			}
			else	{
				text += *from;
				lastspace = (*from == ' ');
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
