/***************************************************************************
                          rwphtml.cpp  -  description
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
#include <rwphtml.h>


RWPHTML::RWPHTML()
{
}


char RWPHTML::ProcessText(char *text, int maxlen)
{
	char *to, *from;
	bool ingreek = false;
	bool inverse = false;
	int len;

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;							// -------------------------------
	for (to = text; *from; from++) {
		if (*from == '\\')
		{
			if(!ingreek) {
				ingreek = true;
				// don´t know what this means, find out later
				//*to++ = ' ';
				continue;
			}
			else {
				ingreek = false;
				continue;
			}
		}

		if ((ingreek) && ((*from == 'h') || (*from == 'H')))
			continue;		// 'h's are mostly useless in RWP translitterations.  The greek is more correct without them.

		if (*from == '#') {	// verse markings (e.g. "#Mark 1:1|")
			inverse = true;
			*to++ = '<';
			*to++ = 'F';
			*to++ = 'O';
			*to++ = 'N';
			*to++ = 'T';
			*to++ = ' ';
			*to++ = 'C';
			*to++ = 'O';
			*to++ = 'L';
			*to++ = 'O';
			*to++ = 'R';
			*to++ = '=';
			*to++ = '#';
			*to++ = '0';
			*to++ = '0';
			*to++ = '0';
			*to++ = '0';
			*to++ = 'F';
			*to++ = 'F';
			*to++ = '>';
			continue;
		}
		if ((*from == '|') && (inverse)) {
			inverse = false;
			*to++ = '<';
			*to++ = '/';
			*to++ = 'F';
			*to++ = 'O';
			*to++ = 'N';
			*to++ = 'T';
			*to++ = '>';
			continue;
		}
		
		if (*from == '{') {
			*to++ = '<';
			*to++ = 'B';
			*to++ = 'R';
			*to++ = '>';
			*to++ = '<';
			*to++ = 'S';
			*to++ = 'T';
			*to++ = 'R';
			*to++ = 'O';
			*to++ = 'N';
			*to++ = 'G';
			*to++ = '>';
			if ((from - &text[maxlen - len]) > 10) {	// not the beginning of the entry
				*to++ = '<';
				*to++ = 'P';
				*to++ = '>';
			}
			continue;
		}

		if (*from == '}')
		{
			// this is kinda neat... DO NOTHING
				*to++ = ' ';
				*to++ = '<';
				*to++ = '/';
				*to++ = 'S';
				*to++ = 'T';
				*to++ = 'R';
				*to++ = 'O';
				*to++ = 'N';
				*to++ = 'G';
				*to++ = '>';
			continue;
		}
		if ((*from == '\n') && (from[1] == '\n')) {
			*to++ = '<';
			*to++ = 'P';
			*to++ = '>';
			continue;
		}

		*to++ = *from;
	}
	*to = 0;
	return 0;
}


