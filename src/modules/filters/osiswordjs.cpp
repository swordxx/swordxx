/******************************************************************************
 *
 * osisstrongs -	SWFilter descendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <osiswordjs.h>
#include <swmodule.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif
#include <ctype.h>
#include <utilxml.h>
#include <versekey.h>

SWORD_NAMESPACE_START

const char oName[] = "Word Javascript";
const char oTip[] = "Toggles Word Javascript data";

const SWBuf choices[3] = {"Off", "On", ""};
const StringList oValues(&choices[0], &choices[2]);


OSISWordJS::OSISWordJS() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");

     defaultGreekLex   = 0;
     defaultHebLex     = 0;
     defaultGreekParse = 0;
     defaultHebParse   = 0;
     mgr               = 0;
}


OSISWordJS::~OSISWordJS() {
}


char OSISWordJS::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	char token[2112]; // cheese.  Fix.
	int tokpos = 0;
	bool intoken = false;
	bool lastspace = false;
	int wordNum = 1;
	char val[128];
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
				if (option) {
					XMLTag wtag(token);
					SWBuf lemma = wtag.getAttribute("lemma");
					SWBuf strong = "";
					SWBuf morph = wtag.getAttribute("morph");
					char gh = 0;
					VerseKey *vkey = 0;
					if (key) {
						vkey = SWDYNAMIC_CAST(VerseKey, key);
					}
					if ((!strncmp(lemma.c_str(), "x-Strongs:", 10)) || (!strncmp(lemma.c_str(), "strong:", 7))) {
						char *num = strstr(lemma.c_str(), ":");
						num++;
						gh = isdigit(*num) ? 0:*num;
						if (!gh) {
							if (vkey) {
								gh = vkey->Testament() ? 'H' : 'G';
							}
						}
						else num++;
						strong = num;
						SWModule *sLex = 0;
						if (gh == 'G') {
							sLex = defaultGreekLex;
						}
						if (gh == 'H') {
							sLex = defaultHebLex;
						}
						if (sLex) {
							sLex->setKey(strong.c_str());
							strong = sLex->RenderText();
						}
					}
					SWBuf layer = (vkey)?vkey->getOSISRef():key->getText();
					for (int i = 0; i < layer.size(); i++) {
						if ((!isdigit(layer[i])) && (!isalpha(layer[i]))) {
							layer[i] = '_';
						}
					}
					text.appendFormatted("<div id=\"%s_%d\" class=\"word-layer\">%s<br/>%s</div>", layer.c_str(), wordNum, strong.c_str(), morph.c_str());
					text.appendFormatted("<span onclick=\"wordInfo(\'%s_%d\', \'image0002\');\" >", layer.c_str(), wordNum);
					wordNum++;



				}
			}
			if ((*token == '/') && (token[1] == 'w') && option) {	// Word
				text += "</w></span>";
				continue;
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
