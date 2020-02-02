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

#include <cassert>
#include <cstring>
#include <iostream>
#include <memory>
#include <swordxx/filters/utf8greekaccents.h>
#include <swordxx/unicode.h>
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
        std::unique_ptr<UTF8GreekAccents> filter;
        if (argc > 1 && !std::strcmp(argv[1], "-ga")) {
            filter = std::make_unique<UTF8GreekAccents>();
            filter->setOptionValue("Off");
        }
        int repeat = 1;
        if (argc > 2)
            repeat = std::atoi(argv[2]);
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
        std::string_view sv(filteredContents);
        while (!sv.empty()) {
            auto r(codepointFromUtf8(sv));
            if (r.second) {
                sv.remove_prefix(r.second);
            } else {
                assert(!r.first);
                r.first = 0xfffd;
                sv.remove_prefix(1u);
            }
            cout << utf8FromValidCodepoint(r.first);
        }
    }
}
