/******************************************************************************
 *  swlocale.h   - definition of Class SWLocale used for retrieval
 *				of locale lookups
 *
 * $Id: swlocale.h,v 1.15 2004/05/07 18:03:36 dglassey Exp $
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

#include <map>

#include <swconfig.h>

#include <versekey.h>
#ifdef SPLITLIB
#include <versekey2.h>
#endif

#include <defs.h>

SWORD_NAMESPACE_START
#define ENDOFABBREVS -2

typedef std::map < SWBuf, SWBuf, std::less < SWBuf > >LookupMap;

/** SWLocale is used for the localisation of the booknames
* The SWLocale is a class which holds the information of one language.
* Every language supported by Sword has one SWLocale object, 
* get the name of the Language using @see getname of this class.
* Another functions useful for frontend developers is @see getDescription.
*/
class SWDLLEXPORT SWLocale {
	LookupMap lookupTable;
	SWConfig *localeSource;
	char *name;
	char *description;
	char *encoding;
	struct abbrev *bookAbbrevs;
	char *BMAX;
	struct sbook **books;
	#ifdef SPLITLIB
	struct sbook2 **books2;
	struct abbrev2 *bookAbbrevs2;
	#endif

public:
	SWLocale(const char *ifilename);
	virtual ~SWLocale();

	/**
	* This function is used to get the name of the languages which this object is handling.
	* @return The name of the managed language. A possible example is "de".
	*/
	virtual const char *getName();
	/**
	* @return The description. A possible example is "German".
	*/
	virtual const char *getDescription();
	virtual const char *getEncoding();
	virtual const char *translate(const char *text);
	virtual void augment(SWLocale &addFrom);
	virtual SWLocale & operator +=(SWLocale &addFrom) { augment(addFrom); return *this; }
	virtual const struct abbrev *getBookAbbrevs();
	virtual void getBooks(char **iBMAX, struct sbook ***ibooks);
	#ifdef SPLITLIB
	virtual void getBooks2(struct sbook2 ***ibooks, VerseKey2 *vk);
	virtual const struct abbrev2 *getBookAbbrevs2();
	#endif
};

SWORD_NAMESPACE_END
#endif
