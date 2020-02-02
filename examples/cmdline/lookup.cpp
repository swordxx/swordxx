/******************************************************************************
 *
 *  lookup.cpp -    Simple example of how to retrieve an entry from a
 *            Sword++ module
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <swordxx/keys/versekey.h>
#include <swordxx/markupfiltmgr.h>
#include <swordxx/swfilter.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using swordxx::SWMgr;
using swordxx::MarkupFilterMgr;
using swordxx::SWModule;
using swordxx::FMT_XHTML;
using swordxx::VerseKey;


int main(int argc, char **argv)
{
    SWMgr manager(std::make_shared<MarkupFilterMgr>(FMT_XHTML));
    if (argc != 3) {
        fprintf(stderr, "\nusage: %s <modname> <\"lookup key\">\n"
                             "\tExample: lookup KJV \"James 1:19\"\n\n", argv[0]);

        std::exit(-1);
    }

    auto const target(manager.getModule(argv[1]));
    if (!target) {
        fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
        for (auto const & mp : manager.modules()) {
            fprintf(stderr, "[%s]\t - %s\n",
                    mp.second->getName().c_str(),
                    mp.second->getDescription().c_str());
        }
        std::exit(-1);
    }

    // turn all filters to default values
    manager.setGlobalOption("Headings", "On");
    manager.setGlobalOption("Strong's Numbers", "Off");
    manager.setGlobalOption("Lemmas", "Off");
//    manager.setGlobalOption("Greek Accents", "Off");

    auto const k(target->getKey());
    if (auto const vk = dynamic_cast<VerseKey *>(k.get())) {
    //    vk->AutoNormalize(false);
        vk->setIntros(true);
        vk->setText(argv[2]);
    }
    else {
        target->setKey(argv[2]);
    }

    target->renderText();        // force an entry lookup to resolve key to something in the index

    std::cout << "==Raw=Entry===============\n";
    std::cout << target->getKeyText() << ":\n";
    std::cout << target->getRawEntry();
    std::cout << "\n";
    std::cout << "==Render=Entry============\n";
    std::cout << target->getRenderHeader();
    std::cout << "\n";
    std::cout << target->renderText();
    std::cout << "\n";
    std::cout << "==========================\n";
    std::cout << "Entry Attributes:\n\n";
    bool heading = false;
    bool preverse = false;
    for (auto const & vp1 : target->getEntryAttributes()) {
        std::cout << "[ " << vp1.first << " ]\n";
        heading = (vp1.first == "Heading");
        for (auto const & vp2 : vp1.second) {
            std::cout << "\t[ " << vp2.first << " ]\n";
            preverse = (heading && vp2.first == "Preverse");
            for (auto const & vp3 : vp2.second) {
                std::cout << "\t\t" << vp3.first << " = " << vp3.second << "\n";
                if (preverse)
                    std::cout << "\t\tmodule->renderText(heading[" << vp3.first
                              << "]) = "
                              << target->renderText(vp3.second.c_str()) << "\n";
            }
        }
    }
    std::cout << std::endl;

    return 0;
}

