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
#include <iostream>
#include <versekey.h>
#include <rawtext.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	SWMgr mymgr;

	RawText::createModule(".");
	RawText mod(".");

	VerseKey vk;
	vk.Headings(1);
	vk.AutoNormalize(0);
	vk.Persist(1);
	mod.setKey(vk);

	vk.Verse(0);
	vk.Chapter(0);
	vk.Book(0);
	vk.Testament(0);

	mod << "Module heading text";

	vk.Verse(0);
	vk.Chapter(0);
	vk.Book(0);
	vk.Testament(1);

	mod << "OT heading text";

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(0);
	vk.Verse(0);

	mod << "Gen heading text";

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(0);

	mod << "Gen 1 heading text";

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(1);

	mod << "Gen 1:1 text";

	
	vk.Testament(0);
	vk.Book(0);
	vk.Chapter(0);
	vk.Verse(0);

	std::cout << "Module heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(0);
	vk.Chapter(0);
	vk.Verse(0);

	std::cout << "OT heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(0);
	vk.Verse(0);

	std::cout << "Gen heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(0);

	std::cout << "Gen 1 heading text ?= " << (const char*)mod << std::endl;

	vk.Testament(1);
	vk.Book(1);
	vk.Chapter(1);
	vk.Verse(1);

	std::cout << "Gen 1:1 text ?= " << (const char*)mod << std::endl;

	  /* old introtest
	SWModule *mhc = mymgr.Modules["MHC"];

	if (mhc) {
		VerseKey vk;
		vk.Headings(1);
		vk.AutoNormalize(0);
		vk.Persist(1);
		vk = "jas 0:0";
		std::cout << vk << ":\n";
		mhc->setKey(vk);
		std::cout << (const char *) mhc->Key() << ":\n";
		std::cout << (const char *) *mhc << "\n";
	}
	  */
	return 0;
}


