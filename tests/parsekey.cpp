/******************************************************************************
 *
 *  parsekey.cpp -
 *
 * $Id$
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/swlog.h>

using namespace swordxx;

int main(int argc, char **argv) {
    if ((argc < 2) || (argc > 8)) {
        std::cerr << "usage: " << *argv << " <\"string to parse\"> [locale_name] [v11n] [context] [echo params 1|0] [test-in-set-verse 1|0] [intros 1|0]\n";
        std::exit(-1);
    }

    if (argc > 2)
        LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(argv[2]);

    VerseKey DefaultVSKey;
//    DefaultVSKey.AutoNormalize(0);

    if (argc > 3)
        DefaultVSKey.setVersificationSystem(argv[3]);

    const char *context = (argc > 4) ? argv[4] : "gen.1.1";

    bool echo = (argc > 5) && !std::strcmp(argv[5], "1");
    bool inSetTest = (argc > 6) && !std::strcmp(argv[6], "1");
    bool intros = (argc > 7) && !std::strcmp(argv[7], "1");

    DefaultVSKey.setIntros(intros);

    SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);
    DefaultVSKey.validateCurrentLocale();

    DefaultVSKey.setText(context);

    ListKey verses = DefaultVSKey.parseVerseList(argv[1], DefaultVSKey.getText().c_str(), true);

    if (echo) {
        for (int i = 1; i < argc; i++) {
            if (i > 1) std::cout << " ";
            std::cout << argv[i];
        }
        std::cout << ": ";
    }
    std::cout << verses.getRangeText() << "\n";
/*
    for (int i = 0; i < verses.Count(); i++) {
        std::cout << verses.GetElement(i)->getRangeText() << "\n";
    }
*/

    if (inSetTest) {
        verses.setText(context);
        std::cout << "Verse is" << ((verses.popError()) ? " NOT" : "") << " in set.\n\n";
    }

    return 0;
}
