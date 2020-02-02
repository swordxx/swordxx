/******************************************************************************
 *
 *  mod2vpl.cpp -    Utility to export a module in VPL format
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <cstring>
#include <iostream>
#include <swordxx/keys/versekey.h>
#include <swordxx/swkey.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using swordxx::SWMgr;
using swordxx::VerseKey;
using swordxx::SWModule;
using swordxx::SWKey;

void cleanbuf(char *buf) {
    char *from = buf;
    char *to = buf;

    while (*from) {
        if ((*from != 10) && (*from != 13)) {
            *to++ = *from++;
        }
        else {
            from++;
        }
    }
    *to = 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s <Mod Name> [0|1 - prepend verse reference to each line]\n", argv[0]);
        std::exit(-1);
    }

    SWMgr mgr;

    auto const it = mgr.modules().find(argv[1]);
    if (it == mgr.modules().end()) {
        fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
        std::exit(-2);
    }

    bool vref = false;
    if (argc > 2)
        vref = (argv[2][0] == '0') ? false : true;


    SWModule & mod = *it->second;

    auto const key = mod.getKey();
    VerseKey * const vkey = dynamic_cast<VerseKey *>(key.get());
    if (!vkey) {
        fprintf(stderr, "error: %s: %s module is not keyed to verses \n", argv[0], argv[1]);
        std::exit(-3);
    }

    vkey->setIntros(true);    // turn on mod/testmnt/book/chap headings

    mod.positionToTop();

    std::vector<char> buffer;
    while (!mod.popError()) {
        buffer.resize(mod.renderText().length() + 1u);
        std::strcpy(buffer.data(), mod.renderText().c_str());
        cleanbuf(buffer.data());
        if (vref) {
            if ((std::strlen(buffer.data()) > 0) && (vref)) {
                std::cout << vkey->getText() << " ";
                std::cout << buffer.data() << std::endl;
            }
        }
        else std::cout << buffer.data() << std::endl;

        mod.increment();
    }
}
