/******************************************************************************
 *
 *  vs2osisref.cpp -    Utility to translate a verse reference to an osisRef
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

using std::endl;
using std::cerr;
using std::cout;


int main(int argc, char **argv)
{
        if (argc < 2) {
            cerr << argv[0] << " - parse verse reference to OSISRef markup\n";
            cerr << "usage: "<< argv[0] << " <verse ref> [verse context] [locale]\n";
            cerr << "\n\n";
            std::exit(-1);
        }


    if (argc > 3) {
        LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(argv[3]);
    }

        VerseKey verseKey = (argc > 2) ? argv[2] : "Gen 1:1";

        std::cout << VerseKey::convertToOSIS(argv[1], &verseKey) << "\n";

    return 0;
}

