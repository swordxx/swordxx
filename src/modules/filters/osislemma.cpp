/******************************************************************************
 *
 * osislemma          -	SWFilter descendant to hide or show lemmata
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osislemma.h>
#include <utilxml.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char OSISLemma::on[] = "On";
const char OSISLemma::off[] = "Off";
const char OSISLemma::optName[] = "Lemmas";
const char OSISLemma::optTip[] = "Toggles Lemmas On and Off if they exist";


OSISLemma::OSISLemma() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


OSISLemma::~OSISLemma() {
}

void OSISLemma::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISLemma::getOptionValue()
{
	return (option) ? on:off;
}

char OSISLemma::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {	// if we don't want lemmas
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
				if ((!strcmp(tag.getName(), "w")) && (!tag.isEndTag())) {	// Lemma
					SWBuf lemma = tag.getAttribute("lemma");
					if (lemma.length()) {
						tag.setAttribute("lemma", 0);
					 	tag.setAttribute("savlm", lemma.c_str());
					}
				}
				// keep tag, possibly with the lemma removed
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
