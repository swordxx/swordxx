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

#include <plainhtml.h>

SWORD_NAMESPACE_START

PLAINHTML::PLAINHTML()
{
}


char PLAINHTML::processText(SWBuf &text, const SWKey *key, const SWModule *module)
{
	int count = 0;

	const char *from;
	SWBuf orig = text;
	from = orig.c_str();
	for (text = ""; *from; from++)
	{
		if ((*from == '\n') && (from[1] == '\n')) // paragraph
		{
			text += "<P>";
			from++;
			continue;
		} else {
			if ((*from == '\n')) // && (from[1] != '\n')) // new line
			{
				text += "<BR>";
				continue;
			}
		}
		
		if (*from == '{') {
			text += "<FONT COLOR=#80000><SMALL> (";
			continue;
		}

		if (*from == '}')
		{
			text += ") </SMALL></FONT>";
			continue;
		}

		if ((*from == ' ') && (count > 5000))
		{
			text += "<WBR>";
			count = 0;
			continue;
		}

		text += *from;
		count++;
	}
	return 0;
}

SWORD_NAMESPACE_END
