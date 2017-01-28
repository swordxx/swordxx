/******************************************************************************
 *
 *  listoptions.cpp -    Simple example to show how to see which 'options' are
 *            available from the installed set of modules and their
 *            possible settings.
 *            Options in Sword++ refer to things like "Strong's
 *            Numbers", "Morphology", etc.
 *
 * $Id$
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

#include <iostream>
#include <swordxx/swmgr.h>


int main() {
    swordxx::SWMgr library;
    auto const options(library.getGlobalOptions());
    for (auto const & option : options) {
        std::cout << option << " ("
                  << library.getGlobalOptionTip(option.c_str()) << ")\n";
        auto const optionValues(library.getGlobalOptionValues(option.c_str()));
        for (auto const & optionValue : optionValues)
            std::cout << "\t" << optionValue << "\n";
    }
    std::cout << std::flush;
}

