/******************************************************************************
 *
 * UTF8HebrewPoints - SWFilter decendant to remove UTF-8 Hebrew vowel points
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <utf8hebrewpoints.h>


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

char UTF8HebrewPoints::ProcessText(char *text, int maxlen, const SWKey *key)
{
    unsigned char *to, *from;
	int len;
	
	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
	  memmove(&text[maxlen - len], text, len);
	  from = (unsigned char*)&text[maxlen - len];
	}
	else	
	  from = (unsigned char*)text;							// -------------------------------

	//The UTF-8 range 0xD6 0x90 to 0xD6 0xAF and 0xD7 0x84 consist of Hebrew cantillation marks so block those out.
	for (to = (unsigned char*)text; *from; from++) {
	  if (*from == 0xD6 && (*(from + 1) <= 0xB0 && *(from + 1) => 0xBF) && *(from + 1) != 0xBE) {
	    from++;
	  }
	  else
	    *to++ = *from;
	}
	*to++ = 0;
	*to = 0;
	return 0;
}
