/******************************************************************************
 *
 * UTF8UTF16 -	SWFilter descendant to convert UTF-8 to UTF-16
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include <utf8utf16.h>

SWORD_NAMESPACE_START

UTF8UTF16::UTF8UTF16() {
}


char UTF8UTF16::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	const unsigned char *from;

	int len;
	unsigned long uchar, uchars[10];
	unsigned char significantFirstBits, subsequent;
	unsigned short schar;
  
	 if ((unsigned long)key < 2)	// hack, we're en(1)/de(0)ciphering
		return -1;

  
	SWBuf orig = text;
	from = (const unsigned char *)orig.c_str();

	for (text = ""; *from; from++) {
		uchar = 0;
		if ((*from & 128) != 128) {
			//if (*from != ' ')
			uchar = *from;
		}
		else if ((*from & 128) && ((*from & 64) != 64)) {
			// error, do nothing
			continue;
		}
		else {
			uchars[0] = *from;
			uchars[0] <<= 1;
			for (subsequent = 1; (uchars[0] & 128) && (subsequent < 10); subsequent++) {
				uchars[0] <<= 1;
				uchars[subsequent] = from[subsequent];
				uchars[subsequent] &= 63;
				uchar <<= 6;
				uchar |= uchars[subsequent];
			}
			subsequent--;
			uchars[0] <<=1;
			significantFirstBits = 8 - (2+subsequent);
	 
			uchar |= (((short)uchars[0]) << (((6*subsequent)+significantFirstBits)-8));
			from += subsequent;
		}

		if (uchar < 0x1ffff) {
			text.setSize(text.size()+2);
			*((unsigned short *)(text.getRawData()+(text.size()-2))) = (unsigned short)uchar;
		}
		else {
			uchar -= 0x10000;
			schar = 0xD800 | (uchar & 0x03ff);
			uchar >>= 10;
			uchar |= 0xDC00;
			text.setSize(text.size()+4);
			*((unsigned short *)(text.getRawData()+(text.size()-4))) = (unsigned short)schar;
			*((unsigned short *)(text.getRawData()+(text.size()-2))) = (unsigned short)uchar;
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
