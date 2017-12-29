/******************************************************************************
 *
 *  romantest.cpp -
 *
 * $Id$
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

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <swordxx/roman.h>


using swordxx::from_rom;

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <roman_numeral>\n", *argv);
        exit(-1);
    }
    /* I don't think we need to_rom, do we? anyway, it isn't written
    char buf[127];
    if (charIsDigit(argv[1][0])) {
        to_rom(std::atoi(argv[1]), buf);
        std::cout << buf << std::endl;
    }
    */
    else std::cout << from_rom(argv[1]) << std::endl;
    return 0;
}

