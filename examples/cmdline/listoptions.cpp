/******************************************************************************
 *
 *  listoptions.cpp -    Simple example to show how to see which 'options' are
 *            available from the installed set of modules and their
 *            possible settings.
 *            Options in Sword++ refer to things like "Strong's
 *            Numbers", "Morphology", etc.
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
#include <swordxx/swmodule.h>
#include <swordxx/swoptfilter.h>


int main(int argc, char * argv[]) {
    swordxx::SWMgr library;

    // specific module features:
    if (argc == 2) {
        auto module(library.getModule(argv[1u]));
        if (!module) {
            std::cerr << "Unable to find module: " << argv[1u] << std::endl;
            return 1;
        }
        std::cout << "Option Features available for module: "
                  << module->getName() << "\n\n";
        for (auto const & of : module->getOptionFilters()) {
            std::cout << of->getOptionName() << " (" << of->getOptionTip()
                      << ")\n";
            for (auto const & ov : of->getOptionValues())
                std::cout << "\t" << ov << "\n";
        }
        return 0;
    }

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

