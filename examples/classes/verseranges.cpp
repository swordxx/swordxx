/******************************************************************************
 *
 *  verseranges.cpp -    This example demonstrates how to work with contiguous
 *            verse ranges using VerseKey
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


using namespace swordxx;
using namespace std;


int main() {
    cout << "\n";

    const char *modName = "KJVA";
    SWMgr manager;
    auto const bible(manager.getModule(modName));
    if (!bible) {
        cout << modName << " not installed for example.  Please install.\n\n";
        std::exit(-1);
    }

    std::shared_ptr<VerseKey> vk(
                std::static_pointer_cast<VerseKey>(bible->createKey()));

    // let's set verse ranges for a variety of different contiguous regions

    // set a verse range for the whole Bible
    vk->setLowerBoundKey(*vk);    // vk initially points to TOP, so we don't need to set position first
    vk->positionToBottom();
    vk->setUpperBoundKey(*vk);
    cout << vk->getRangeText() << "\n";


    vk->clearBounds();


    // Old Testament
    vk->positionToTop();
    vk->setLowerBoundKey(*vk);

    vk->setTestament(2);
    vk->decrement();

    vk->setUpperBoundKey(*vk);
    cout << vk->getRangeText() << "\n";


    vk->clearBounds();


    // New Testament
    vk->positionToTop();
    vk->setTestament(2);
    vk->setLowerBoundKey(*vk);
    vk->positionToBottom();
    vk->setUpperBoundKey(*vk);
    cout << vk->getRangeText() << "\n";


    vk->clearBounds();


    // Current Book
    vk->setText("John 3:16");
    vk->setChapter(1); vk->setVerse(1);
    vk->setLowerBoundKey(*vk);
    vk->setChapter(vk->getChapterMax()); vk->setVerse(vk->getVerseMax());
    vk->setUpperBoundKey(*vk);
    cout << vk->getRangeText() << "\n";


    // -------------------------
    // Shorter syntax using the parser and based on book names, and requires intimate knowledge of VersificationMgr
    // You're probably better off using the above code, but this is here for completeness
    //
    auto const refSys(vk->versificationSystem());


    // whole Bible
    VerseKey vkBible(
                refSys->books().front().getOSISName().c_str(),
                refSys->books().back().getOSISName().c_str(),
                refSys);
    cout << vkBible.getRangeText() << "\n";

    // OT
    VerseKey vkOT(
                refSys->books().front().getOSISName().c_str(),
                refSys->books()[refSys->getBMAX()[0]-1].getOSISName().c_str(),
                refSys);
    cout << vkOT.getRangeText() << "\n";

    // NT
    VerseKey vkNT(
                refSys->books()[refSys->getBMAX()[0]].getOSISName().c_str(),
                refSys->books().back().getOSISName().c_str(),
                refSys);
    cout << vkNT.getRangeText() << "\n";

    // Current Book
    vk->setText("John 3:16");
    VerseKey vkCurrentBook(vk->getBookName().c_str(),
                           vk->getBookName().c_str(),
                           refSys);
    cout << vkCurrentBook.getRangeText() << "\n" << endl;

    return 0;
}

