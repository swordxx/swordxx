/******************************************************************************
 *
 * thmlmorph -	SWFilter decendant to hide or show morph tags
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlmorph.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char ThMLMorph::on[] = "On";
const char ThMLMorph::off[] = "Off";
const char ThMLMorph::optName[] = "Morphological Tags";
const char ThMLMorph::optTip[] = "Toggles Morphological Tags On and Off if they exist";


ThMLMorph::ThMLMorph() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLMorph::~ThMLMorph() {
}

void ThMLMorph::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLMorph::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLMorph::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want morph tags
		bool intoken = false;

		SWBuf token;
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
				if (!strnicmp(token, "sync", 4) && strstr(token, "type=\"morph\"")) {	// Morph
				  continue;
				}
				// if not a morph tag token, keep token in text
				text += '<';
                                text.append(token);
				text += '>';
				continue;
			}
			if (intoken) {
				token += *from
			}
			else	{
				text += *from;
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
