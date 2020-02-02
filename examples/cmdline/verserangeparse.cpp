/******************************************************************************
 *
 *  verserangeparse.cpp -    This example shows
 *                how to parse a verse reference
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/keys/listkey.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using swordxx::SWMgr;
using swordxx::VerseKey;
using swordxx::ListKey;
using swordxx::SWModule;
using swordxx::FMT_PLAIN;
using swordxx::MarkupFilterMgr;
using std::cout;
using std::endl;

int main(int argc, char **argv)
{
    const char *range = (argc > 1) ? argv[1] : "Mat 2:10,12-15";

    VerseKey parser;
    auto result(std::make_shared<ListKey>());

    *result = parser.parseVerseList(range, parser.getText().c_str(), true);

    // let's iterate the key and display
    for (result->positionToTop(); !result->popError(); result->increment()) {
        cout << result->getText() << "\n";
    }
    cout << endl;

    // Now let's output a module with the entries from the result

    // we'll initialize our library of books
    SWMgr library(std::make_shared<MarkupFilterMgr>(FMT_PLAIN));    // render plain without fancy markup

    // Let's get a book;
    auto const book(library.getModule("KJV"));

    // couldn't find our test module
    if (!book) return -1;

    // now let's iterate the book and display
    book->setKey(result);
    for (result->positionToTop(); !result->popError(); result->increment()) {
        cout << "*** " << book->getKeyText() << ": " << book->renderText() << "\n";
    }

    return 0;
}
