/******************************************************************************
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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
#include <cstring>
#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#endif
#include <swordxx/filemgr.h>
#include <swordxx/modules/common/zipcomprs.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace swordxx;

namespace {

class FileCompress: public ZipCompress {

public: /* Methods: */

    FileCompress(std::string const & filename)
        : ufd(FileMgr::createPathAndFile(filename.c_str()))
        , zfd(FileMgr::createPathAndFile((filename + ".zip").c_str()))
    {}

    ~FileCompress() override {
        ::close(ufd);
        ::close(zfd);
    }

    unsigned long GetChars(char * buf, unsigned long len, Direction) override
    { return ::read(ifd, buf, len); }

    unsigned long SendChars(char * buf, unsigned long len, Direction) override
    { return ::write(ofd, buf, len); }

    void Encode() override {
        ifd = ufd;
        ofd = zfd;
        ZipCompress::Encode();
    }

    void Decode() override {
        ifd = zfd;
        ofd = ufd;
        ZipCompress::Decode();
    }

private: /* Fields: */

    int ifd;
    int ofd;
    int ufd;
    int zfd;

};

} // anonymous namespace

int main(int argc, char * argv[]) {
    bool decomp = false;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <filename|filename.zip>\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (std::strlen(argv[1]) > 4) {
        if (!std::strcmp(&argv[1][std::strlen(argv[1])-4], ".zip")) {
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
