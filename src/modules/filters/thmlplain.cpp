/******************************************************************************
 *
 * thmlplain -	SWFilter decendant to strip out all ThML tags or convert to
 *		ASCII rendered symbols.
 */


#include <stdlib.h>
#include <string.h>
#include <thmlplain.h>


ThMLPlain::ThMLPlain() {
}


char ThMLPlain::ProcessText(char *text, int maxlen)
{
  char *to, *from, token[50], tokpos = 0;
  bool intoken = false;
  int len;

  len = strlen(text) + 1;						// shift string to right of buffer
  if (len < maxlen) {
    memmove(&text[maxlen - len], text, len);
    from = &text[maxlen - len];
  }
  else	from = text;							// -------------------------------
	
  for (to = text; *from; from++) {
    if (*from == '<') {
      intoken = true;
      tokpos = 0;
      memset(token, 0, 50);
      continue;
    }
    if (*from == '>') {
      intoken = false;
      // process desired tokens

      if (!strncmp("note", token, 4)) {
	*to++ = ' ';
	*to++ = '(';
      }
      else if (!strncmp("br", token, 2))
	*to++ = '\n';
      else if (!strncmp("/p", token, 2))
	*to++ = '\n';
      else if (!strncmp("/note", token, 5)) {
	*to++ = ')';
	*to++ = ' ';
      }
      continue;
    }
    if (intoken)
      token[tokpos++] = *from;
    else	*to++ = *from;
  }
  *to = 0;
  return 0;
}
