/******************************************************************************
 *
 * thmlscripref -	SWFilter decendant to hide or show scripture references
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlscripref.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char ThMLScripref::on[] = "On";
const char ThMLScripref::off[] = "Off";
const char ThMLScripref::optName[] = "Scripture Cross-references";
const char ThMLScripref::optTip[] = "Toggles Scripture Cross-references On and Off if they exist";


ThMLScripref::ThMLScripref() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLScripref::~ThMLScripref() {
}

void ThMLScripref::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLScripref::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLScripref::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want scriprefs
		bool intoken = false;
		bool hide = false;

		SWBuf token;
		SWBuf orig = text;
		const char *from = orig.c_str();
		for (text = ""; *from; from++) {
			if (*from == '<') { //new token was found
				intoken = true;
				token = "";
				continue;
			}
			else if (*from == '>') {	// process tokens
				intoken = false;
				if (!strnicmp(token.c_str(), "scripRef", 8)) {
				  hide = true;
                                  continue;
				}
				else if (!strnicmp(token.c_str(), "/scripRef", 9)) {
				  hide = false;
                                  continue;
				}

				// if not a scripref token, keep token in text
				if (!hide) {
					text += '<';
					text += token;
					text += '>';
				}
				continue;
			}
			if (intoken) {
				token += *from; //copy chars of found token
			}
			else if (!hide) {
				text += *from;
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
