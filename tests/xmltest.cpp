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
    std::cout << x.toString() << "\nTag name: [" << x.name() << "]\n";
    for (auto const & name : x.attributeNames())
        std::cout << " - attribute: [" << name << "] = ["
                  << x.attribute(name.c_str()) << "]\n";
    std::cout << " isEmpty: " << x.isEmpty() << "\n isEndTag: " << x.isEndTag()
              << "\n\n";

    if (argc < 2) {  // only run if we're defaulted
        static char const testParts[] = "ABC D EF GHIJ";
        std::cout << "Setting attribute 'multiPart' to: '" << testParts
                  << "'\n";
        x.setAttribute("multiPart", testParts);
        std::cout << x.toString() << std::endl;
    }

}
