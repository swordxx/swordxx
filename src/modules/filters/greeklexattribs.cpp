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

	if (module->isProcessEntryAttributes()) {
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
						phrase.append(currentPhrase, (int)(from - currentPhrase)-1);
						currentPhrase = from;
						while (*from && isdigit(*from)) from++;
						freq = "";
						freq.append(currentPhrase, (int)(from - currentPhrase));
						module->getEntryAttributes()["AVPhrase"][phrase]["Frequency"] = freq;
						currentPhrase = 0;
					}
				}
				if (*from == ';') inAV = false;

			}
			else if (!strncmp(from, "AV-", 3)) {
				inAV = true;
				from+=2;
			}
		}
	}
	return 0;
}


