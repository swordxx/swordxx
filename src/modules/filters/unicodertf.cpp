/******************************************************************************
 *
 * unicodertf -	SWFilter decendant to convert a double byte unicode file
 *				 to RTF tags
 */


#include <stdlib.h>
#include <stdio.h>
#include <unicodertf.h>

SWORD_NAMESPACE_START

UnicodeRTF::UnicodeRTF() {
}


char UnicodeRTF::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	unsigned char *from;
	int len;
	char digit[10];
	short ch;	// must be signed per unicode spec (negative is ok for big numbers > 32768)

	SWBuf orig = text;

	len = strlenw(text.c_str()) + 2;						// shift string to right of buffer
#warning is this right? I needed to cast "const" away.
	from = (unsigned char*)orig.c_str();

	// -------------------------------
	bool lastUni = false;
	for (text = ""; *from; from++) {
		ch = 0;
		if ((*from & 128) != 128) {
//			if ((*from == ' ') && (lastUni))
//				*to++ = ' ';
			text += *from;
			lastUni = false;
			continue;
		}
		if ((*from & 128) && ((*from & 64) != 64)) {
	    // error
			*from = 'x';
			continue;
		}
		*from <<= 1;
		int subsequent;
		for (subsequent = 1; (*from & 128); subsequent++) {
			*from <<= 1;
			from[subsequent] &= 63;
			ch <<= 6;
			ch |= from[subsequent];
		}
		subsequent--;
		*from <<=1;
		char significantFirstBits = 8 - (2+subsequent);
		
		ch |= (((short)*from) << (((6*subsequent)+significantFirstBits)-8));
		from += subsequent;
		text += '\\';
		text += 'u';
		sprintf(digit, "%d", ch);
		for (char *dig = digit; *dig; dig++)
			text += *dig;
		text += '?';
		lastUni = true;
	}
	   
	return 0;
}

SWORD_NAMESPACE_END
