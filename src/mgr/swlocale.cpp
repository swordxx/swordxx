/******************************************************************************
 *  swlocale.cpp   - implementation of Class SWLocale used for retrieval
 *				of locale lookups
 *
 * $Id: swlocale.cpp,v 1.2 2000/03/12 23:12:32 scribe Exp $
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
		delete description;

	if (name)
		delete name;
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
