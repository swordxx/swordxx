/*
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

#include <swmgr.h>
#include <swtext.h>
#include <versekey.h>
#include <iostream>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	SWMgr mymgr;
	ModMap::iterator it;
	SWModule *module = mymgr.Modules["KJV"];
	VerseKey parser;
	ListKey lk = parser.ParseVerseList("mal4:6-rev", parser, true);
	lk.Persist(1);
	module->SetKey(lk);

	(*module) = TOP;
	std::cout << module->KeyText() << "\n";
	return 0;
}
