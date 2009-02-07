/***************************************************************************
 *                         plainfootnotes.cpp  -  description
 *                            -------------------
 *   begin                : Wed Oct 13 1999
 *   copyright            : (C) 1999 by The team of BibleTime
 *   email                : info@bibletime.de
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#include <plainfootnotes.h>
#include <swkey.h>

#include <stdlib.h>

SWORD_NAMESPACE_START

const char oName[] = "Footnotes";
const char oTip[] = "Toggles Footnotes On and Off In Bible Texts If They Exist";

const SWBuf choices[3] = {"Off", "On", ""};
const StringList oValues(&choices[0], &choices[2]);

PLAINFootnotes::PLAINFootnotes() : SWOptionFilter(oName, oTip, &oValues) {
	setOptionValue("Off");
}

PLAINFootnotes::~PLAINFootnotes(){
}


char PLAINFootnotes::processText(SWBuf &text, const SWKey *key, const SWModule *module) {
	if (!option) {	// if we don't want footnotes
		//char token[2048];
		//SWBuf token;
		//int tokpos = 0;
		//bool intoken 	= false;
		//bool lastspace = false;

		bool hide = false;

		SWBuf orig = text;
		const char *from = orig.c_str();
		for (text = ""; *from; from++) {
		 	if (*from == '{') // Footnote start
			{
				hide = true;
				continue;
			}
			else if (*from == '}') // Footnote end
			{
				hide = false;
				continue;
			}
			
			//if (intoken) {
				//if (tokpos < 2045)
			//		token += *from;
				//	token[tokpos+2] = 0;
			//}
			//else	{
				if (!hide) {
					text = *from;
					//lastspace = (*from == ' ');
				}
			//}
		}
	}
	return 0;
}

SWORD_NAMESPACE_END
