/******************************************************************************
 *  swlocale.cpp   - implementation of Class SWLocale used for retrieval
 *				of locale lookups
 *
 * $Id: swlocale.cpp,v 1.3 2000/03/13 09:36:03 scribe Exp $
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

#include <swlocale.h>
#include <utilfuns.h>


SWLocale::SWLocale(const char * ifilename) {
	ConfigEntMap::iterator confEntry;

	name         = 0;
	description  = 0;
	bookAbbrevs  = 0;
	BMAX         = 0;
	books        = 0;
	localeSource = new SWConfig(ifilename);

	confEntry = localeSource->Sections["Meta"].find("Name");
	if (confEntry != localeSource->Sections["Meta"].end())
		stdstr(&name, (*confEntry).second.c_str());
	
	confEntry = localeSource->Sections["Meta"].find("Description");
	if (confEntry != localeSource->Sections["Meta"].end())
		stdstr(&description, (*confEntry).second.c_str());
}


SWLocale::~SWLocale() {

	delete localeSource;

	if (description)
		delete [] description;

	if (name)
		delete [] name;

	if (bookAbbrevs)
		delete [] bookAbbrevs;

	if (BMAX) {
		for (int i = 0; i < 2; i++)
			delete [] books[i];
		delete [] BMAX;
		delete [] books;
	}
}


const char *SWLocale::translate(const char *text) {
	LookupMap::iterator entry;

	entry = lookupTable.find(text);

	if (entry == lookupTable.end()) {
		ConfigEntMap::iterator confEntry;
		confEntry = localeSource->Sections["Text"].find(text);
		if (confEntry == localeSource->Sections["Text"].end())
			lookupTable.insert(LookupMap::value_type(text, text));
		else	lookupTable.insert(LookupMap::value_type(text, (*confEntry).second.c_str()));
		entry = lookupTable.find(text);
	}
	return (*entry).second.c_str();
}


const char *SWLocale::getName() {
	return name;
}


const char *SWLocale::getDescription() {
	return description;
}


SWLocale &SWLocale::operator +=(SWLocale &addFrom) {
	*localeSource += *addFrom.localeSource;
	return *this;
}


const struct abbrev *SWLocale::getBookAbbrevs() {
	static const char *nullstr = "";
	if (!bookAbbrevs) {
		ConfigEntMap::iterator it;
		int i;
		int size = localeSource->Sections["Book Abbrevs"].size();
		bookAbbrevs = new struct abbrev[size + 1];
		for (i = 0, it = localeSource->Sections["Book Abbrevs"].begin(); it != localeSource->Sections["Book Abbrevs"].end(); it++, i++) {
			bookAbbrevs[i].ab = (*it).first.c_str();
			bookAbbrevs[i].book = atoi((*it).second.c_str());
		}
		bookAbbrevs[i].ab = nullstr;
		bookAbbrevs[i].book = -1;
	}
		
	return bookAbbrevs;
}


void SWLocale::getBooks(char **iBMAX, struct sbook ***ibooks) {
	if (!BMAX) {
		BMAX = new char [2];
		BMAX[0] = VerseKey::builtin_BMAX[0];
		BMAX[1] = VerseKey::builtin_BMAX[1];

		books = new struct sbook *[2];
		books[0] = new struct sbook[BMAX[0]];
		books[1] = new struct sbook[BMAX[1]];

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < BMAX[i]; j++) {
				books[i][j] = VerseKey::builtin_books[i][j];
				books[i][j].name = translate(VerseKey::builtin_books[i][j].name);
			}
		}
	}

	*iBMAX  = BMAX;
	*ibooks = books;
}
