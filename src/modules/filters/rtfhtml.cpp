/***************************************************************************
                          rtfhtml.cpp  -  description
                             -------------------
    begin                : Wed Oct 13 1999
    copyright            : (C) 1999 by The team of BibleTime
    email                : info@bibletime.de
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
#include <rtfhtml.h>


RTFHTML::RTFHTML() {

}


char RTFHTML::ProcessText(char *text, int maxlen, const SWKey *key)
{
	char *to, *from;
	int len;
	bool center = false;

	len = strlen(text) + 1;						// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else	from = text;							// -------------------------------
	for (to = text; *from; from++) {
		if (*from == '\\') // a RTF command
		{
			if ((from[1] == 'p') && (from[2] == 'a') && (from[3] == 'r') && (from[4] == 'd'))
			{ // switch all modifier off
				if (center)
				{
					*to++ = '<';
					*to++ = '/';
					*to++ = 'C';
					*to++ = 'E';
					*to++ = 'N';
					*to++ = 'T';
					*to++ = 'E';
					*to++ = 'R';
					*to++ = '>';
					center = false;
				}
				from += 4;
				continue;
			}
			if ((from[1] == 'p') && (from[2] == 'a') && (from[3] == 'r'))
			{
				*to++ = '<';
				*to++ = 'P';
				*to++ = '>';
				*to++ = '\n';
				from += 3;
				continue;
			}
			if (from[1] == ' ')
			{
				from += 1;
				continue;
			}
			if ((from[1] == 'q') && (from[2] == 'c')) // center on
			{
				if (!center)
				{
					*to++ = '<';
					*to++ = 'C';
					*to++ = 'E';
					*to++ = 'N';
					*to++ = 'T';
					*to++ = 'E';
					*to++ = 'R';
					*to++ = '>';
					center = true;
				}
				from += 2;
				continue;
			}
		}

		*to++ = *from;
	}
	*to = 0;
	return 0;
}
