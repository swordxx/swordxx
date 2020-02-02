/******************************************************************************
 *
 *  lexdump.c -    This utility outputs a specified ordinal entry from a lex
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#ifndef O_BINARY
#define O_BINARY 0
#endif

int main(int argc, char **argv) {
    char *tmpbuf;
    int idxfd, datfd;
    long offset;
    unsigned int size;
    char datbuf[255];

    if (argc != 3) {
        fprintf(stderr, "usage: %s <datapath/datafilebasename> <index>\n", argv[0]);
        std::exit(1);
    }

    {
        auto const tmpBuf(std::make_unique<char[]>(std::strlen(argv[1]) + 11));
        sprintf(tmpBuf.get(), "%s.idx", argv[1]);
        idxfd = open(tmpBuf.get(), O_RDONLY|O_BINARY);
        sprintf(tmpBuf.get(), "%s.dat", argv[1]);
        datfd = open(tmpBuf.get(), O_RDONLY|O_BINARY);
    }

    offset = std::atoi(argv[2]) * 6;
    lseek(idxfd, offset, SEEK_SET);
    read(idxfd, &offset, 4);
    read(idxfd, &size, 2);
    printf("offset: %ld; size: %d\n", offset, size);
    lseek(datfd, offset, SEEK_SET);
    read(datfd, datbuf, 40);
    datbuf[40] = 0;
    printf("%s\n", datbuf);
    close(datfd);
    close(idxfd);
    return 0;

}
