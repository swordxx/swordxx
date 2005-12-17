/******************************************************************************
 *
 * osisstrongs -	SWFilter descendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <ctype.h>
#include <osisstrongs.h>
#include <swmodule.h>
#include <versekey.h>
#include <utilxml.h>

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
	int wordNum = 1;
	char val[128];
	char wordstr[5];
	char *valto;
	char *ch;
	const char *wordStart = 0;

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
				if (module->isProcessEntryAttributes()) {
					wordStart = from+1;
					char gh = 0;
					VerseKey *vkey = 0;
					if (key) {
						vkey = SWDYNAMIC_CAST(VerseKey, key);
					}
					XMLTag wtag(token);
					SWBuf lemma      = wtag.getAttribute("lemma");
					SWBuf morph      = wtag.getAttribute("morph");
					SWBuf src        = wtag.getAttribute("src");
					SWBuf morphClass = "";
					SWBuf lemmaClass = "";


					const char *m = strchr(morph.c_str(), ':');
					if (m) {
						int len = m-morph.c_str();
						morphClass.append(morph.c_str(), len);
						morph << len+1;
					}
					m = strchr(lemma.c_str(), ':');
					if (m) {
						int len = m-lemma.c_str();
						lemmaClass.append(lemma.c_str(), len);
						lemma << len+1;
					}

					if ((lemmaClass == "x-Strongs") || (lemmaClass == "strong")) {
						gh = isdigit(lemma[0]) ? 0:lemma[0];
						if (!gh) {
							if (vkey) {
								gh = vkey->Testament() ? 'H' : 'G';
							}
						}
						else lemma << 1;
						lemmaClass = "strong";
					}
					if ((morphClass == "x-Robinsons") || (morphClass == "x-Robinson") || (morphClass == "Robinson")) {
						morphClass = "robinson";
					}

					sprintf(wordstr, "%03d", wordNum);
					if (gh) lemma.insert(0,gh);
					if (lemma.length())
					module->getEntryAttributes()["Word"][wordstr]["Lemma"] = lemma;
					if (lemmaClass.length())
					module->getEntryAttributes()["Word"][wordstr]["LemmaClass"] = lemmaClass;
					if (morph.length())
					module->getEntryAttributes()["Word"][wordstr]["Morph"] = morph;
					if (morphClass.length())
					module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = morphClass;
					if (src.length())
						module->getEntryAttributes()["Word"][wordstr]["Src"] = src;
					strcat(token, " wn=\"");
					strcat(token, wordstr);
					strcat(token, "\"");

					wordNum++;
				}

				if (!option) {
					// remove all lemmas.  This class should be renamed to OSISLemma
					char *num = strstr(token, "lemma=\"");
					if (num) {
						memcpy(num, "savlm", 5);
					}
				}
			}
			if ((*token == '/') && (token[1] == 'w')) {	// Word End
				if (module->isProcessEntryAttributes()) {
					if (wordStart) {
						SWBuf tmp;
						tmp.append(wordStart, (from-wordStart)-3);
						sprintf(wordstr, "%03d", wordNum-1);
						module->getEntryAttributes()["Word"][wordstr]["Text"] = tmp;
					}
				}
				wordStart = 0;
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
