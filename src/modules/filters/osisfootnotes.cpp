/******************************************************************************
 *
 *osishmlheadings -	SWFilter decendant to hide or show headings
 *			in an OSIS module.
 */


#include <stdlib.h>
#include <string.h>
#include <osisheadings.h>
#include <swmodule.h>
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
	option = false;
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

char OSISFootnotes::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	SWBuf token;
	bool intoken    = false;
	bool hide       = false;

	SWBuf orig = text;
	const char *from = orig.c_str();

	/*for (text = ""; *from; from++) {
		if (*from == '<') {
			intoken = true;
			token = "";
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;

			hide = (!option && !strincmp(token, "note"));

			// if not a footnote token, keep token in text
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
		else header += *from;
	}*/
	return 0;
}

SWORD_NAMESPACE_END

