/******************************************************************************
 *
 * Latin1UTF16 -	SWFilter decendant to convert a Latin-1 character to UTF-16
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <latin1utf16.h>

Latin1UTF16::Latin1UTF16() {
}


char Latin1UTF16::ProcessText(char *text, int maxlen, const SWKey *key)
{
    unsigned char *from;
    unsigned short *to;
    int len;
    
    len = strlen(text) + 1;						// shift string to right of buffer
    if (len < maxlen) {
      memmove(&text[maxlen - len], text, len);
      from = (unsigned char*)&text[maxlen - len];
    }
    else	
      from = (unsigned char*)text;
    // -------------------------------
    
    for (to = (unsigned short*)text; *from; from++) {
      *to++ = (unsigned short)*from;
    }
    *to = 0;
    return 0;
}
