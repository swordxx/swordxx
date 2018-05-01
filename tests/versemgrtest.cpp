/******************************************************************************
 *
 *  versemgrtest.cpp -
 *
 * $Id$
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <swordxx/versificationmgr.h>


using namespace swordxx;

using std::cout;
using std::endl;

int main(int argc, char **argv) {

    const char *v11n = (argc > 1) ? argv[1] : "KJV";

    const VersificationMgr::System *system = VersificationMgr::systemVersificationMgr()->getVersificationSystem(v11n);
    int bookCount = system->getBookCount();
    const VersificationMgr::Book *lastBook = system->getBook(bookCount-1);
    int chapMax = lastBook->getChapterMax();
    int verseMax = lastBook->getVerseMax(chapMax);
    long offsetMax = system->getOffsetFromVerse(bookCount-1, chapMax, verseMax);

    cout << "Versification System: " << v11n << "\n";
    cout << "Book Count: " << bookCount << "\n";
    cout << "Last Book: " << lastBook->getLongName() << " (" << lastBook->getOSISName() << ")\n";
    cout << "  Chapter Max: " << chapMax << "\n";
    cout << "    Verse Max: " << verseMax << "\n";
    cout << "       Offset: " << offsetMax << "\n\n";
    cout << "Offset, Book, Chapter, Verse\n";

    int book, chapter, verse;
    for (long offset = 0; offset <= offsetMax; offset++) {
        system->getVerseFromOffset(offset, &book, &chapter, &verse);
        cout << offset << ": " << book << ", " << chapter << ", " << verse << "\n";
    }

    cout << endl;

    return 0;
}
