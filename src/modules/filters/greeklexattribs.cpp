/******************************************************************************
 *
 * greeklexattribs -	SWFilter decendant to set entry attributes for greek
 * 	lexicons
 */


#include <stdlib.h>
#include <string.h>
#include <greeklexattribs.h>
#include <swmodule.h>


GreekLexAttribs::GreekLexAttribs() {
}


char GreekLexAttribs::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module) {
	char *from;
	bool inAV = false;
	string phrase;
	string freq;
	char *currentPhrase = 0;


	for (from = text; *from; from++) {
		if (inAV) {
			if (currentPhrase == 0) {
				if (isalpha(*from))
					currentPhrase = from;
			}
			else {
				if ((!isalpha(*from)) && (*from != ' ')) {
					phrase = "";
					phrase.copy(currentPhrase, (int)(from - currentPhrase));
					currentPhrase = from;
					while (*from && isdigit(*from)) from++;
					freq = "";
					freq.copy(currentPhrase, (int)(from - currentPhrase));
					module->getEntryAttributes()["AVPhrase"][currentPhrase]["Frequency"] = freq;
				}
			}
			if (*from == ';') inAV = false;

		}
		else if (!strncmp(from, "AV-", 3)) {
			inAV = true;
			from+=2;
		}
	}
	return 0;
}


