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

#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <versemgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

using std::cout;
using std::endl;

int main(int argc, char **argv) {
	VerseMgr *vmgr = VerseMgr::getSystemVerseMgr();
	const VerseMgr::System *system = vmgr->getVersificationSystem("KJV");

	int book, chapter, verse;
	for (long offset = 0; offset < 31000; offset++) {
		system->getVerseFromOffset(offset, &book, &chapter, &verse);
		cout << "offset: " << offset << "; book: " << book << "; chapter: " << chapter << "; verse: " << verse << "\n";
	}
	cout << endl;

	return 0;
}
