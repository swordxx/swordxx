/******************************************************************************
 *
 * gbfrtf -	SWFilter decendant to convert all GBF tags to RTF tags
 */


#include <stdlib.h>
#include <string.h>
#include <gbfrtf.h>
#include <ctype.h>

GBFRTF::GBFRTF() {
}


char GBFRTF::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	unsigned char *to, *from;
	char token[2048];
	int tokpos = 0;
	bool intoken = false;
	int len;
	const char *tok;

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = (unsigned char *)&text[maxlen - len];
	}
	else	from = (unsigned char *)text;							// -------------------------------
	for (to = (unsigned char *)text; *from; from++) {
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
					*to++ = '{';
					*to++ = '\\';
					*to++ = 'c';
					*to++ = 'f';
					*to++ = '3';
					*to++ = ' ';
					*to++ = '\\';
					*to++ = 's';
					*to++ = 'u';
					*to++ = 'b';
					*to++ = ' ';
					*to++ = '<';
					for (tok = token + 2; *tok; tok++)
						*to++ = *tok;
					*to++ = '>';
					*to++ = '}';
					continue;

				case 'T':               // Tense
					*to++ = '{';
					*to++ = '\\';
					*to++ = 'c';
					*to++ = 'f';
					*to++ = '4';
					*to++ = ' ';
					*to++ = '\\';
					*to++ = 's';
					*to++ = 'u';
					*to++ = 'b';
					*to++ = ' ';
					*to++ = '(';
					bool separate = false;
					for (tok = token + 2; *tok; tok++) {
						if (separate) {
							*to++ = ';';
							*to++ = ' ';
							separate = false;
						}
						switch (*tok) {
						case 'G':
						case 'H':
							for (tok++; *tok; tok++) {
								if (isdigit(*tok)) {
									*to++ = *tok;
									separate = true;
								}
								else {
									tok--;
									break;
								}
							}
							break;
						default:
							for (; *tok; tok++) {
							       *to++ = *tok;
							}
						}
					}
					*to++ = ')';
					*to++ = '}';
					continue;
				}
				break;
			case 'R':
				switch(token[1]) {
				case 'X':
				  *to++ = '<';
				  *to++ = 'a';
				  *to++ = ' ';
				  *to++ = 'h';
				  *to++ = 'r';
				  *to++ = 'e';
				  *to++ = 'f';
				  *to++ = '=';
				  *to++ = '"';
				  *to++ = '"';
				  *to++ = '>';
				  continue;
				case 'x':
				  *to++ = '<';
				  *to++ = '/';
				  *to++ = 'a';
				  *to++ = '>';
				  continue;
				case 'F':               // footnote begin
					*to++ = '{';
					*to++ = '\\';
					*to++ = 'i';
					*to++ = '1';
					*to++ = ' ';
					*to++ = '\\';
					*to++ = 's';
					*to++ = 'u';
					*to++ = 'b';
					*to++ = ' ';
					*to++ = '(';
					continue;
				case 'f':               // footnote end
					*to++ = ')';
					*to++ = ' ';
					*to++ = '}';
					continue;
				}
				break;
			case 'F':			// font tags
				switch(token[1]) {
				case 'I':		// italic start
					*to++ = '\\';
					*to++ = 'i';
					*to++ = '1';
					*to++ = ' ';
					continue;
				case 'i':		// italic end
					*to++ = '\\';
					*to++ = 'i';
					*to++ = '0';
					*to++ = ' ';
					continue;
				case 'B':		// bold start
					*to++ = '\\';
					*to++ = 'b';
					*to++ = '1';
					*to++ = ' ';
					continue;
				case 'b':		// bold end
					*to++ = '\\';
					*to++ = 'b';
					*to++ = '0';
					*to++ = ' ';
					continue;
				case 'N':
					   *to++ = '{';
					   if (!strnicmp(token+2, "Symbol", 6)) {
					  *to++ = '\\';
					  *to++ = 'f';
					  *to++ = '7';
					  *to++ = ' ';
					}
					continue;
				case 'n':
					*to++ = '}';
					continue;
				case 'S':
					   *to++ = '{';
					   *to++ = '\\';
					*to++ = 's';
					*to++ = 'u';
					*to++ = 'p';
					*to++ = 'e';
					*to++ = 'r';
					*to++ = ' ';
					continue;
				case 's':
						   *to++ = '}';
					continue;
				case 'R':
					   *to++ = '{';
					*to++ = '\\';
					*to++ = 'c';
					*to++ = 'f';
					*to++ = '6';
					*to++ = ' ';
					continue;
				case 'r':
					   *to++ = '}';
					continue;
				}
				break;
			case 'C':			// special character tags
				switch(token[1]) {
				case 'A':               // ASCII value
					*to++ = (char)atoi(&token[2]);
					continue;
				case 'G':
					*to++ = '>';
					continue;
				case 'L':               // line break
					*to++ = '\\';
					*to++ = 'l';
					*to++ = 'i';
					*to++ = 'n';
					*to++ = 'e';
					*to++ = ' ';
					continue;
				case 'M':               // new paragraph
					*to++ = '\\';
					*to++ = 'p';
					*to++ = 'a';
					*to++ = 'r';
					*to++ = ' ';
					continue;
				case 'T':
					*to++ = '<';
				}
				break;
			case 'T':			// title formatting
			  switch(token[1])
			    {
			    case 'T':               // Book title begin
			      *to++ = '{';
			      *to++ = '\\';
				 *to++ = 'l';
				 *to++ = 'a';
				 *to++ = 'r';
				 *to++ = 'g';
				 *to++ = 'e';
				 *to++ = ' ';
				 continue;
			    case 't':
				 *to++ = '}';
				 continue;
			    case 'S':
				 *to++ = '\\';
				 *to++ = 'p';
				 *to++ = 'a';
				 *to++ = 'r';
				 *to++ = ' ';
				 *to++ = '{';
			      *to++ = '\\';
			      *to++ = 'i';
			      *to++ = '1';
			      *to++ = '\\';
			      *to++ = 'b';
			      *to++ = '1';
			      *to++ = ' ';
			      continue;
			    case 's':
			      *to++ = '}';
			      *to++ = '\\';
			      *to++ = 'p';
			      *to++ = 'a';
			      *to++ = 'r';
			      *to++ = ' ';
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
		else	*to++ = *from;
	}
	*to++ = 0;
	*to = 0;
	return 0;
}
