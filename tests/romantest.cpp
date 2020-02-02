/******************************************************************************
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/roman.h>


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << (*argv) << " <roman_numeral>" << std::endl;
        return EXIT_FAILURE;
    }

    if (auto const r = swordxx::parseRomanNumeral(argv[1u]); r.has_value()) {
        std::cout << (*r) << std::endl;
    } else {
        std::cerr << "Invalid input!" << std::endl;
        return EXIT_FAILURE;
    }
}

