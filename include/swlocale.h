/******************************************************************************
 *  swlocale.h   - definition of Class SWLocale used for retrieval
 *				of locale lookups
 *
 * $Id: swlocale.h,v 1.3 2000/03/13 09:36:02 scribe Exp $
 *
 * Copyright 2000 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWLOCALE_H
#define SWLOCALE_H

#include <string>
#include <map>

#include <swconfig.h>
#include <versekey.h>


using namespace std;

typedef map <string, string, less <string> > LookupMap;

class SWLocale {
	LookupMap lookupTable;
	SWConfig *localeSource;
	char *name;
	char *description;
	struct abbrev *bookAbbrevs;
	char *BMAX;
	struct sbook **books;

public:
	SWLocale(const char *ifilename);
	virtual ~SWLocale();

	virtual const char *getName();
	virtual const char *getDescription();
	virtual const char *translate(const char *text);
	virtual SWLocale &operator +=(SWLocale &addFrom);
	virtual const struct abbrev *getBookAbbrevs();
	virtual void getBooks(char **iBMAX, struct sbook ***ibooks);
};

#endif
