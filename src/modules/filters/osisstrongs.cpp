/******************************************************************************
 *
 * osisstrongs -	SWFilter descendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osisstrongs.h>
#include <swmodule.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif
#include <ctype.h>

SWORD_NAMESPACE_START

const char oName[] = "Strong's Numbers";
const char oTip[] = "Toggles Strong's Numbers On and Off if they exist";

const SWBuf choices[3] = {"On", "Off", ""};
const StringList oValues(&choices[0], &choices[2]);


OSISStrongs::OSISStrongs() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


OSISStrongs::~OSISStrongs() {
}


char OSISStrongs::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	const char *from;
	char token[2048]; // cheese.  Fix.
	int tokpos = 0;
	bool intoken = false;
	int len;
	bool lastspace = false;
	int word = 1;
	char val[128];
	char wordstr[5];
	char *valto;
	char *ch;

	SWBuf orig = text;
	from = orig.c_str();

	len = strlen(text) + 1;	// shift string to right of buffer

	for (text = ""; *from; from++) {
		if (*from == '<') {
			intoken = true;
			tokpos = 0;
			token[0] = 0;
			token[1] = 0;
			token[2] = 0;
			//textEnd = text.size();
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;
			if ((*token == 'w') && (token[1] == ' ')) {	// Word
				*wordstr = 0;
				if (module->isProcessEntryAttributes()) {
					valto = val;
					char *num = strstr(token, "lemma=\"x-Strongs:");					
					int strongMarkerLength = 17;
					if (!num) { //try alternative strong marker value
						num = strstr(token, "lemma=\"strong:");
						strongMarkerLength = 14;
					}

					if (num) {
						for (num+=strongMarkerLength; ((*num) && (*num != '\"')); num++) {
							*valto++ = *num;
						}
						*valto = 0;
						
						if (atoi((!isdigit(*val))?val+1:val) < 5627) {
							// normal strongs number
							sprintf(wordstr, "%03d", word++);
							module->getEntryAttributes()["Word"][wordstr]["Strongs"] = val;
							
							//now try to find the end tag to get the text between <w> and </w> to set the entry attribute
							
							char* startTagEnd = strstr(from, ">"); //end of the opening tag
							if (startTagEnd) {
								startTagEnd++;
								
								char* endTagStart = strstr(startTagEnd, "</w>"); //end of the opening tag
								if (endTagStart && endTagStart > startTagEnd) { //content in between
									SWBuf tmp;
									tmp.append(startTagEnd, endTagStart - startTagEnd);
									module->getEntryAttributes()["Word"][wordstr]["Text"] = tmp;
								}
							}
						}
						else {
							// verb morph
							sprintf(wordstr, "%03d", word-1);
							module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
						}
					}
				}
				if (wordstr) {
					strcat(token, " wn=\"");
					strcat(token, wordstr);
					strcat(token, "\"");
				}
				if (!option) {
					char *num = strstr(token, "lemma=\"x-Strongs:");
					if (num) {
						memcpy(num, "savlm", 5);
					}
					else {
						num = strstr(token, "lemma=\"strong:");
						if (num) {
							memcpy(num, "savlm", 5);
						}
					}
				}
			}
			// if not a strongs token, keep token in text
			text += '<';
			for (char *tok = token; *tok; tok++) {
				text += *tok;
			}
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
	return 0;
}

SWORD_NAMESPACE_END
