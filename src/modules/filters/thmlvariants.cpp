/******************************************************************************
 *
 * thmlvariants -	SWFilter decendant to hide or show textual variants
 *			in a ThML module.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlvariants.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

SWORD_NAMESPACE_START

const char ThMLVariants::primary[] = "Primary Reading";
const char ThMLVariants::secondary[] = "Secondary Reading";
const char ThMLVariants::all[] = "All Readings";

const char ThMLVariants::optName[] = "Textual Variants";
const char ThMLVariants::optTip[] = "Switch between Textual Variants modes";


ThMLVariants::ThMLVariants() {
	option = false;
	options.push_back(primary);
	options.push_back(secondary);
	options.push_back(all);
}


ThMLVariants::~ThMLVariants() {
}

void ThMLVariants::setOptionValue(const char *ival)
{
	if (!stricmp(ival, primary)) option = 0;
        else if (!stricmp(ival, secondary)) option = 1;
        else option = 2;
}

const char *ThMLVariants::getOptionValue()
{
        if (option == 0) {
	        return primary;
	}
	else if (option == 1) {
	        return secondary;
	}
	else {
	        return all;
	}
}

char ThMLVariants::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
        if (option == 0) { //we want primary only
		char *to, *from, token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		int len;
		bool hide = false;

		len = strlen(text) + 1;	// shift string to right of buffer
		if (len < maxlen) {
			memmove(&text[maxlen - len], text, len);
			from = &text[maxlen - len];
		}
		else	from = text;
		
		// -------------------------------

		for (to = text; *from; from++) {
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
				if (!strncmp(token, "div type=\"variant\" class=\"2\"", 28)) {
				  hide = true;
                                  continue;
				}
				else if (!strncmp(token, "/div", 4)) {
				  hide = false;
                                  continue;
				}

				// if not a footnote token, keep token in text
				if (!hide) {
					*to++ = '<';
					for (char *tok = token; *tok; tok++)
						*to++ = *tok;
					*to++ = '>';
				}
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;
			}
			else	{
				if (!hide) {
					*to++ = *from;
				}
			}
		}
		*to++ = 0;
		*to = 0;

	}
        else if (option == 1) { //we want variant only
		char *to, *from, token[2048]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		int len;
		bool hide = false;

		len = strlen(text) + 1;	// shift string to right of buffer
		if (len < maxlen) {
			memmove(&text[maxlen - len], text, len);
			from = &text[maxlen - len];
		}
		else	from = text;

		// -------------------------------

		for (to = text; *from; from++) {
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
				if (!strncmp(token, "div type=\"variant\" class=\"1\"", 28)) {
				  hide = true;
                                  continue;
				}
				else if (!strncmp(token, "/div", 4)) {
				  hide = false;
                                  continue;
				}

				// if not a footnote token, keep token in text
				if (!hide) {
					*to++ = '<';
					for (char *tok = token; *tok; tok++)
						*to++ = *tok;
					*to++ = '>';
				}
				continue;
			}
			if (intoken) {
				if (tokpos < 2045)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;
			}
			else	{
				if (!hide) {
					*to++ = *from;
				}
			}
		}
		*to++ = 0;
		*to = 0;

	}
	return 0;
}






SWORD_NAMESPACE_END
