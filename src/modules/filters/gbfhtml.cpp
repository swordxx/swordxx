/***************************************************************************
                          gbfhtml.cpp  -  description
                             -------------------
    begin                : Thu Jun 24 1999
    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
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
#include <gbfhtml.h>


GBFHTML::GBFHTML()
{
}


char GBFHTML::ProcessText(char *text, int maxlen, const SWKey *key, const SWModule *module)
{
	char *to, *from, token[2048];
	int tokpos = 0;
	bool intoken 	= false;
	bool hasFootnotePreTag = false;
	bool isRightJustified = false;
	bool isCentered = false;
	int len;

	len = strlen(text) + 1;		// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else
		from = text;			// -------------------------------
	
	for (to = text; *from; from++)
	{
		if (*from == '\n') {
			*from = ' ';
		}			
		if (*from == '<') {
			intoken = true;
			tokpos = 0;
			token[0] = 0;
			token[1] = 0;
			token[2] = 0;
			continue;
		}
		if (*from == '>') {
			intoken = false;
			// process desired tokens
			switch (*token) {
				case 'W':	// Strongs
					switch(token[1])
					{
						case 'G':               // Greek
						case 'H':               // Hebrew
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
							*to++ = 'e';
							*to++ = 'm';
							*to++ = '>';
							for (char *tok = token+2; *tok; tok++)
								*to++ = *tok;
							*to++ = '<';
							*to++ = '/';
							*to++ = 'e';
							*to++ = 'm';
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
					  case 'B':								//word(s) explained in footnote
							*to++ = '<';
							*to++ = 'i';
							*to++ = '>';
							hasFootnotePreTag = true; //we have the RB tag
							continue;
						case 'F':               // footnote begin
							if (hasFootnotePreTag) {
								*to++ = '<';
								*to++ = '/';
								*to++ = 'i';
								*to++ = '>';
								*to++ = ' ';
							}
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
							*to++ = '\"';
							*to++ = '#';
							*to++ = '8';
							*to++ = '0';
							*to++ = '0';
							*to++ = '0';
							*to++ = '0';
							*to++ = '0';
							*to++ = '\"';
							*to++ = '>';

							*to++ = ' ';
							*to++ = '<';
							*to++ = 's';
							*to++ = 'm';
							*to++ = 'a';
							*to++ = 'l';
							*to++ = 'l';
							*to++ = '>';
							*to++ = '(';

							continue;
						case 'f':               // footnote end
							*to++ = ')';
							*to++ = '<';
							*to++ = '/';
							*to++ = 's';
							*to++ = 'm';
							*to++ = 'a';
							*to++ = 'l';
							*to++ = 'l';
							*to++ = '>';
							*to++ = ' ';
							*to++ = '<';
							*to++ = '/';
							*to++ = 'f';
							*to++ = 'o';
							*to++ = 'n';
							*to++ = 't';
							*to++ = '>';
							hasFootnotePreTag = false;
							continue;
					}
					break;

				case 'F':			// font tags
					switch(token[1])
					{
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
							*to++ = 'F';
							*to++ = 'F';
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
							*to++ = '"';
						        for (char *tok = token + 2; *tok; tok++)
								*to++ = *tok;
							*to++ = '"';
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
							*to++ = ' ';
							*to++ = '/';
							*to++ = '>';
							*to++ = ' ';
							continue;
						case 'M':               // new paragraph
							*to++ = '<';
							*to++ = 'b';
							*to++ = 'r';
							*to++ = ' ';
							*to++ = '/';
							*to++ = '>';
							continue;
						case 'T':
							//*to++ = ' ';
							continue;
					}
					break;
				case 'J':	//Justification
					switch(token[1])
					{
						case 'R':	//right
							*to++ = '<';
							*to++ = 'd';
							*to++ = 'i';
							*to++ = 'v';
							*to++ = ' ';
							*to++ = 'a';
							*to++ = 'l';
							*to++ = 'i';
							*to++ = 'g';
							*to++ = 'n';
							*to++ = '=';
							*to++ = '\"';
							*to++ = 'r';
							*to++ = 'i';
							*to++ = 'g';
							*to++ = 'h';
							*to++ = 't';
							*to++ = '\"';
							*to++ = '>';
							isRightJustified = true;
							continue;

						case 'C':	//center
							*to++ = '<';
							*to++ = 'd';
							*to++ = 'i';
							*to++ = 'v';
							*to++ = ' ';
							*to++ = 'a';
							*to++ = 'l';
							*to++ = 'i';
							*to++ = 'g';
							*to++ = 'n';
							*to++ = '=';
							*to++ = '\"';
							*to++ = 'c';
							*to++ = 'e';
							*to++ = 'n';
							*to++ = 't';
							*to++ = 'e';
							*to++ = 'r';
							*to++ = '\"';
							*to++ = '>';
							isCentered = true;
							continue;

						case 'L': //left, reset right and center
							if (isCentered) {
								*to++ = '<';
								*to++ = '/';
								*to++ = 'c';
								*to++ = 'e';
								*to++ = 'n';
								*to++ = 't';
								*to++ = 'e';
								*to++ = 'r';
								*to++ = '>';
								isCentered = false;
							}
							if (isRightJustified) {
								*to++ = '<';
								*to++ = '/';
								*to++ = 'd';
								*to++ = 'i';
								*to++ = 'v';
								*to++ = '>';
								isRightJustified = false;
							}
							continue;
					}
					break;
				case 'T':			// title formatting
					switch(token[1])
					{
						case 'T':               // Book title begin
							*to++ = '<';
							*to++ = 'b';
							*to++ = 'i';
							*to++ = 'g';
							*to++ = '>';
							continue;
						case 't':
							*to++ = '<';
							*to++ = '/';
							*to++ = 'b';
							*to++ = 'i';
							*to++ = 'g';
							*to++ = '>';
							continue;/*
                    				case 'S':
						        *to++ = '<';
						        *to++ = 'b';
						        *to++ = 'r';
						        *to++ = ' ';
						        *to++ = '/';
						        *to++ = '>';
						        *to++ = '<';
						        *to++ = 'b';
						        *to++ = 'i';
							*to++ = 'g';
						        *to++ = '>';
							continue;
                    				case 's':
						        *to++ = '<';
						        *to++ = '/';
						        *to++ = 'b';
						        *to++ = 'i';
						        *to++ = 'g';
						        *to++ = '>';
						        *to++ = '<';
						        *to++ = 'b';
						        *to++ = 'r';
						        *to++ = ' ';
						        *to++ = '/';
						        *to++ = '>';
							continue;*/
					}
					break;
	
				case 'P': // special formatting
					switch(token[1])
					{
						case 'P': // Poetry begin
							*to++ = '<';
							*to++ = 'c';
							*to++ = 'i';
							*to++ = 't';
							*to++ = 'e';
							*to++ = '>';
							continue;
						case 'p':
							*to++ = '<';
							*to++ = '/';
							*to++ = 'c';
							*to++ = 'i';
							*to++ = 't';
							*to++ = 'e';
							*to++ = '>';
							continue;
					}
					break;
			}
			continue;
		}
		if (intoken) {
		 	if (tokpos < 2045) {
		 		token[tokpos++] = *from;
		 		token[tokpos+2] = 0;
		 	}
		 }
		else
			*to++ = *from;
	}
	*to++ = 0;
	*to = 0;
	return 0;
}
