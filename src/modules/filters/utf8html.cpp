/******************************************************************************
 *
 * utf8html -	SWFilter decendant to convert a UTF-8 stream to HTML escapes
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <utf8html.h>

UTF8HTML::UTF8HTML() {
}


char UTF8HTML::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	unsigned char *to, *from;
	int len;
        char digit[10];
        unsigned long ch;
	 if ((unsigned long)key < 2)	// hack, we're en(1)/de(0)ciphering
		return -1;

	len = strlenw(text) + 2;						// shift string to right of buffer
	if (len < maxlen) {
	        memmove(&text[maxlen - len], text, len);
		from = (unsigned char*)&text[maxlen - len];
	}
	else	from = (unsigned char*)text;
	// -------------------------------
	for (to = (unsigned char*)text; *from; from++) {
	  ch = 0;
          if ((*from & 128) != 128) {
//          	if (*from != ' ')
	       *to++ = *from;
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
          *to++ = '&';
          *to++ = '#';
	  sprintf(digit, "%d", ch);
		for (char *dig = digit; *dig; dig++)
			*to++ = *dig;
		*to++ = ';';
	}
	*to++ = 0;
	*to = 0;
	return 0;
}
