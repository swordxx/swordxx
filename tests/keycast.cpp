/******************************************************************************
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
#include <swordxx/swmgr.h>


int main() {
    swordxx::SWMgr mgr;

#if 0
    // This works
    auto globalOptions = mgr.getGlobalOptions();
    for (auto const & str : globalOptions) {
        std::cout << str << std::endl;
        for (auto const & str2 : mgr.getGlobalOptionValues(str.c_str()))
            std::cout << "\t"<< str2 << std::endl;
    }
#endif

    // This crashes:
    for (auto const & str : mgr.getGlobalOptionValues("Footnotes"))
        std::cout << "\t"<< str << std::endl;
};





