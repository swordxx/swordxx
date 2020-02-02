/******************************************************************************
 *
 *  simple.cpp -    This is about the simplest useful example of using the
 *            Sword++ engine.
 *
 * After Sword++ is installed, it should compile with something similar to:
 *
 *    g++ -o simple simple.cpp `pkg-config --cflags --libs swordxx`
 *
 * If you'd like to choose the type of tag markup which Sword++ will output
 *    for you, include:
 *
 *    #include <markupfiltmgr.h>
 *
 * and change your instantiation of SWMgr, below, to (e.g., for HTML):
 *
 *    SWMgr library(0, 0, true, new MarkupFilterMgr(FMT_HTMLHREF));
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cstdlib>
#include <iostream>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;

int main(int argc, char * argv[]) {
    char const * key = "James 1:19";
    char const * module = "Darby";

    /* Parse arguments: (1) module (2) key */
    switch (argc) {
    case 0: break;
    case 1: break; // argv[0] only, e.g. application name
    case 3:
        key = argv[2];
        /* Fall through: */
    case 2:
        module = argv[1];
        break;
    default:
        std::cerr << "Usage: " << argv[0] << " [module [key]]" << std::endl;
        return EXIT_FAILURE;
    }

    {
        SWMgr library;
        if (auto const mod = library.getModule(module)) {
            if (mod->setKey(key) != 0) {
                std::cerr << "No such key: \"" << key << "\"!" << std::endl;
                return EXIT_FAILURE;
            }
            std::cout << mod->renderText() << std::endl;
            return EXIT_SUCCESS;
        }
    }
    std::cerr << "Error opening module \"" << module << "\"!" << std::endl;
    return EXIT_FAILURE;
}
