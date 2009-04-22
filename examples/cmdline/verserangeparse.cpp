/******************************************************************
 * This example shows:
 *	How to parse a verse reference
 *	How to persist a custom range key in a book
 *
 * $Id: swmgr.h 2321 2009-04-13 01:17:00Z scribe $
 *
 * Copyright 1998-2009 CrossWire Bible Society (http://www.crosswire.org)
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

#include <iostream>
#include <swmgr.h>
#include <versekey.h>
#include <listkey.h>
#include <swmodule.h>
#include <markupfiltmgr.h>

using sword::SWMgr;
using sword::VerseKey;
using sword::ListKey;
using sword::SWModule;
using sword::SW_POSITION;
using sword::FMT_PLAIN;
using sword::MarkupFilterMgr;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
	const char *range = (argc > 1) ? argv[1] : "Mat 2:10,12-15";

	VerseKey parser;
	ListKey result;

	result = parser.ParseVerseList(range, parser, true);
	// let's iterate the key and display
	for (result = TOP; !result.Error(); result++) {
		cout << result << "\n";
	}
	cout << endl;

	// Now if we'd like persist this key for use inside of a book...
	result.Persist(true);
	
	// Let's get a book;
	SWMgr library(new MarkupFilterMgr(FMT_PLAIN));	// render plain without fancy markup
	SWModule *book = library.getModule("KJV");

	// and set our limited key inside
	book->setKey(result);

	// now let's iterate the book and display
	for ((*book) = TOP; !book->Error(); (*book)++) {
		cout << "*** " << book->getKeyText() << ": " << book->RenderText() << "\n";
	}

	// since we've told our result key to persist in book, we can reuse our
	// setup by simply resetting result, e.g.
	//
	// result = parser.ParseVerseList(someNewRange, parser, true);
	//
	// now an iteration of book will give us our new range.
	//
	// to stop persistence of our custom key, we'll need to set our book's key
	// to something simple:
	// 
	// book->setKey("gen.1.1");
	// 
	// this allows book to create and use an instance of its preferred key type
	//

	return 0;
}
