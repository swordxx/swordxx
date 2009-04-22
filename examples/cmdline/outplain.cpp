/******************************************************************************
 * This example show how to output the plain text entries from a SWORD module.
 * This is also good for speed tests
 *
 * Class SWMgr manages installed modules for a frontend.
 * SWMgr reads a mods.conf file to discover its information.
 * It then instantiates the correct decendent of SWModule for each
 * module entry in mods.conf
 * The developer may use this class to query what modules are installed
 * and to retrieve an (SWModule *) for any one of these modules
 *
 * SWMgr makes its modules available as an STL Map.
 * The Map definition is typedef'ed as ModMap
 * ModMap consists of: FIRST : SWBuf moduleName
 *                     SECOND: SWModule *module
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

/******************************************************************************
 * Class SWMgr manages installed modules for a frontend.
 * SWMgr reads a mods.conf file to discover its information.
 * It then instantiates the correct decendent of SWModule for each
 * module entry in mods.conf
 * The developer may use this class to query what modules are installed
 * and to retrieve an (SWModule *) for any one of these modules
 *
 * SWMgr makes its modules available as an STL Map.
 * The Map definition is typedef'ed as ModMap
 * ModMap consists of: FIRST : SWBuf moduleName
 *                     SECOND: SWModule *module
 *
 */

#include <iostream>

#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <markupfiltmgr.h>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {
	SWMgr manager(new MarkupFilterMgr(sword::FMT_HTMLHREF, sword::ENC_UTF16));

	const char *bookName = (argc > 1) ? argv[1] : "WLC";
	SWModule *b = manager.getModule(bookName);
	if (!b) return -1;
	SWModule &book = *b;
	book.processEntryAttributes(false);
	VerseKey *vk = SWDYNAMIC_CAST(VerseKey, book.getKey());
	for (book = TOP; !book.Error() && !book.getRawEntryBuf().size(); book++);
	if (!book.getRawEntryBuf().size()) return -2; 	// empty module
	for (;!book.Error(); book++) {
		cout << "$$$";
		if (vk) cout << vk->getOSISRef();
		else    cout << book.getKeyText();
		cout << "\n" << book.StripText() << "\n\n";
	}
}
