/******************************************************************************
 *
 *  lastVerseInChapter.cpp -    This example demonstrates how to navigate
 *                verses using VerseKey.  It uselessly iterates
 *                every chapter of the KJV, finds the last verse
 *                and prints out the sole verse in the KJV which
 *                ends in a ',' -- just for fun.
 *
 * Copyright 2011-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/utilstr.h>


using namespace swordxx;
using namespace std;


int main() {
    SWMgr library;

    auto const book(library.getModule("KJV"));

    auto const vk(book->getKeyAs<VerseKey>());
    for (;!vk->popError();vk->setChapter(vk->getChapter()+1)) {
        vk->setVerse(vk->getVerseMax());
        std::string text = book->stripText();
        trimString(text);
        if (!text.empty() && text.back() == ',') {
            cout << vk->getText() << ":\n\n";
            cout << text << endl;
        }
    }

    return 0;
}

