/******************************************************************************
 *
 * gbfplain -	SWFilter decendant to strip out all GBF tags or convert to
 *		ASCII rendered symbols.
 */


#include <stdlib.h>
#include <string.h>
#include <gbfplain.h>

SWORD_NAMESPACE_START

GBFPlain::GBFPlain() {
}


char GBFPlain::processText (SWBuf &text, const SWKey *key, const SWModule *module)
{
	char token[2048];
	int tokpos = 0;
	bool intoken = false;
	const char *from;
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
		if (*from == '>') {
			intoken = false;
						// process desired tokens
			switch (*token) {
			case 'W':	// Strongs
				switch(token[1]) {
					case 'G':               // Greek
					case 'H':               // Hebrew
					case 'T':               // Tense
						text += " <";
						for (char *tok = token + 2; *tok; tok++)
							text += *tok;
						text += "> ";
						continue;
				}
				break;
			case 'R':
				switch(token[1]) {
				case 'F':               // footnote begin
					text += " [";
					continue;
				case 'f':               // footnote end
					text += "] ";
					continue;
				}
				break;
			case 'C':
				switch(token[1]) {
				case 'A':               // ASCII value
					text += (char)atoi(&token[2]);
					continue;
				case 'G':
					text += ">";
					continue;
/*								Bug in WEB
				case 'L':
					*to++ = '<';
					continue;
*/
				case 'L':	//        Bug in WEB.  Use above entry when fixed
				case 'N':               // new line
					text += '\n';
					continue;
				case 'M':               // new paragraph
					text += "\n\n";
					continue;
				}
				break;
			}
			continue;
		}
		if (intoken) {
			if (tokpos < 2045)
				token[tokpos++] = *from;
				token[tokpos+2] = 0;
		}
		else	text += *from;
	}
	return 0;
}

SWORD_NAMESPACE_END
