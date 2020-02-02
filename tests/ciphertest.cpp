/******************************************************************************
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <swordxx/filemgr.h>
#include <swordxx/filters/cipherfil.h>
#include <string>


using namespace swordxx;

int main(int argc, char **argv) {

    if (argc != 3) {
        std::cerr << "usage: " << *argv << " <key> <0-encipher|1-decipher>\n";
        return -1;
    }


    long encipher = std::atoi(argv[2]);

    CipherFilter filter(argv[1]);

    std::string text;
    char buf[4096];
    std::cin >> buf;
    text = buf;

    filter.processText(text, (SWKey *)encipher);

    std::cout << text;
}
