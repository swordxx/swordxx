/******************************************************************************
 * Simple example to show how to see which options are available from the
 * installed set of modules and their possible settings.
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

using sword::SWMgr;
using sword::StringList;
using std::cout;

int main(int argc, char **argv)
{
	SWMgr library;

	StringList options = library.getGlobalOptions();
	for (StringList::const_iterator it = options.begin(); it != options.end(); it++) {
		cout << *it << " (" << library.getGlobalOptionTip(*it) << ")\n";
		StringList optionValues = library.getGlobalOptionValues(*it);
		for (StringList::const_iterator it2 = optionValues.begin(); it2 != optionValues.end(); it2++) {
			cout << "\t" << *it2 << "\n";
		}
	}
	return 0;
}
