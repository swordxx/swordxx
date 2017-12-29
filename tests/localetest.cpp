/******************************************************************************
 *
 *  localetest.cpp -
 *
 * $Id$
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

#include <iostream>
#include <swordxx/keys/versekey.h>
#include <swordxx/localemgr.h>


using namespace swordxx;

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr <<  "usage: " << *argv << " <locale_name> <text>\n";
        std::exit(-1);
    }

    std::cout << LocaleMgr::getSystemLocaleMgr()->translate(argv[2], argv[1])
              << "\n";

/*
    VerseKey bla;
    bla = "James 1:19";

    bla.setLocale("de");
    std::cout << bla << std::endl;
    bla = "Johannes 1:1";
    std::cout << bla << std::endl;

    LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName("de");
    VerseKey key2;
    key2.setLocale("en");
    ListKey list = key2.ParseVerseList("Luke 3:23-28",key2, true);
    std::cout << list << std::endl;
*/


}
