/******************************************************************************
 *
 * UTF8UTF16 -	SWFilter decendant to convert UTF-8 to UTF-16
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include <utf8utf16.h>

UTF8UTF16::UTF8UTF16() {
}


char UTF8UTF16::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
  unsigned char *from;
  unsigned short *to;

  int len;
  unsigned long uchar;
  unsigned char significantFirstBits, subsequent;
  unsigned short schar;
  
	 if ((unsigned long)key < 2)	// hack, we're en(1)/de(0)ciphering
		return -1;
  len = strlen(text) + 1;						// shift string to right of buffer
  if (len < maxlen) {
    memmove(&text[maxlen - len], text, len);
    from = (unsigned char*)&text[maxlen - len];
  }
  else
    from = (unsigned char*)text;
  
  
  // -------------------------------
  
  for (to = (unsigned short*)text; *from; from++) {
    uchar = 0;
    if ((*from & 128) != 128) {
      //          	if (*from != ' ')
      uchar = *from;
    }
    else if ((*from & 128) && ((*from & 64) != 64)) {
      // error, do nothing
      continue;
    }
    else {
      *from <<= 1;
      for (subsequent = 1; (*from & 128); subsequent++) {
	*from <<= 1;
	from[subsequent] &= 63;
	uchar <<= 6;
	uchar |= from[subsequent];
      }
      subsequent--;
      *from <<=1;
      significantFirstBits = 8 - (2+subsequent);
      
      uchar |= (((short)*from) << (((6*subsequent)+significantFirstBits)-8));
      from += subsequent;
    }

    if (uchar < 0x1ffff) {
      *to++ = (unsigned short)uchar;
    }
    else {
      uchar -= 0x10000;
      schar = 0xD800 | (uchar & 0x03ff);
      uchar >>= 10;
      uchar |= 0xDC00;
      *to++ = (unsigned short)schar;
      *to++ = (unsigned short)uchar;
    }
  }
  *to = (unsigned short)0;

  return 0;
}

