/******************************************************************************
 *
 * thmlheadings -	SWFilter decendant to hide or show headings
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlheadings.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char ThMLHeadings::on[] = "On";
const char ThMLHeadings::off[] = "Off";
const char ThMLHeadings::optName[] = "Headings";
const char ThMLHeadings::optTip[] = "Toggles Headings On and Off if they exist";


ThMLHeadings::ThMLHeadings() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLHeadings::~ThMLHeadings() {
}

void ThMLHeadings::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLHeadings::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLHeadings::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want headings
		SWBuf token;
		bool intoken = false;
		bool hide = false;

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
				if (!strnicmp(token, "div class=\"sechead\"", 19)) {
					hide = true;
					continue;
				}
				
				if (!strnicmp(token, "div class=\"title\"", 17)) {
					hide = true;
					continue;
				}
				else if (hide && !strnicmp(token, "/div", 4)) {
					hide = false;
					continue;
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
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
