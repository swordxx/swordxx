/******************************************************************************
 *
 *  search.cpp -    This simple example shows how to perform a search on a
 *            Sword++ module. It amounts to a simple commandline
 *            search tool with a usage like:
 *
 *                search KJV "swift hear slow speak"
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cstdint>
#include <cstdio>
#include <iostream>
#include <regex.h> // GNU
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/modules/texts/rawtext.h>
#include <swordxx/swmgr.h>


using namespace swordxx;

/*
 *            >=0 - regex
 *            -1  - phrase
 *            -2  - multiword
 *            -3  - entryAttrib (eg. Word//Lemma/G1234/)
 *            -4  - Lucene
 */

char SEARCH_TYPE=-4;

char printed = 0;
void percentUpdate(char percent, void *userData) {
    char maxHashes = *((char *)userData);

    while ((((float)percent)/100) * maxHashes > printed) {
        std::cerr << "=";
        printed++;
        std::cerr.flush();
    }
/*
    std::cout << (int)percent << "% ";
*/
    std::cout.flush();
}


int main(int argc, char **argv)
{
//    SWMgr manager(0, 0, true, new MarkupFilterMgr(FMT_RTF, ENC_RTF));
    SWMgr manager;
    ListKey listkey;
    ListKey scope;
    if ((argc < 3) || (argc > 5)) {
        fprintf(stderr, "\nusage: %s <modname> <\"search string\"> [\"search_scope\"] [\"search again for string in previous result set\"]\n"
                             "\tExample: search KJV \"swift hear slow speak\"\n\n", argv[0]);

        exit(-1);
    }


    std::string searchTerm = argv[2];
    manager.setGlobalOption("Greek Accents", "Off");
    manager.setGlobalOption("Strong's Numbers", "On");
    manager.setGlobalOption("Hebrew Vowel Points", "Off");
    manager.filterText("Greek Accents", searchTerm);

    auto const it(manager.modules().find(argv[1]));
    if (it == manager.modules().end()) {
        fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
        for (auto const & mp : manager.modules())
            fprintf(stderr, "[%s]\t - %s\n",
                    mp.second->getName().c_str(),
                    mp.second->getDescription().c_str());
        exit(-1);
    }

    SWModule & target = *it->second;

    if (argc > 3) {            // if min / max specified
        SWKey *k = target.getKey();
        VerseKey * parser = dynamic_cast<VerseKey *>(k);
        VerseKey kjvParser;
        if (!parser) parser = &kjvParser;    // use standard KJV parsing as fallback
        scope = parser->parseVerseList(argv[3], parser->getText(), true);
        scope.setPersist(true);
        target.setKey(scope);
    }

    std::cerr << "[0=================================50===============================100]\n ";
    char lineLen = 70;
    listkey = target.search(searchTerm.c_str(), SEARCH_TYPE, /*SEARCHFLAG_MATCHWHOLEENTRY*/ REG_ICASE, nullptr, &percentUpdate, &lineLen);
    std::cerr << std::endl;
    if (argc > 4) {            // if min / max specified
        scope = listkey;
        scope.setPersist(true);
        target.setKey(scope);
        printed = 0;
        std::cerr << " ";
        listkey = target.search(argv[4], SEARCH_TYPE, /*SEARCHFLAG_MATCHWHOLEENTRY*/ REG_ICASE, nullptr, &percentUpdate, &lineLen);
        std::cerr << std::endl;
    }
// we don't want to sort by verse if we've been given scores
//    listkey.sort();
    while (!listkey.popError()) {
        std::cout << listkey.getText();
        if (listkey.getElement()->m_userData) std::cout << " : " << listkey.getElement()->m_userData << "%";
        std::cout << std::endl;
        ++listkey;
    }

    return 0;

}
