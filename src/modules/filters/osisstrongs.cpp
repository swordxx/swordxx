/******************************************************************************
 *
 * osisstrongs -	SWFilter descendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osisstrongs.h>
#include <swmodule.h>
#include <ctype.h>

SWORD_NAMESPACE_START

const char oName[] = "Strong's Numbers";
const char oTip[] = "Toggles Strong's Numbers On and Off if they exist";

const SWBuf choices[3] = {"Off", "On", ""};
const StringList oValues(&choices[0], &choices[2]);


OSISStrongs::OSISStrongs() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}


OSISStrongs::~OSISStrongs() {
}


char OSISStrongs::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	char token[2112]; // cheese.  Fix.
	int tokpos = 0;
	bool intoken = false;
	bool lastspace = false;
	int word = 1;
	char val[128];
	char wordstr[5];
	char *valto;
	char *ch;

	const SWBuf orig = text;
	const char * from = orig.c_str();

	for (text = ""; *from; ++from) {
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
							
							const char* startTagEnd = strstr(from, ">"); //end of the opening tag
							if (startTagEnd) {
								startTagEnd++;
								
								const char* endTagStart = strstr(startTagEnd, "</w>"); //end of the opening tag
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
			text.append('<');
			text.append(token);
			text.append('>');
			
			continue;
		}
		if (intoken) {
			if (tokpos < 2045)
				token[tokpos++] = *from;
				token[tokpos+2] = 0;
		}
		else	{
			text.append(*from);
			lastspace = (*from == ' ');
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
