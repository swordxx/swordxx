/******************************************************************************
 *
 * thmllemma -	SWFilter decendant to hide or show lemmas
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmllemma.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char ThMLLemma::on[] = "On";
const char ThMLLemma::off[] = "Off";
const char ThMLLemma::optName[] = "Lemmas";
const char ThMLLemma::optTip[] = "Toggles Lemmas On and Off if they exist";

ThMLLemma::ThMLLemma() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLLemma::~ThMLLemma() {
}

void ThMLLemma::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLLemma::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLLemma::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want lemmas
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
				if (!strnicmp(token, "sync", 4) && strstr(token, "type=\"lemma\" ")) {	// Lemma
                                        continue;
				}
				// if not a lemma token, keep token in text
				text = '<';
                                text.append(token);
      				text += '>';
				continue;
			}
			if (intoken) {
				token += *from;
			}
			else {
                                text += *from;
       			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
