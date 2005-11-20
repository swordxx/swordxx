/******************************************************************************
 *
 * UTF8UTF16 -	SWFilter descendant to convert UTF-8 to UTF-16
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include <utf8utf16.h>
#include <swbuf.h>

SWORD_NAMESPACE_START

UTF8UTF16::UTF8UTF16() {
}

char UTF8UTF16::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	const unsigned char *from;
	char digit[10];
	unsigned long ch;
        signed short utf16;
	unsigned char from2[7];

	SWBuf orig = text;

	from = (const unsigned char *)orig.c_str();

	// -------------------------------
	for (text = ""; *from; from++) {
		ch = 0;
                //case: ANSI
		if ((*from & 128) != 128) {
			text.setSize(text.size()+2);
			*((unsigned short *)(text.getRawData()+(text.size()-2))) = (unsigned short)*from;
			continue;
		}
                //case: Invalid UTF-8 (illegal continuing byte in initial position)
		if ((*from & 128) && ((*from & 64) != 64)) {
			continue;
		}
                //case: 2+ byte codepoint
		from2[0] = *from;
		from2[0] <<= 1;
		int subsequent;
		for (subsequent = 1; (from2[0] & 128) && (subsequent < 7); subsequent++) {
			from2[0] <<= 1;
			from2[subsequent] = from[subsequent];
			from2[subsequent] &= 63;
			ch <<= 6;
			ch |= from2[subsequent];
		}
		subsequent--;
		from2[0] <<= 1;
		char significantFirstBits = 8 - (2+subsequent);
		
		ch |= (((short)from2[0]) << (((6*subsequent)+significantFirstBits)-8));
		from += subsequent;
			if (ch < 0x10000) {
				text.setSize(text.size()+2);
				*((unsigned short *)(text.getRawData()+(text.size()-2))) = (unsigned short)ch;
			 }
			else {
				utf16 = (signed short)((ch - 0x10000) / 0x400 + 0xD800);
				text.setSize(text.size()+2);
				*((unsigned short *)(text.getRawData()+(text.size()-2))) = (unsigned short)utf16;
				utf16 = (signed short)((ch - 0x10000) % 0x400 + 0xDC00);
				text.setSize(text.size()+2);
				*((unsigned short *)(text.getRawData()+(text.size()-2))) = (unsigned short)utf16;
			}
	}
	text.setSize(text.size()+2);
	*((unsigned short *)(text.getRawData()+(text.size()-2))) = (unsigned short)0;
	   
	return 0;

}

SWORD_NAMESPACE_END
