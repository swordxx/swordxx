/******************************************************************************
 *
 * gbfmorph -	SWFilter decendant to hide or show morph tags
 *			in a GBF module.
 */


#include <stdlib.h>
#include <string.h>
#include <gbfmorph.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char GBFMorph::on[] = "On";
const char GBFMorph::off[] = "Off";
const char GBFMorph::optName[] = "Morphological Tags";
const char GBFMorph::optTip[] = "Toggles Morphological Tags On and Off if they exist";


GBFMorph::GBFMorph() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


GBFMorph::~GBFMorph() {
}

void GBFMorph::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *GBFMorph::getOptionValue()
{
	return (option) ? on:off;
}

char GBFMorph::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (!option) {	// if we don't want morph tags
		const char *from;
		char token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		int len;
		bool lastspace = false;

		SWBuf orig = text;
		from = orig.c_str();

		for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				if (*token == 'W' && token[1] == 'T') {	// Morph
				  if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
				    if (lastspace)
				      text--;
				  }
				  continue;
				}
				// if not a morph tag token, keep token in text
				text += '<';
				text += token;
				text += '>';
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;
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
