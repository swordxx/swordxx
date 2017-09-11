/******************************************************************************
 *
 *  utf8norm.cpp -
 *
 * $Id$
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
#include <memory>
#include <swordxx/filters/utf8greekaccents.h>
#include <swordxx/utilstr.h>
#include <string>
#include <unistd.h>
#include <vector>


using namespace swordxx;
using namespace std;

int main(int argc, char **argv) {
    if (argc > 1 && argv[1][0] != '-') {
        auto const * buf = argv[1];
        std::string const fixed(assureValidUTF8(buf));
        cout << "input / processed:\n" << buf << "\n" << fixed << endl;
    } else {
        std::unique_ptr<SWOptionFilter> filter;
        if (argc > 1 && !strcmp(argv[1], "-ga")) {
            filter = std::make_unique<UTF8GreekAccents>();
            if (filter->isBoolean())
                filter->setOptionValue("Off");
        }
        int repeat = 1;
        if (argc > 2)
            repeat = atoi(argv[2]);
        std::string contents;
        char chunk[255];
        ::ssize_t count;
        do {
            count = ::read(STDIN_FILENO, chunk, 254);
            if (count > 0) {
                chunk[count] = '\0';
                contents.append(chunk);
            }
        } while (count > 0);
        auto filteredContents(contents);
        if (filter) {
            while (repeat-- > 0) {
                filteredContents = contents;
                filter->processText(filteredContents);
            }
        }
        std::vector<unsigned char> rawData(filteredContents.size() + 1u);
        std::memcpy(rawData.data(),
                    filteredContents.c_str(),
                    filteredContents.size() + 1u);
        // UTF-32 BOM
        std::uint32_t ch = 0xfeff;
        auto const * c = rawData.data();
        while (*c) {
            ch = getUniCharFromUTF8(&c);
            if (!ch)
                ch = 0xFFFD;
            cout << getUTF8FromUniChar(ch);
        }
    }
}
