/******************************************************************************
 *
 * UTF16UTF8 -	SWFilter decendant to convert UTF-16 to UTF-8
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include <utf16utf8.h>

UTF16UTF8::UTF16UTF8() {
}


char UTF16UTF8::ProcessText(char *text, int maxlen, const SWKey *key)
{
  unsigned short *from;
  unsigned char *to;
  int32_t len;

  unsigned long uchar;
  unsigned short schar;

  len = 0;
  from = (unsigned short*) text;
  while (*from) len++;
  len++;

  // shift string to right of buffer
  if (len < maxlen) {
    memmove(&text[maxlen - len], text, len);
    from = (unsigned short*)&text[maxlen - len];
  }
  else
    from = (unsigned short*)text;
  
  
  // -------------------------------
  
  for (to = (unsigned char*)text; *from; from++) {
    uchar = 0;

    if (*from < 0xD800 || *from > 0xDFFF) {
      uchar = *from;
    }
    else if (*from >= 0xD800 && *from <= 0xDBFF) {
      uchar = *from;
      schar = *(from+1);
      if (uchar < 0xDC00 || uchar > 0xDFFF) {
	//error, do nothing
	continue;
      }
      uchar &= 0x03ff;
      schar &= 0x03ff;
      uchar <<= 10;
      uchar |= schar;
      uchar += 0x10000;
      from++;
    }
    else {
      //error, do nothing
      continue;
    }
    
    if (uchar < 0x80) { 
      *to++ = uchar; 
    }
    else if (uchar < 0x800) { 
      *to++ = 0xc0 | (uchar >> 6); 
      *to++ = 0x80 | (uchar & 0x3f);
    }
    else if (uchar < 0x10000) {
      *to++ = 0xe0 | (uchar >> 12); 
      *to++ = 0x80 | (uchar >> 6) & 0x3f; 
      *to++ = 0x80 | uchar & 0x3f;
    }
    else if (uchar < 0x200000) {
      *to++ = 0xF0 | (uchar >> 18);
      *to++ = 0x80 | (uchar >> 12) & 0x3F; 
      *to++ = 0x80 | (uchar >> 6) & 0x3F; 
      *to++ = 0x80 | uchar & 0x3F;
    }
  }
  *to = 0;
  
  return 0;
}

