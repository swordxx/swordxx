/***************************************************************************
                          plainfootnotes.cpp  -  description
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

#include <plainfootnotes.h>
#include <swkey.h>

#include <stdlib.h>
#include <string.h>
#ifndef __GNUC__
#else
#include <unixstr.h>
#endif

const char PLAINFootnotes::on[] = "On";
const char PLAINFootnotes::off[] = "Off";
const char PLAINFootnotes::optName[] = "Footnotes";
const char PLAINFootnotes::optTip[] = "Toggles Footnotes On and Off In Bible Texts If They Exist";

PLAINFootnotes::PLAINFootnotes(){
	option = false;
	options.push_back(on);
	options.push_back(off);
}

PLAINFootnotes::~PLAINFootnotes(){
}


void PLAINFootnotes::setOptionValue(const char *ival)
{
	option = (!stricmp(ival, on));
}

const char *PLAINFootnotes::getOptionValue()
{
	return (option) ? on:off;
}


char PLAINFootnotes::ProcessText(char *text, int maxlen, const SWKey *key)
{
	char *to, *from, token[20], tokpos = 0;
	bool intoken 	= false;
	bool lastspace = false;

	if (!option) {	// if we don't want footnotes
		char *to, *from;
		int len;
		bool hide = false;

		len = strlen(text) + 1;	// shift string to right of buffer
		if (len < maxlen)
		{
			memmove(&text[maxlen - len], text, len);
			from = &text[maxlen - len];
		}
		else	from = text;	// -------------------------------

		for (to = text; *from; from++) {
			if (*from == '{') // Footnote start
			{
				hide = true;
				continue;
			}
			if (*from == '}') // Footnote end
			{
				hide=false;
				continue;
			}
			if (intoken)
				token[tokpos++] = *from;
			else	{
				if (!hide) {
					*to++ = *from;
					lastspace = (*from == ' ');
				}
			}
		}
		*to = 0;
	}
	return 0;
}

