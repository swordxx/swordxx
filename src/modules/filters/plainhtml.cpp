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
#include <plainhtml.h>


PLAINHTML::PLAINHTML()
{
}


char PLAINHTML::ProcessText(char *text, int maxlen)
{
	char *to, *from;
	int len;
	int count = 0;

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;							// -------------------------------
	for (to = text; *from; from++)
	{
		if ((*from == '\n') && (from[1] == '\n')) // paragraph
		{
			*to++ = '<';
			*to++ = 'P';
			*to++ = '>';
			from++;
			continue;
		} else {
			if ((*from == '\n')) // && (from[1] != '\n')) // new line
			{
				*to++ = '<';
				*to++ = 'B';
				*to++ = 'R';
				*to++ = '>';
				continue;
			}
		}
		
		if (*from == '{') {
			*to++ = '<';
			*to++ = 'S';
			*to++ = 'M';
			*to++ = 'A';
			*to++ = 'L';
			*to++ = 'L';
			*to++ = '>';
			*to++ = ' ';
			*to++ = '(';
			continue;
		}

		if (*from == '}')
		{
			*to++ = ')';
			*to++ = ' ';
			*to++ = '<';
			*to++ = '/';
			*to++ = 'S';
			*to++ = 'M';
			*to++ = 'A';
			*to++ = 'L';
			*to++ = 'L';
			*to++ = '>';
			continue;
		}
		if ((*from == ' ') && (count > 5000))
		{
			*to++ = '<';
			*to++ = 'W';
			*to++ = 'B';
			*to++ = 'R';
			*to++ = '>';
			count = 0;
			continue;
		}

		*to++ = *from;
		count++;
	}
	*to = 0;
	return 0;
}



