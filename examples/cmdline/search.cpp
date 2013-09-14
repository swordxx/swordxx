/******************************************************************************
 *
 *  search.cpp -	This simple example shows how to perform a search on a
 *			SWORD module. It amounts to a simple commandline
 *			search tool with a usage like:
 *
 *				search KJV "swift hear slow speak"
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <versekey.h>
#include <markupfiltmgr.h>
#include <regex.h> // GNU
#include <iostream>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

/*
 *			>=0 - regex
 *			-1  - phrase
 *			-2  - multiword
 *			-3  - entryAttrib (eg. Word//Lemma/G1234/)
 *			-4  - Lucene
 */

char SEARCH_TYPE=-2;

char printed = 0;
void percentUpdate(char percent, void *userData) {
	char maxHashes = *((char *)userData);
	
	while ((((float)percent)/100) * maxHashes > printed) {
		std::cerr << "=";
		printed++;
		std::cerr.flush();
	}
/*
	std::cout << (int)percent << "% ";
*/
	std::cout.flush();
}


int main(int argc, char **argv)
{
//	SWMgr manager(0, 0, true, new MarkupFilterMgr(FMT_RTF, ENC_RTF));
	SWMgr manager;
	SWModule *target;
	ListKey listkey;
	ListKey scope;
	ModMap::iterator it;

	if ((argc < 3) || (argc > 5)) {
		fprintf(stderr, "\nusage: %s <modname> <\"search string\"> [\"search_scope\"] [\"search again for string in previous result set\"]\n"
							 "\tExample: search KJV \"swift hear slow speak\"\n\n", argv[0]);

		exit(-1);
	}


	SWBuf searchTerm = argv[2];
	manager.setGlobalOption("Greek Accents", "Off");
	manager.setGlobalOption("Strong's Numbers", "On");
	manager.setGlobalOption("Hebrew Vowel Points", "Off");
	manager.filterText("Greek Accents", searchTerm);

	it = manager.Modules.find(argv[1]);
	if (it == manager.Modules.end()) {
		fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
		for (it = manager.Modules.begin(); it != manager.Modules.end(); ++it) {
			fprintf(stderr, "[%s]\t - %s\n", (*it).second->getName(), (*it).second->getDescription());
		}
		exit(-1);
	}

	target = (*it).second;

	if (argc > 3) {			// if min / max specified
		SWKey *k = target->getKey();
		VerseKey *parser = SWDYNAMIC_CAST(VerseKey, k);
		VerseKey kjvParser;
		if (!parser) parser = &kjvParser;	// use standard KJV parsing as fallback
		scope = parser->parseVerseList(argv[3], *parser, true);
		scope.setPersist(true);
		target->setKey(scope);
	}

	std::cerr << "[0=================================50===============================100]\n ";
	char lineLen = 70;
	listkey = target->search(searchTerm.c_str(), SEARCH_TYPE, /*SEARCHFLAG_MATCHWHOLEENTRY*/ REG_ICASE, 0, 0, &percentUpdate, &lineLen);
	std::cerr << std::endl;
	if (argc > 4) {			// if min / max specified
		scope = listkey;
		scope.setPersist(true);
		target->setKey(scope);
		printed = 0;
		std::cerr << " ";
		listkey = target->search(argv[4], SEARCH_TYPE, /*SEARCHFLAG_MATCHWHOLEENTRY*/ REG_ICASE, 0, 0, &percentUpdate, &lineLen);
		std::cerr << std::endl;
	}
	listkey.sort();
	while (!listkey.popError()) {
		std::cout << (const char *)listkey << std::endl;
		listkey++;
	}

	return 0;

}
