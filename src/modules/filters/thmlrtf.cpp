/***************************************************************************
                     thmlrtf.cpp  -  ThML to RTF filter
                             -------------------
    begin                : 1999-10-27
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
#include <thmlrtf.h>


ThMLRTF::ThMLRTF()
{
}


char ThMLRTF::ProcessText(char *text, int maxlen)
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
		  if (!strncmp(token, "sync type=\"Strongs\" value=\"G", 28) || \
		      !strncmp(token, "sync type=\"Strongs\" value=\"H", 28)) {
		    *to++ = '{';
		    *to++ = '\\';
		    *to++ = 'f';
		    *to++ = 's';
		    *to++ = '1';
		    *to++ = '5';
		    *to++ = ' ';
		    *to++ = '<';
		    for (unsigned int i = 28; token[i] != '\"'; i++)
		      *to++ = token[i];
		    *to++ = '>';
		    *to++ = '}';
		    continue;
		  }
		  else if (!strncmp(token, "sync type=\"Strongs\" value=\"T", 28)) {
		    *to++ = '{';
		    *to++ = '\\';
		    *to++ = 'f';
		    *to++ = 's';
		    *to++ = '1';
		    *to++ = '5';
		    *to++ = ' ';
		    *to++ = '(';
		    for (unsigned int i = 29; token[i] != '\"'; i++)
		      *to++ = token[i];
		    *to++ = ')';
		    *to++ = '}';
		    continue;
			}
		  else if (!strncmp(token, "note place=\"foot\"", 17)) {
		    *to++ = '{';
		    *to++ = '\\';
		    *to++ = 'i';
		    *to++ = '1';
		    *to++ = ' ';
		    *to++ = '\\';
		    *to++ = 'f';
		    *to++ = 's';
		    *to++ = '1';
		    *to++ = '5';
		    *to++ = ' ';
		    *to++ = '(';
		    continue;
		  }
		  else if (!strncmp(token, "/note", 5)) {
		    *to++ = ')';
		    *to++ = ' ';
		    *to++ = '}';
		    continue;
		  }
		  else if (!strncmp(token, "br", 2) || !strncmp(token, "BR", 2)) {
		    *to++ = '\\';
		    *to++ = 'l';
		    *to++ = 'i';
		    *to++ = 'n';
		    *to++ = 'e';
		    *to++ = ' ';
		    continue;
		  }
		  else switch(*token) {
		  case 'I':			// font tags
		  case 'i':
		    *to++ = '\\';
		    *to++ = 'i';
		    *to++ = '1';
		    *to++ = ' ';
		    continue;
		  case 'B':		// bold start
		  case 'b':
		    *to++ = '\\';
		    *to++ = 'b';
		    *to++ = '1';
		    *to++ = ' ';
		    continue;
		  case '/':
		    switch(token[1]) {
		    case 'P':
		    case 'p':
		      *to++ = '\\';
		      *to++ = 'p';
		      *to++ = 'a';
		      *to++ = 'r';
		      *to++ = ' ';
		      continue;
		    case 'I':
		    case 'i':		// italic end
		      *to++ = '\\';
		      *to++ = 'i';
		      *to++ = '0';
		      *to++ = ' ';
		      continue;
		    case 'B':		// bold start
		    case 'b':
		      *to++ = '\\';
		      *to++ = 'b';
		      *to++ = '0';
		      *to++ = ' ';
		      continue;
		    }
		  }
		}
		if (intoken)
			token[tokpos++] = *from;
		else	*to++ = *from;
	}
	*to = 0;
	return 0;
}



