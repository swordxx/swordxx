/******************************************************************************
 *
 * UTF8HebrewPoints - SWFilter descendant to remove UTF-8 Hebrew vowel points
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <utf8hebrewpoints.h>

SWORD_NAMESPACE_START

const char UTF8HebrewPoints::on[] = "On";
const char UTF8HebrewPoints::off[] = "Off";
const char UTF8HebrewPoints::optName[] = "Hebrew Vowel Points";
const char UTF8HebrewPoints::optTip[] = "Toggles Hebrew Vowel Points";

UTF8HebrewPoints::UTF8HebrewPoints() {
	option = true;
	options.push_back(on);
	options.push_back(off);
}

UTF8HebrewPoints::~UTF8HebrewPoints(){};

void UTF8HebrewPoints::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *UTF8HebrewPoints::getOptionValue()
{
	return (option) ? on:off;
}

char UTF8HebrewPoints::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	if (!option) {
		//The UTF-8 range 0xD6 0xB0 to 0xD6 0xBF excluding 0xD6 0x consist of Hebrew cantillation marks so block those out.
		SWBuf orig = text;
		const unsigned char* from = (unsigned char*)orig.c_str();
		for (text = ""; *from; from++) {
			if ((*from == 0xD6) && (*(from + 1) >= 0xB0 && *(from + 1) <= 0xBF) && (*(from + 1) != 0xBE)) {
				from++;
			}
			else {
     			        text += *from;
                        }
		}
     	}
	return 0;
}

SWORD_NAMESPACE_END
