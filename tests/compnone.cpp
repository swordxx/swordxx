/******************************************************************************
 *
 *  compnone.cpp -
 *
 * $Id$
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

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#endif
#include <swordxx/filemgr.h>
#include <swordxx/modules/common/swcomprs.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace swordxx;

class FileCompress: public SWCompress {
    int ifd;
    int ofd;
    int ufd;
    int zfd;
public:
    FileCompress(char *);
    ~FileCompress() override;
    unsigned long GetChars(char *, unsigned long len, Direction) override;
    unsigned long SendChars(char *, unsigned long len, Direction) override;
    void Encode() override;
    void Decode() override;
};


FileCompress::FileCompress(char *fname)
{
    char buf[256];

    ufd  = FileMgr::createPathAndFile(fname);

    sprintf(buf, "%s.zzz", fname);
    zfd = FileMgr::createPathAndFile(buf);
}


FileCompress::~FileCompress()
{
    close(ufd);
    close(zfd);
}


unsigned long FileCompress::GetChars(char *buf, unsigned long len, Direction)
{
    return read(ifd, buf, len);
}


unsigned long FileCompress::SendChars(char *buf, unsigned long len, Direction)
{
    return write(ofd, buf, len);
}


void FileCompress::Encode()
{
    ifd = ufd;
    ofd = zfd;

    SWCompress::Encode();
}


void FileCompress::Decode()
{
    ifd = zfd;
    ofd = ufd;

    SWCompress::Decode();
}


int main(int argc, char * argv[]) {
    bool decomp = false;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename|filename.zzz>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (std::strlen(argv[1]) > 4) {
        if (!std::strcmp(&argv[1][std::strlen(argv[1])-4], ".zzz")) {
            argv[1][std::strlen(argv[1])-4] = 0;
            decomp = true;
        }
    }

    FileCompress fobj(argv[1]);
    if (decomp) {
        fobj.Decode();
    } else {
        fobj.Encode();
    }
}
