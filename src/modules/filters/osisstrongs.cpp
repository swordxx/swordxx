/******************************************************************************
 *
 * osisstrongs -	SWFilter descendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <stdio.h>
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
	char wordstr[5];
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
					SWBuf lemma      = "";
					SWBuf morph      = "";
					SWBuf src        = "";
					SWBuf morphClass = "";
					SWBuf lemmaClass = "";

					const char *attrib;
					sprintf(wordstr, "%03d", wordNum);
					if ((attrib = wtag.getAttribute("morph"))) {
						int count = wtag.getAttributePartCount("morph", ' ');
						int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
						do {
							SWBuf mClass = "";
							SWBuf mp = "";
							attrib = wtag.getAttribute("morph", i, ' ');
							if (i < 0) i = 0;	// to handle our -1 condition

							const char *m = strchr(attrib, ':');
							if (m) {
								int len = m-attrib;
								mClass.append(attrib, len);
								attrib += (len+1);
							}
							if ((mClass == "x-Robinsons") || (mClass == "x-Robinson") || (mClass == "Robinson")) {
								mClass = "robinson";
							}
							if (i) { morphClass += " "; morph += " "; }
							mp += attrib;
							morphClass += mClass;
							morph += mp;
							if (count > 1) {
								SWBuf tmp;
								tmp.setFormatted("Morph.%d", i+1);
								module->getEntryAttributes()["Word"][wordstr][tmp] = mp;
								tmp.setFormatted("MorphClass.%d", i+1);
								module->getEntryAttributes()["Word"][wordstr][tmp] = mClass;
							}
						} while (++i < count);
					}

					if ((attrib = wtag.getAttribute("lemma"))) {
						int count = wtag.getAttributePartCount("lemma", ' ');
						int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
						do {
							SWBuf lClass = "";
							SWBuf l = "";
							attrib = wtag.getAttribute("lemma", i, ' ');
							if (i < 0) i = 0;	// to handle our -1 condition

							const char *m = strchr(attrib, ':');
							if (m) {
								int len = m-attrib;
								lClass.append(attrib, len);
								attrib += (len+1);
							}
							if ((lClass == "x-Strongs") || (lClass == "strong") || (lClass == "Strong")) {
								gh = isdigit(attrib[0]) ? 0:attrib[0];
								if (!gh) {
									if (vkey) {
										gh = vkey->Testament() ? 'H' : 'G';
									}
								}
								else attrib++;
								lClass = "strong";
							}
							if (gh) l += gh;
							l += attrib;
							if (i) { lemmaClass += " "; lemma += " "; }
							lemma += l;
							lemmaClass += lClass;
							if (count > 1) {
								SWBuf tmp;
								tmp.setFormatted("Lemma.%d", i+1);
								module->getEntryAttributes()["Word"][wordstr][tmp] = l;
								tmp.setFormatted("LemmaClass.%d", i+1);
								module->getEntryAttributes()["Word"][wordstr][tmp] = lClass;
							}
						} while (++i < count);
						module->getEntryAttributes()["Word"][wordstr]["PartCount"].setFormatted("%d", count);
					}

					if ((attrib = wtag.getAttribute("src"))) {
						int count = wtag.getAttributePartCount("src", ' ');
						int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
						do {
							SWBuf mp = "";
							attrib = wtag.getAttribute("src", i, ' ');
							if (i < 0) i = 0;	// to handle our -1 condition

							if (i) src += " ";
							mp += attrib;
							src += mp;
							if (count > 1) {
								SWBuf tmp;
								tmp.setFormatted("Src.%d", i+1);
								module->getEntryAttributes()["Word"][wordstr][tmp] = mp;
							}
						} while (++i < count);
					}


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

					if (wtag.isEmpty()) {
						int j;
						for (j = strlen(token)-1; ((j>0) && (strchr(" /", token[j]))); j--);
						token[j+1] = 0;
					}
					
					strcat(token, " wn=\"");
					strcat(token, wordstr);
					strcat(token, "\"");

					if (wtag.isEmpty()) {
						strcat(token, "/");
					}

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
			
			// keep token in text
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
