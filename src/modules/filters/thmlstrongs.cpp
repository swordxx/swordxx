/******************************************************************************
 *
 * thmlstrongs -	SWFilter decendant to hide or show strongs number
 *			in a ThML module.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <thmlstrongs.h>
#include <swmodule.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif


const char ThMLStrongs::on[] = "On";
const char ThMLStrongs::off[] = "Off";
const char ThMLStrongs::optName[] = "Strong's Numbers";
const char ThMLStrongs::optTip[] = "Toggles Strong's Numbers On and Off if they exist";


ThMLStrongs::ThMLStrongs() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}


ThMLStrongs::~ThMLStrongs() {
}

void ThMLStrongs::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *ThMLStrongs::getOptionValue()
{
	return (option) ? on:off;
}

char ThMLStrongs::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	char *to, *from, token[2048]; // cheese.  Fix.
	int tokpos = 0;
	bool intoken = false;
	int len;
	bool lastspace = false;
	int word = 1;
	char val[128];
	char wordstr[5];
	char *valto;
	char *ch;

	len = strlen(text) + 1;	// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;
	
	// -------------------------------

	for (to = text; *from; from++) {
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
			if (!strnicmp(token, "sync type=\"Strongs\" ", 20)) {	// Strongs
				if (module->isProcessEntryAttributes()) {
					valto = val;
					for (unsigned int i = 27; token[i] != '\"' && i < 150; i++)
						*valto++ = token[i];
					*valto = 0;
					if (atoi((!isdigit(*val))?val+1:val) < 5627) {
						// normal strongs number
						sprintf(wordstr, "%03d", word++);
						module->getEntryAttributes()["Word"][wordstr]["Strongs"] = val;
					}
					else {
						// verb morph
						sprintf(wordstr, "%03d", word-1);
						module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
					}
				}

				if (!option) {	// if we don't want strongs
					if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
						if (lastspace)
							to--;
					}
					continue;
				}
			}
			if (module->isProcessEntryAttributes()) {
				if (!strncmp(token, "sync type=\"morph\"", 17)) {
					for (ch = token+17; *ch; ch++) {
						if (!strncmp(ch, "class=\"", 7)) {
							valto = val;
							for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
								*valto++ = ch[i];
							*valto = 0;
							sprintf(wordstr, "%03d", word-1);
							module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = val;
						}
						if (!strncmp(ch, "value=\"", 7)) {
							valto = val;
							for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
								*valto++ = ch[i];
							*valto = 0;
							sprintf(wordstr, "%03d", word-1);
							module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
						}
					}
				}
			}
			// if not a strongs token, keep token in text
			*to++ = '<';
			for (char *tok = token; *tok; tok++)
				*to++ = *tok;
			*to++ = '>';
			continue;
		}
		if (intoken) {
			if (tokpos < 2045)
				token[tokpos++] = *from;
				token[tokpos+2] = 0;
		}
		else	{
			*to++ = *from;
			lastspace = (*from == ' ');
		}
	}
	*to++ = 0;
	*to = 0;
	return 0;
}
