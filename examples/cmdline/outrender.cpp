/******************************************************************************
 *
 *  outrender.cpp -    This example show how to choose an output render
 *            markup and render entries from a Sword++ module. The
 *            following snippet outputs a module in HTML output
 *            encoded as UTF8.
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
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

    // find the first non-zero entry
    for (book.positionToTop(); !book.popError() && !book.getRawEntry().size(); book.increment());
    if (!book.getRawEntry().size()) return -2;     // empty module

    for (;!book.popError(); book.increment()) {

        cout << "$$$";
        if (vk) cout << vk->getOSISRef();
        else    cout << book.getKeyText();

        cout << "\n" << book.renderText() << "\n\n";
    }

    return 0;
}

