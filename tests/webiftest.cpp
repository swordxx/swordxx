/******************************************************************************
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#endif
#include <iostream>
#include <memory>
#include <swordxx/filters/gbfosis.h>
#include <swordxx/filters/thmlhtmlhref.h>
#include <swordxx/filters/thmlosis.h>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace swordxx;
using namespace std;

#define MAXBUF 30000

int main(int argc, char **argv) {

    const char* modName = (argc >= 2) ? argv[1] : "KJV";
    const char* keyName = (argc == 3) ? argv[2] : "John 1:1";

    SWMgr mgr(nullptr, nullptr, true, std::make_shared<MarkupFilterMgr>(FMT_WEBIF, ENC_UTF8));
    mgr.setGlobalOption("Strong's Numbers", "on");
    mgr.setGlobalOption("Morphological Tags", "on");

    auto module(mgr.getModule(modName));
    if (!module) {
        module = mgr.modules().begin()->second;
    }
    module->setKey(keyName);
    std::cout << module->renderText() << std::endl<< std::endl<< std::endl;

    //------------------------

    SWMgr mgr2(nullptr, nullptr, true, std::make_shared<MarkupFilterMgr>(FMT_HTMLHREF, ENC_UTF8));
    mgr2.setGlobalOption("Strong's Numbers", "on");
    mgr2.setGlobalOption("Morphological Tags", "on");
    module = mgr2.getModule(modName);
    if (!module) {
        module = mgr2.modules().begin()->second;
    }

    module->setKey(keyName);
    std::cout << module->renderText() << std::endl;

    return 0;
}

