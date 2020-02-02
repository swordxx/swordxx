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

#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <swordxx/filemgr.h>
#include <swordxx/filters/papyriplain.h>
#include <swordxx/filters/rtfhtml.h>
#include <swordxx/filters/utf8utf16.h>
#include <swordxx/sysdata.h>


using namespace swordxx;
using namespace std;


int main(int argc, char **argv) {
//    UTF8UTF16 filter;
    RTFHTML filter;
//    PapyriPlain filter;
//
    auto const fileMgr(FileMgr::getSystemFileMgr());
    auto fd((argc > 1) ? fileMgr->open(argv[1], FileMgr::RDONLY) : nullptr);

    std::string lineBuffer = "T\\u12345?his is t<e>xt which has papy-\nri markings in it.\n  L[et's be] sure it gets--\n cleaned up well for s(earching)";

    if (argc > 1 && !std::strcmp(argv[1], "-v")) {
        std::cout << "Original:\n\n";

        while (!(lineBuffer = fd->getLine()).empty())
            cout << lineBuffer << "\n";

        cout << "\n\n-------\n\n";
    }

    if (fd)
        fd = fileMgr->open(argv[1], FileMgr::RDONLY);

//    cout << "\xff\xfe";    // UTF16LE file signature

    while (!(lineBuffer = fd->getLine()).empty()) {
        lineBuffer += "\n";
        filter.processText(lineBuffer);
        cout << lineBuffer;
/*
//    Example showing safe to cast to u16 stream
        unsigned int size = lineBuffer.size() / 2;
        uint16_t *wcharBuf = (uint16_t *)lineBuffer.getRawData();
        for (unsigned int i = 0; i < size; ++i) {
            std::wcout << (wchar_t)wcharBuf[i];    // must cast for correct output and because wchar_t is different size on linux we couldn't declare out wcharBuf a wchar_t *
        }
*/
    }

    return 0;
}

