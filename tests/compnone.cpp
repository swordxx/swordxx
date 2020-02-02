/******************************************************************************
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

#include <cstdlib>
#include <cstring>
#include <swordxx/filemgr.h>
#include <swordxx/modules/common/swcomprs.h>
#ifdef __GNUC__
#include <unistd.h>
#endif


using namespace swordxx;

namespace {

class FileCompress: public SWCompress {

public: /* Methods: */

    FileCompress(std::string const & filename)
        : ufd(FileMgr::createPathAndFile(filename.c_str()))
        , zfd(FileMgr::createPathAndFile((filename + ".zzz").c_str()))
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
        SWCompress::Encode();
    }

    void Decode() override {
        ifd = zfd;
        ofd = ufd;
        SWCompress::Decode();
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
