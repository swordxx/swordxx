/******************************************************************************
 *
 *  simple.cpp -	This is about the simplest useful example of using the
 *			SWORD engine.
 *
 * After sword is installed, it should compile with something similar to:
 *
 *	g++ -o simple simple.cpp `pkg-config --cflags --libs sword`
 *
 * If you'd like to choose the type of tag markup which sword will output
 *	for you, include:
 *
 *	#include <markupfiltmgr.h>
 *
 * and change your instantiation of SWMgr, below, to (e.g., for HTML):
 *
 *	SWMgr library(0, 0, true, new MarkupFilterMgr(FMT_HTMLHREF));
 *
 * $Id$
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <iostream>


using namespace sword;
using std::cout;


int main(int argc, char **argv) {
	SWMgr library;
	SWModule *darby = library.getModule("Darby");
	darby->setKey("James 1:19");
	cout << darby->RenderText();

	return 0;
}

