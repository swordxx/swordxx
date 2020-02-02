/******************************************************************************
 *
 *  emptyvss.cpp -    Utility to list empty verses in a module
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <iostream>
#include <swordxx/keys/versekey.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using swordxx::SWMgr;
using swordxx::VerseKey;
using swordxx::SWKey;
using swordxx::SWModule;

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <Mod Name>\n", argv[0]);
        std::exit(-1);
    }

    SWMgr mgr;

    auto const it = mgr.modules().find(argv[1]);
    if (it == mgr.modules().end()) {
        fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
        std::exit(-2);
    }

    SWModule & mod = *it->second;

    auto const key(mod.getKey());
    auto const vkey = dynamic_cast<VerseKey *>(key.get());
    if (!vkey) {
        fprintf(stderr, "error: %s: %s module is not keyed to verses \n", argv[0], argv[1]);
        std::exit(-3);
    }

    vkey->setIntros(false);    // turn on mod/testmnt/book/chap headings

    mod.positionToTop();

    while (!mod.popError()) {
        if (vkey->getVerse()) {
            if (!mod.renderText().length()) {
                std::cout << vkey->getText() << std::endl;
            }
            mod.increment();
        }
    }
}
