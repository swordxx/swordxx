/******************************************************************************
 *  swlocale.cpp   - implementation of Class SWLocale used for retrieval
 *				of locale lookups
 *
 * $Id: swlocale.cpp,v 1.11 2004/05/07 18:11:24 dglassey Exp $
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

SWORD_NAMESPACE_START

SWLocale::SWLocale(const char * ifilename) {
	ConfigEntMap::iterator confEntry;

	name         = 0;
	description  = 0;
	encoding     = 0;
	bookAbbrevs  = 0;
	BMAX         = 0;
	books        = 0;
	#ifdef SPLITLIB
	books2        = 0;
	bookAbbrevs2  = 0;
	#endif
	localeSource = new SWConfig(ifilename);

	confEntry = localeSource->Sections["Meta"].find("Name");
	if (confEntry != localeSource->Sections["Meta"].end())
		stdstr(&name, (*confEntry).second.c_str());
	
	confEntry = localeSource->Sections["Meta"].find("Description");
	if (confEntry != localeSource->Sections["Meta"].end())
		stdstr(&description, (*confEntry).second.c_str());

	confEntry = localeSource->Sections["Meta"].find("Encoding"); //Either empty (==Latin1) or UTF-8
	if (confEntry != localeSource->Sections["Meta"].end())
		stdstr(&encoding, (*confEntry).second.c_str());
}


SWLocale::~SWLocale() {

	delete localeSource;

	if (encoding)
		delete [] encoding;
	
	if (description)
		delete [] description;

	if (name)
		delete [] name;

	if (bookAbbrevs)
		delete [] bookAbbrevs;

	#ifdef SPLITLIB
	if (bookAbbrevs2)
		delete [] bookAbbrevs2;
	#endif
	
	if (BMAX) {
		#ifndef VK2
		for (int i = 0; i < 2; i++)
			delete [] books[i];
		#endif
		delete [] BMAX;
		delete [] books;
		#ifdef SPLITLIB
		delete [] books2;
		#endif
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
		else {//valid value found
			/*
			- If Encoding==Latin1 and we have a StringHelper, convert to UTF-8
			- If StringHelper present and Encoding is UTF-8, use UTF8
			- If StringHelper not present and Latin1, use Latin1
			- If StringHelper not present and UTF-8, no idea what to do. Should't happen
			*/
/*			if (StringHelper::getSystemStringHelper()) {
				if (!strcmp(encoding, "UTF-8")) {
					lookupTable.insert(LookupMap::value_type(text, (*confEntry).second.c_str()));
				}
				else { //latin1 expected, convert to UTF-8
					SWBuf t((*confEntry).second.c_str());
					t = StringHelper::getSystemStringHelper()->latin2unicode( t );
					
					lookupTable.insert(LookupMap::value_type(text, t.c_str()));
				}
			}
			else { //no stringhelper, just insert. Nothing we can do*/
				lookupTable.insert(LookupMap::value_type(text, (*confEntry).second.c_str()));
// 			}
			
		}
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

const char *SWLocale::getEncoding() {
	return encoding;
}

void SWLocale::augment(SWLocale &addFrom) {
	*localeSource += *addFrom.localeSource;
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
			#ifdef NONNUMERICLOCALETHING
			bookAbbrevs[i].book = VerseKey::getOSISBookNum((*it).second.c_str());
			#else
			bookAbbrevs[i].book = atoi((*it).second.c_str());
			#endif
			//printf("SWLocale::getBookAbbrevs %s:%s %d\n",bookAbbrevs[i].ab,
			//	(*it).second.c_str(), bookAbbrevs[i].book); 
		}
		bookAbbrevs[i].ab = nullstr;
		bookAbbrevs[i].book = -1;
	}
		
	return bookAbbrevs;
}


#ifdef SPLITLIB
const struct abbrev2 *SWLocale::getBookAbbrevs2() {
	static const char *nullstr = "";
	if (!bookAbbrevs2) {
		ConfigEntMap::iterator it;
		int i;
		int size = localeSource->Sections["Book Abbrevs"].size();
		bookAbbrevs2 = new struct abbrev2[size + 1];
		for (i = 0, it = localeSource->Sections["Book Abbrevs"].begin(); it != localeSource->Sections["Book Abbrevs"].end(); it++, i++) {
			bookAbbrevs2[i].ab = (*it).first.c_str();
			bookAbbrevs2[i].book = VerseKey2::getOSISBookNum((*it).second.c_str());
			//printf("SWLocale::getBookAbbrevs2 %d:%s:%s %d\n",i,bookAbbrevs2[i].ab,
			//	(*it).second.c_str(), bookAbbrevs2[i].book); 
		}
		bookAbbrevs2[i].ab = nullstr;
		bookAbbrevs2[i].book = ENDOFABBREVS;
	}
		
	return bookAbbrevs2;
}
void SWLocale::getBooks2(struct sbook2 ***ibooks, VerseKey2 *vk) {
	if (!books2) {

		books2 = new struct sbook2*[1];
		books2[0] = new struct sbook2[MAXOSISBOOKS+1];
		for (int j = 0; j <= MAXOSISBOOKS; j++) {
				books2[0][j].name = translate(vk->getNameOfBook(j));
				books2[0][j].prefAbbrev = translate(vk->getPrefAbbrev(j));
		}
	}

	*ibooks = books2;
}
#endif
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


SWORD_NAMESPACE_END
