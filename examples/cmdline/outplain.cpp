/******************************************************************************
 *
 *  outplain.cpp -    This example shows how to output the plain text
 *            entries from a Sword++ module. This small program
 *            outputs a Sword++ module in 'imp' format, e.g.:
 *
 * $$$Gen.1.1
 * In the beginning God created
 * the heavens and the earth
 *
 * $$$Gen.1.2
 * ...
 *
 * Class SWMgr manages installed modules for a frontend.
 * The developer may use this class to query what modules are installed
 * and to retrieve an (SWModule *) for any one of these modules
 *
 * SWMgr makes its modules available as an STL Map.
 * The Map definition is typedef'ed as ModMap
 * ModMap consists of: FIRST : std::string moduleName
 *                     SECOND: SWModule *module
 *
 * Copyright 2008-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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
#include <memory>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;
using namespace std;


int main(int argc, char **argv) {

    SWMgr manager(std::make_shared<MarkupFilterMgr>(swordxx::FMT_HTMLHREF, swordxx::ENC_UTF16));

    const char *bookName = (argc > 1) ? argv[1] : "WLC";
    auto const b(manager.getModule(bookName));
    if (!b) return -1;
    SWModule &book = *b;
    book.setProcessEntryAttributes(false);
    auto const vk(book.getKeyAs<VerseKey const>());
    for (book.positionToTop(); !book.popError() && !book.getRawEntry().size(); book.increment());
    if (!book.getRawEntry().size()) return -2;     // empty module
    for (;!book.popError(); book.increment()) {
        cout << "$$$";
        if (vk) cout << vk->getOSISRef();
        else    cout << book.getKeyText();
        cout << "\n" << book.stripText() << "\n\n";
    }

    return 0;
}
