/***************************************************************************
                     gbfthml.cpp  -  GBF to ThML filter
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
#include <gbfthml.h>


GBFThML::GBFThML()
{
}


char GBFThML::ProcessText(char *text, int maxlen)
{
	char *to, *from, token[20], tokpos = 0;
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
			memset(token, 0, 20);
			continue;
		}
		if (*from == '>')
		{
			intoken = false;
			// process desired tokens
			switch (*token) {
			case 'W':	// Strongs
			  switch(token[1]) {
			  case 'G':
			  case 'H':			    
			    *to++ = '<';
			    *to++ = 's';
			    *to++ = 'y';
			    *to++ = 'n';
			    *to++ = 'c';
			    *to++ = ' ';
			    *to++ = 't';
			    *to++ = 'y';
			    *to++ = 'p';
			    *to++ = 'e';
			    *to++ = '=';
			    *to++ = '"';
			    *to++ = 'S';
			    *to++ = 't';
			    *to++ = 'r';
			    *to++ = 'o';
			    *to++ = 'n';
			    *to++ = 'g';
			    *to++ = 's';
			    *to++ = '"';
			    *to++ = ' ';
			    *to++ = 'v';
			    *to++ = 'a';
			    *to++ = 'l';
			    *to++ = 'u';
			    *to++ = 'e';
			    *to++ = '=';
			    *to++ = '"';
			    for (unsigned int i = 1; i < strlen(token); i++)
			      *to++ = token[i];
			    *to++ = '"';
			    *to++ = ' ';
			    *to++ = '/';
			    *to++ = '>';
			    continue;
			    
			  case 'T':               // Tense
			    *to++ = ' ';
			    *to++ = '<';
			    *to++ = 's';
			    *to++ = 'm';
			    *to++ = 'a';
			    *to++ = 'l';
			    *to++ = 'l';
			    *to++ = '>';
			    *to++ = '<';
			    *to++ = 'i';
			    *to++ = '>';
			    for (unsigned int i = 3; i < strlen(token); i++)
			      *to++ = token[i];
			    *to++ = '<';
			    *to++ = '/';
			    *to++ = 'i';
			    *to++ = '>';
			    *to++ = '<';
			    *to++ = '/';
			    *to++ = 's';
			    *to++ = 'm';
			    *to++ = 'a';
			    *to++ = 'l';
			    *to++ = 'l';
			    *to++ = '>';
			    *to++ = ' ';
			    continue;
				}
			  break;
			case 'R':
			  switch(token[1])
			    {
			    case 'F':               // footnote begin
			      *to++ = '<';
			      *to++ = 'n';
			      *to++ = 'o';
			      *to++ = 't';
			      *to++ = 'e';
			      *to++ = ' ';
			      *to++ = 'p';
			      *to++ = 'l';
			      *to++ = 'a';
			      *to++ = 'c';
			      *to++ = 'e';
			      *to++ = '=';
			      *to++ = '"';
			      *to++ = 'f';
			      *to++ = 'o';
			      *to++ = 'o';
			      *to++ = 't';
			      *to++ = '"';
			      *to++ = '>';
			      continue;
			    case 'f':               // footnote end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'n';
			      *to++ = 'o';
			      *to++ = 't';
			      *to++ = 'e';
			      *to++ = '>';
			      continue;
			    }
			  break;
			case 'F':			// font tags
			  switch(token[1])
			    {
			    case 'N':
			      *to++ = '<';
			      *to++ = 'f';
			      *to++ = 'o';
			      *to++ = 'n';
			      *to++ = 't';
			      *to++ = ' ';
			      *to++ = 'f';
			      *to++ = 'a';
			      *to++ = 'c';
			      *to++ = 'e';
			      *to++ = '=';
			      *to++ = '\"';		
			      for (unsigned int i = 2; i < strlen(token); i++)
				*to++ = token[i];
			      *to++ = '\"';
			      *to++ = '>';
			      continue;
			    case 'n':
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'f';
			      *to++ = 'o';
			      *to++ = 'n';
			      *to++ = 't';
			      *to++ = '>';
			      continue;
			    case 'I':		// italic start
			      *to++ = '<';
			      *to++ = 'i';
			      *to++ = '>';
			      continue;
			    case 'i':		// italic end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'i';
			      *to++ = '>';
			      continue;
			    case 'B':		// bold start
			      *to++ = '<';
			      *to++ = 'b';
			      *to++ = '>';
			      continue;
			    case 'b':		// bold end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'b';
			      *to++ = '>';
			      continue;

			    case 'R':		// words of Jesus begin
			      *to++ = '<';
			      *to++ = 'f';
			      *to++ = 'o';
			      *to++ = 'n';
			      *to++ = 't';
			      *to++ = ' ';
			      *to++ = 'c';
			      *to++ = 'o';
			      *to++ = 'l';
			      *to++ = 'o';
			      *to++ = 'r';
			      *to++ = '=';
			      *to++ = '#';
			      *to++ = 'f';
			      *to++ = 'f';
			      *to++ = '0';
			      *to++ = '0';
			      *to++ = '0';
			      *to++ = '0';
			      *to++ = '>';
			      continue;
			    case 'r':		// words of Jesus end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'f';
			      *to++ = 'o';
			      *to++ = 'n';
			      *to++ = 't';
			      *to++ = '>';
			      continue;
			    case 'U':		// Underline start
			      *to++ = '<';
			      *to++ = 'u';
			      *to++ = '>';
			      continue;
			    case 'u':		// Underline end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'u';
			      *to++ = '>';
			      continue;
			    case 'O':		// Old Testament quote begin
			      *to++ = '<';
			      *to++ = 'c';
			      *to++ = 'i';
			      *to++ = 't';
			      *to++ = 'e';
			      *to++ = '>';
			      continue;
			    case 'o':		// Old Testament quote end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'c';
			      *to++ = 'i';
			      *to++ = 't';
			      *to++ = 'e';
			      *to++ = '>';
			      continue;
			    case 'S':		// Superscript begin
			      *to++ = '<';
			      *to++ = 's';
			      *to++ = 'u';
			      *to++ = 'p';
			      *to++ = '>';
			      continue;
			    case 's':		// Superscript end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 's';
			      *to++ = 'u';
			      *to++ = 'p';
			      *to++ = '>';
			      continue;
			    case 'V':		// Subscript begin
			      *to++ = '<';
			      *to++ = 's';
			      *to++ = 'u';
			      *to++ = 'b';
			      *to++ = '>';
			      continue;
			    case 'v':		// Subscript end
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 's';
			      *to++ = 'u';
			      *to++ = 'b';
			      *to++ = '>';
			      continue;
			    }
			  break;
			case 'C':			// special character tags
			  switch(token[1])
				{
				case 'A':               // ASCII value
				  *to++ = (char)atoi(&token[2]);
				  continue;
				case 'G':
				  //*to++ = ' ';
				  continue;
				case 'L':               // line break
				  *to++ = '<';
				  *to++ = 'b';
				  *to++ = 'r';
				  *to++ = '>';
				  *to++ = ' ';
				  continue;
				case 'M':               // new paragraph
				  *to++ = '<';
				  *to++ = 'p';
				  *to++ = '>';
				  continue;
				case 'T':
				  //*to++ = ' ';
				  continue;
				}
			  break;
			case 'P':			// special formatting
			  switch(token[1])
			    {
			    case 'P':               // Poetry begin
			      *to++ = '<';
			      *to++ = 'v';
			      *to++ = 'e';
			      *to++ = 'r';
			      *to++ = 's';
			      *to++ = 'e';
			      *to++ = '>';
			      continue;
			    case 'p':
			      *to++ = '<';
			      *to++ = '/';
			      *to++ = 'v';
			      *to++ = 'e';
			      *to++ = 'r';
			      *to++ = 's';
			      *to++ = 'e';
			      *to++ = '>';
			      continue;
			    }
			  break;
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



