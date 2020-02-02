/******************************************************************************
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cstring>
#include <iostream>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


int main(int argc, char * argv[]) {
    std::string const modName((argc > 1) ? argv[2] : "WLC");
    std::string const keyText((argc > 2) ? argv[3] : "Gen.1.9");
    std::string const searchText((argc > 3) ? argv[4] : "מתחת");
    swordxx::SWMgr library;
    auto const book(library.getModule(modName.c_str()));
    auto const filters(library.getGlobalOptions());
    for (auto const & filter : filters)
        /* Blindly turn off all filters. Some filters don't support "Off", but
           that's ok, we should just silently fail on those. */
        library.setGlobalOption(filter.c_str(), "Off");

    book->setKey(keyText.c_str());
    std::string const entryStripped(book->stripText());
    std::cout << "Module: " << book->getDescription() << "\t Key: "
              << book->getKeyText() << "\nRawEntry:\n" << book->getRawEntry()
              << "\nStripText:\n" << entryStripped << "\nSearch Target: "
              << searchText << "\nSearch Target StripText: "
              << book->stripText(searchText.c_str()) << "\nFound: "
              << (std::strstr(entryStripped.c_str(),
                              book->stripText(searchText.c_str()).c_str())
                  ? "true"
                  : "false")
              << std::endl;
}
