/******************************************************************************
 *
 *  xmltest.cpp -
 *
 * $Id$
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/utilxml.h>
#include <type_traits>


int main(int argc, char * argv[]) {
    static char const xml[] =
            "<verse osisID=\"John.1.1\" type=\'test type\' yeah = \"stuff\" />";
    std::cout << ((argc > 1) ? argv[1]: xml) << "\n";

    swordxx::XMLTag x(argc > 1 ? argv[1] : xml);

    std::cout << x.toString() << "\n";
    x.setAttribute("addedAttribute", "with a \" quote");
    std::cout << x.toString() << "\nTag name: [" << x.getName() << "]\n";
    for (auto const & name : x.getAttributeNames()) {
        auto const count = x.getAttributePartCount(name.c_str(), ' ');
        std::cout << " - attribute: [" << name << "] = ["
                  << x.getAttribute(name.c_str()) << "]\n\t" << count
                  << " parts:\n";
        // -1 for whole value cuz it's faster, but does the same thing as 0:
        std::remove_cv_t<decltype(count)> i = (count > 1) ? 0 : -1;
        do {
            std::cout << "\t" << x.getAttribute(name.c_str(), i, ' ') << "\n";
            if (i < 0)
                i = 0;    // to handle our -1 condition
        } while (++i < count);
    }
    std::cout << " isEmpty: " << x.isEmpty() << "\n isEndTag: " << x.isEndTag()
              << "\n\n";

    if (argc < 2) {  // only run if we're defaulted
        static char const testParts[] = "ABC D EF GHIJ";
        std::cout << "Setting attribute 'multiPart' to: '" << testParts
                  << "'\n";
        x.setAttribute("multiPart", testParts);
        std::cout << x.toString() << "\nSetting part 2 to 'MMM'\n";
        x.setAttribute("multiPart", "MMM", 2, ' ');
        std::cout << x.toString() << "\nRemoving part 1\n";
        x.setAttribute("multiPart", nullptr, 1, ' ');
        std::cout << x.toString() << "\nRemoving part 2\n";
        x.setAttribute("multiPart", nullptr, 2, ' ');
        std::cout << x.toString() << std::endl;
    }

}
