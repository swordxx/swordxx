/******************************************************************************
 *
 * UTF8Latin1 -	SWFilter decendant to convert UTF-8 to Latin-1
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include <utf8latin1.h>

UTF8Latin1::UTF8Latin1(char rchar) : replacementChar(rchar) {
}


char UTF8Latin1::ProcessText(char *text, int maxlen, const SWKey *key)
{
  unsigned char *from;
  unsigned short *to;

  int len;
  unsigned long uchar;
  unsigned char significantFirstBits, subsequent;
  unsigned short schar;
  
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

    if (uchar < 0xff) {
        *to++ = (unsigned char)uchar;
    }
    else {
        *to++ = replacementChar;
    }
  }
  *to++ = 0;
  *to = 0;

  return 0;
}

