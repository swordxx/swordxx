/******************************************************************************
 *
 * osisstrongs -	SWFilter decendant to hide or show strongs number
 *			in a OSIS module.
 */


#include <stdlib.h>
#include <string.h>
#include <osisstrongs.h>
#include <swmodule.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif
#include <ctype.h>

SWORD_NAMESPACE_START

const char OSISStrongs::on[] = "On";
const char OSISStrongs::off[] = "Off";
const char OSISStrongs::optName[] = "Strong's Numbers";
const char OSISStrongs::optTip[] = "Toggles Strong's Numbers On and Off if they exist";


OSISStrongs::OSISStrongs() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


OSISStrongs::~OSISStrongs() {
}

void OSISStrongs::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *OSISStrongs::getOptionValue()
{
	return (option) ? on:off;
}

char OSISStrongs::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
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
	unsigned int textStart = 0, textEnd = 0;
	bool newText = false;
	std::string tmp;

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
			textEnd = text.size();
			continue;
		}
		if (*from == '>') {	// process tokens
			intoken = false;
			if ((*token == 'w') && (token[1] == ' ')) {	// Word
				if (module->isProcessEntryAttributes()) {
					valto = val;
					char *num = strstr(token, "lemma=\"x-Strongs:");
					if (num) {
						for (num+=17; ((*num) && (*num != '\"')); num++)
							*valto++ = *num;
						*valto = 0;
						if (atoi((!isdigit(*val))?val+1:val) < 5627) {
							// normal strongs number
							sprintf(wordstr, "%03d", word++);
							module->getEntryAttributes()["Word"][wordstr]["Strongs"] = val;
							tmp = "";
							tmp.append(textStart, (int)(textEnd - textStart));
							module->getEntryAttributes()["Word"][wordstr]["Text"] = tmp;
							newText = true;
						}
						else {
							// verb morph
							sprintf(wordstr, "%03d", word-1);
							module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
						}
					}
				}
				if (!option) {
					char *num = strstr(token, "lemma=\"x-Strongs:");
					if (num) {
						for (int i = 0; i < 17; i++)
							*num++ = ' ';
						for (; ((*num) && (*num!='\"')); num++)
							*num = ' ';
						if (*num)
							*num = ' ';
					}
				}
			}
			// if not a strongs token, keep token in text
			text += '<';
			for (char *tok = token; *tok; tok++)
				text += *tok;
			text += '>';
			if (newText) {textStart = text.size(); newText = false; }
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
