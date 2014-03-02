/******************************************************************************
 *
 *  simplechapter.cpp -	
 *
 * $Id$
 *
 * Copyright 2013-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <iostream>


using namespace sword;
using namespace std;


int main(int argc, char **argv) {

	SWMgr library;
	SWModule *kjv = library.getModule((argc > 1)?argv[1]:"KJV");

	kjv->setKey(argc > 2 ? argv[2] : "Jn.3.16");

	VerseKey *key = (VerseKey *)kjv->getKey();

	int curVerse = key->getVerse();
	int curChapter = key->getChapter();
	int curBook = key->getBook();

	for (key->setVerse(1);
				(key->getBook()    == curBook)    &&
				(key->getChapter() == curChapter) &&
				!kjv->popError();
				(*kjv)++) {
		if (key->getVerse() == curVerse) cout << "* ";
		cout << key->getVerse() << " " << kjv->stripText() << "\n";
	}

	return 0;
}

