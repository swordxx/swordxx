/******************************************************************************
 *
 *  vs2osisreftext.cpp -    Utility to translate a verse reference to an
 *                osisRef
 *
 * Copyright 2008-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/localemgr.h>


using namespace swordxx;

int main(int argc, char **argv) {
    if ((argc < 2) || (argc > 4)) {
        fprintf(stderr, "usage: %s <\"string to parse\"> [locale_name] [test-in-set-verse]\n", *argv);
        std::exit(-1);
    }

    if (argc > 2)
        LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(argv[2]);

    VerseKey DefaultVSKey;

    DefaultVSKey.setText("jas3:1");

    ListKey verses = DefaultVSKey.parseVerseList(argv[1], DefaultVSKey.getText().c_str(), true);

    std::cout << verses.getOSISRefRangeText() << "\n";

    if (argc > 3) {
        verses.setText(argv[3]);
        std::cout << "Verse is" << ((verses.popError()) ? " NOT" : "") << " in set.\n\n";
    }

    return 0;
}
