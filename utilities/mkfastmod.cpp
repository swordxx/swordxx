/******************************************************************************
 *
 *  mkfastmod.cpp -
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

#ifdef _MSC_VER
    #pragma warning( disable: 4251 )
#endif

#include <cstdio>
#include <string>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/modules/texts/rawtext.h>
#include <swordxx/swmgr.h>


using swordxx::SWMgr;
using swordxx::SWModule;
using swordxx::ListKey;
using swordxx::VerseKey;
using swordxx::ModMap;

void percentUpdate(char percent, void *userData) {
    static char printed = 0;
    char maxHashes = *((char *)userData);

    while ((((float)percent)/100) * maxHashes > printed) {
        printf("=");
        printed++;
        fflush(stdout);
    }
/*
    std::cout << (int)percent << "% ";
*/
    fflush(stdout);
}


int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <modname>\n", argv[0]);
        exit(-1);
    }

    SWModule *target;
    ListKey listkey;
    ModMap::iterator it;

    SWMgr manager;
    it = manager.Modules.find(argv[1]);
    if (it == manager.Modules.end()) {
        fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
        for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
            fprintf(stderr,
                    "[%s]\t - %s\n",
                    (*it).second->getName().c_str(),
                    (*it).second->getDescription().c_str());
        }
        exit(-1);
    }
    target = it->second;

    printf("Deleting any existing framework...\n");
    target->deleteSearchFramework();
    printf("Building framework, please wait...\n");
    char lineLen = 70;
    printf("[0=================================50==============================100]\n ");
    char error = target->createSearchFramework(&percentUpdate, &lineLen);
    if (error) {
        fprintf(stderr, "%s: couldn't create search framework (permissions?)\n", *argv);
    }
    printf("\n");
}
