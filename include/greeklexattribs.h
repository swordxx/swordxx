/*
 *
 * thmlplain.h
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef GREEKLEXATTRIBS_H
#define GREEKLEXATTRIBS_H

#include <swfilter.h>

#include <defs.h>

  /** this filter converts ThML text to plain text
  */
class SWDLLEXPORT GreekLexAttribs:public SWFilter {
public:
	GreekLexAttribs();
	virtual char ProcessText(char *text, int maxlen, const SWKey *, const SWModule * = 0);
};

#endif
