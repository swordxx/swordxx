/******************************************************************************
 *
 *  versenorm.cpp -    checks normalization of verse to module
 *
 * Copyright 2012-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/keys/versekey.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;
using namespace std;


int main() {

        const char *modName = "RusCARS";
        const char *keyTextRegular = "1Sam1.20";
        const char *keyTextNeedsNormalization = "1Sam1.200";


        SWMgr library;
        auto const book(library.getModule(modName));
        if (!book) {
                cerr << "Can't find module: " << modName << endl;
                return -1;
        }


    // get two VerseKey objects for a module
    std::shared_ptr<VerseKey> verse(
                std::static_pointer_cast<VerseKey>(book->createKey()));
    std::shared_ptr<VerseKey> verseNormalized(
                std::static_pointer_cast<VerseKey>(book->createKey()));

    // turn off autonormalization for one VerseKey reference
        verse->setAutoNormalize(false);

    // set a reference not requiring normalization to both VerseKey objects
    verse->setText(keyTextRegular);
    verseNormalized->setText(keyTextRegular);

    // check to see if they are equal
        cout << verse->getText() << ((*verse) == (*verseNormalized) ? " == " : " != ") << verseNormalized->getText() << endl;

    // now do the same for a reference which requires normalization in this module's v11n system
    verse->setText(keyTextNeedsNormalization);
    verseNormalized->setText(keyTextNeedsNormalization);

        cout << verse->getText() << ((*verse) == (*verseNormalized) ? " == " : " != ") << verseNormalized->getText() << endl;

    return 0;
}
