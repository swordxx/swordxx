/***************************************************************************
                     thmlgbf.cpp  -  ThML to GBF filter
                             -------------------
    begin                : 1999-10-28
    copyright            : 1999 by Chris Little
    email                : chrislit@chiasma.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <thmlgbf.h>


ThMLGBF::ThMLGBF()
{
}


char ThMLGBF::ProcessText(char *text, int maxlen)
{
  char *to, *from, token[50], tokpos = 0;
  bool intoken 	= false;
  int len;
  
  len = strlen(text) + 1;						// shift string to right of buffer
  if (len < maxlen) {
    memmove(&text[maxlen - len], text, len);
    from = &text[maxlen - len];
  }
  else	from = text;							// -------------------------------
  for (to = text; *from; from++)
    {
      if (*from == '<') {
	intoken = true;
	tokpos = 0;
	memset(token, 0, 50);
	continue;
      }
      if (*from == '>')
	{
	  intoken = false;
	  // process desired tokens
	  if (!strncmp(token, "sync type=\"Strongs\" value=\"G", 28) || !strncmp(token, "sync type=\"Strongs\" value=\"H", 28)) {
	      *to++ = '<';
	      *to++ = 'W';
	      for (unsigned int i = 27; token[i] != '\"'; i++)
		*to++ = token[i];
	      *to++ = '>';
	      continue;
	      }
	  else if (!strncmp(token, "note place=\"foot\"", 17)) {
	    *to++ = '<';
	    *to++ = 'R';
	    *to++ = 'F';
	    *to++ = '>';
	    continue;
	  }
	  else if (!strncmp(token, "/note", 5)) {
	    *to++ = '<';
	    *to++ = 'R';
	    *to++ = 'f';
	    *to++ = '>';
	    continue;
	  }
	  else if (!strncmp(token, "foreign lang=\"el\"", 17)) {
	    *to++ = '<';
	    *to++ = 'F';
	    *to++ = 'N';
	    *to++ = 'S';
	    *to++ = 'I';
	    *to++ = 'L';
	    *to++ = ' ';
	    *to++ = 'G';
	    *to++ = 'a';
	    *to++ = 'l';
	    *to++ = 'a';
	    *to++ = 't';
	    *to++ = 'i';
	    *to++ = 'a';
	    *to++ = '>';
	    continue;
	  }
	  else if (!strncmp(token, "foreign lang=\"he\"", 17)) {
	    *to++ = '<';
	    *to++ = 'F';
	    *to++ = 'N';
	    *to++ = 'S';
	    *to++ = 'I';
	    *to++ = 'L';
	    *to++ = ' ';
	    *to++ = 'E';
	    *to++ = 'z';
	    *to++ = 'r';
	    *to++ = 'a';
	    *to++ = '>';
	    continue;
	  }
	  else if (!strncmp(token, "/foreign", 8)) {
	    *to++ = '<';
	    *to++ = 'F';
	    *to++ = 'n';
	    *to++ = '>';
	    continue;				
	  }
	  else if (!strncmp(token, "br", 2) || !strncmp(token, "BR", 2)) {
		    *to++ = '<';
		    *to++ = 'C';
		    *to++ = 'L';
		    *to++ = '>';
		    continue;
		  }
	  else switch(*token) {
	          case 'I':			// font tags
		  case 'i':
		    *to++ = '<';
		    *to++ = 'F';
		    *to++ = 'I';
		    *to++ = '>';
		    continue;
		  case 'B':		// bold start
		  case 'b':
		    *to++ = '<';
		    *to++ = 'F';
		    *to++ = 'B';
		    *to++ = '>';
		    continue;
		  case '/':
		    switch(token[1]) {
		    case 'P':
		    case 'p':
		      *to++ = '<';
		      *to++ = 'C';
		      *to++ = 'M';
		      *to++ = '>';
		      continue;
		    case 'I':
		    case 'i':		// italic end
		      *to++ = '<';
		      *to++ = 'F';
		      *to++ = 'i';
		      *to++ = '>';
		      continue;
		    case 'B':		// bold start
		    case 'b':
		      *to++ = '<';
		      *to++ = 'F';
		      *to++ = 'b';
		      *to++ = '>';
		      continue;
		    }
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



