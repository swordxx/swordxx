/******************************************************************************
 *
 * GBFRedLetterWords -	SWFilter decendant to hide or show strongs number
 *			in a GBF module.
 */


#include <stdlib.h>
#include <string.h>
#include <gbfredletterwords.h>
#include <swmodule.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif
#include <ctype.h>

SWORD_NAMESPACE_START

const char GBFRedLetterWords::on[] = "On";
const char GBFRedLetterWords::off[] = "Off";
const char GBFRedLetterWords::optName[] = "Red letter words";
const char GBFRedLetterWords::optTip[] = "Toggles Red letter words  On and Off if they exist";


GBFRedLetterWords::GBFRedLetterWords() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


GBFRedLetterWords::~GBFRedLetterWords() {
}

void GBFRedLetterWords::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *GBFRedLetterWords::getOptionValue()
{
	return (option) ? on:off;
}

char GBFRedLetterWords::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
/** This function removes the red letter words in Bible like the WEB
* The words are marked by <FR> as start and <Fr> as end tag.
*/
	if (!option) {	// if we don't want footnotes
		char token[4096]; // cheese.  Fix.
		int tokpos = 0;
		bool intoken = false;
		int len;
		bool hide = false;

	const char *from;
	SWBuf orig = text;
	from = orig.c_str();
	for (text = ""; *from; from++) {
			if (*from == '<') {
				intoken = true;
				tokpos = 0;
//				memset(token, 0, 4096);
				token[0] = 0;
				token[1] = 0;
				token[2] = 0;
				continue;
			}
			if (*from == '>') {	// process tokens
				intoken = false;
				/*switch (*token) {
				case 'F':			// Font attribute
					switch(token[1]) {
					case 'R':               // Begin red letter words
						hide = true;
						break;
					case 'r':               // end red letter words
						hide = false;
						break;
					}
					continue;	// skip token
				}*/

				//hide the token if either FR or Fr was detected
				hide = (token[0] == 'F' && ( (token[1] == 'R') || (token[1] == 'r') ));

				// if not a red letter word token, keep token in text
				if (!hide) {
					text += '<';
					for (char *tok = token; *tok; tok++)
						text += *tok;
					text += '>';
				}
				continue;
			}
			if (intoken) {
				if (tokpos < 4090)
					token[tokpos++] = *from;
					token[tokpos+2] = 0;	// +2 cuz we init token with 2 extra '0' because of switch statement
			}
			else {
				text += *from;
			}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
