/******************************************************************************
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
#include <swordxx/keys/listkey.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/localemgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


#define MODULE_NAME "KJVgb"

int main() {
    swordxx::SWMgr mgr;
    auto const mod(mgr.getModule(MODULE_NAME));
    if (!mod) {
        std::cerr << "Module \"" MODULE_NAME "\" not found!" << std::endl;
        return EXIT_FAILURE;
    }

    std::shared_ptr<swordxx::VerseKey> key1(
                std::static_pointer_cast<swordxx::VerseKey>(mod->createKey()));

    key1->setTestament(2);
    key1->setBook(4);
    key1->setChapter(2);
    key1->setVerse(3);

    std::cout << "\n" << key1->getText() << ":\n\n";

    swordxx::ListKey keys;
    keys << *key1;

    std::cout << "\n" << keys.getRangeText() << ":\n\n";

    swordxx::ListKey keys2 = keys;

    std::cout << "\n" << keys2.getRangeText() << ":\n\n";

    keys = key1->parseVerseList("Lk.4.5");

    std::cout << "\n" << key1->getText() << ":\n\n";

    key1->setText("jn.6.7");

    std::cout << "\n" << key1->getText() << ":\n\n";


    mod->setKey("lk.2.3");

    std::cout << "\n" << mod->getKeyText() << ":\n" << std::endl;
    std::cout << mod->getRawEntry() << std::endl;


    std::cout << "\nListkey persist key iteration test\n\n";
    keys = key1->parseVerseList("mat1", nullptr, true);

    for (keys.positionToTop(); !keys.popError(); keys.increment())
        std::cout << "\n" << keys.getText() << ":\n" << std::endl;

    mod->setKey(keys.clone());

    for (mod->positionToTop(); !mod->popError(); mod->increment()) {
        std::cout << "\n" << mod->getKeyText() << ":\n" << std::endl;
    }
}
