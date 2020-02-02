/******************************************************************************
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

        const char *modName = "HunKar";
        SWMgr library;
        auto const book(library.getModule(modName));
        if (!book) {
                cerr << "Can't find module: " << modName << endl;
                return -1;
        }
        auto const key(book->getKeyAs<VerseKey>());

        key->setIntros(true);
        book->setSkipConsecutiveLinks(true);
        book->positionToTop();

        cout << key->getText() << endl;

        return 0;
}
