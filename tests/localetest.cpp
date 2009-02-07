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

#include <localemgr.h>
#include <versekey.h>
#include <iostream>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <locale_name> <text>\n", *argv);
		exit(-1);
	}

	LocaleMgr lm;

	printf("%s\n", lm.translate(argv[1], argv[2]));

	VerseKey bla;
	bla = "James 1:19";

	bla.setLocale("de");
	std::cout << bla << std::endl;
	bla = "Johannes 1:1";
	std::cout << bla << std::endl;

	LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName("de");
	VerseKey key2;
	key2.setLocale("en");
	ListKey list = key2.ParseVerseList("Luke 3:23-28",key2, true);
	std::cout << list << std::endl;


}
