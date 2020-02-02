/******************************************************************************
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <cstring>
#ifndef __GNUC__
#include <io.h>
#endif
#include <swordxx/filemgr.h>
#include <swordxx/modules/common/rawstr.h>
#include <swordxx/swmgr.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace swordxx;

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: %s <lex path>\n\n", *argv);
        std::exit(-1);
    }

    RawStr mod(argv[1]);
     char buf[127];

    sprintf(buf, "%s.idx", argv[1]);
    long maxoff = FileMgr::getSystemFileMgr()->open(buf, FileMgr::RDONLY, true)->seek(0, SEEK_END) - 6;

    std::string last = "";
    bool first = true;
    for (long index = 0; index < maxoff; index+=6) {
        auto const trybuf(mod.getIDXBuf(index));
        if (!first) {
            if (trybuf.compare(last) < 0) {
                printf("entry %ld(offset: %ld) (%s) is less than previous entry (%s)\n\n", index/6, index, trybuf.c_str(), last.c_str());
                std::exit(-3);
            }
        }
        else first = false;
        last = trybuf;
    }

    return 0;
}
