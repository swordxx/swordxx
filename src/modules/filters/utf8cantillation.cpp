/******************************************************************************
 *
 * UTF8Cantillation - SWFilter decendant to remove UTF-8 Hebrew cantillation
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <utf8cantillation.h>


const char UTF8Cantillation::on[] = "On";
const char UTF8Cantillation::off[] = "Off";
const char UTF8Cantillation::optName[] = "Hebrew Cantillation";
const char UTF8Cantillation::optTip[] = "Toggles Hebrew Cantillation Marks";

UTF8Cantillation::UTF8Cantillation() {
	option = false;
	options.push_back(on);
	options.push_back(off);
}

UTF8Cantillation::~UTF8Cantillation(){};

void UTF8Cantillation::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *UTF8Cantillation::getOptionValue()
{
	return (option) ? on:off;
}

char UTF8Cantillation::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	if (!option) {
	unsigned char *to, *from;
	to = (unsigned char*)text;
	//The UTF-8 range 0xD6 0x90 to 0xD6 0xAF and 0xD7 0x84 consist of Hebrew cantillation marks so block those out.
	for (from = (unsigned char*)text; *from; from++) {
	  if (*from != 0xD6) {
	    if (*from == 0xD7 && *(from + 1) == 0x84) {
	      from++;
	    }
	    else {
	      *to++ = *from;
	    }
	  }
	  else if (*(from + 1) < 0x90 || *(from + 1) > 0xAF) {
	    *to++ = *from;
	    from++;
	    *to++ = *from;
	  }
          else {
                from++;
          }
	}
	*to++ = 0;
	*to = 0;
     }
	return 0;
}
