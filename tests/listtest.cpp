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

#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <listkey.h>
#include <versekey.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv)
{
	ListKey lk, lk2;
	VerseKey vk("jn 1:1", "jn 1:12");
	VerseKey vk2("jude", "jude");
	SWKey text;

	vk = "jas 1:19";
	text = (const char *)vk;
	lk << text;
	lk << text;
	lk << text;
	lk << "James 1:19";
	lk << "yoyo";
	lk << vk;
	lk2 << "test1";
	lk2 << lk;
	lk2 << vk2;
	lk2 << "test2";
	for (lk2 = TOP; !lk2.Error(); lk2++)
		std::cout << (const char *) lk2 << "\n";


	lk2 = VerseKey().ParseVerseList("mat-mark", 0, true);

	VerseKey yoyo("john");
	yoyo = MAXCHAPTER;
	std::cout << yoyo;
/*

	for (int i = 0; i < 2; i++) {
	VerseKey x, y;
	ListKey lk3;
	x = "rev";
	y = x;
	x = "mat";
	VerseKey newElement;
	newElement.LowerBound(x);
	newElement.UpperBound(y);
	lk3 << newElement;

	lk2 << lk3;
	}

*/
	std::cout << "---------\n";

	for (lk2 = TOP; !lk2.Error(); lk2++)
		std::cout << (const char *) lk2 << "\n";

	lk.ClearList();
	lk << "john 3:16";
	std::cout << "\nCount should be 1: " << lk.Count();

	lk = vk.ParseVerseList("mat;mark;luke", vk, true);
	lk = (VerseKey)"john 3:16";
	std::cout << "\nError should be set: " << ((lk.Error()) ? "set":"not set");
	lk = vk.ParseVerseList("mk 3:16", vk, true);
	lk = (VerseKey)"john 3:16";
	std::cout << "\nError should be set: " << ((lk.Error()) ? "set":"not set");

	std::cout << "\n\n";
	return 0;
}
