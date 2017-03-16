/******************************************************************************
 *
 *  rawverse.h -    code for class 'RawVerse'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef RAWVERSEBASE_H
#define RAWVERSEBASE_H

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include <string>
#include "../../defs.h"
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../sysdata.h"
#include "../../utilstr.h"


namespace swordxx {

class FileDesc;

class SWDLLEXPORT RawVerseBase {

protected: /* Fields: */

    FileDesc * idxfp[2];
    FileDesc * textfp[2];

    std::string m_path;

public: /* Methods: */

    RawVerseBase(const char *ipath, int fileMode = -1);
    virtual ~RawVerseBase() noexcept;

    void readText(char testmt,
                  long start,
                  unsigned short size,
                  std::string & buf) const;
    /**
      \brief Finds the offset of the key verse from the indexes.
      \param testmt testament to find (0 - Bible/module introduction)
      \param idxoff offset into .vss
      \param start address to store the starting offset
      \param size address to store the size of the entry
    */
    template <int MULTIPLIER,
              long LEN,
              typename StartType,
              typename SizeType,
              typename SizeType2>
    void findOffset(char testmt,
                    long idxoff,
                    long * start,
                    SizeType2 * size) const
    {
        idxoff *= MULTIPLIER;
        if (!testmt)
            testmt = ((idxfp[1]) ? 1:2);

        if (idxfp[testmt-1]->getFd() >= 0) {
            idxfp[testmt-1]->seek(idxoff, SEEK_SET);
            StartType tmpStart;
            SizeType tmpSize;
            idxfp[testmt-1]->read(&tmpStart, 4);
            long len = idxfp[testmt-1]->read(&tmpSize, sizeof(tmpSize)); // read size

            *start = swapToArch(tmpStart);
            *size  = swapToArch(tmpSize);

            if (len < LEN) {
                *size = (SizeType2)((*start) ? (textfp[testmt-1]->seek(0, SEEK_END) - (long)*start) : 0);    // if for some reason we get an error reading size, make size to end of file
            }
        }
        else {
            *start = 0;
            *size = 0;
        }
    }

    template <typename StartType, typename SizeType, int MULTIPLIER>
    void doSetText(char testmt, long idxoff, char const * buf, long len) {
        StartType start;
        SizeType size;

        idxoff *= MULTIPLIER;
        if (!testmt)
            testmt = ((idxfp[1]) ? 1:2);

        size = (len < 0) ? std::strlen(buf) : len;

        start = textfp[testmt-1]->seek(0, SEEK_END);
        idxfp[testmt-1]->seek(idxoff, SEEK_SET);

        if (size) {
            textfp[testmt-1]->seek(start, SEEK_SET);
            textfp[testmt-1]->write(buf, (int)size);

            // add a new line to make data file easier to read in an editor
            static char const nl = '\n';
            textfp[testmt-1]->write(&nl, 1);
        }
        else {
            start = 0;
        }

        start = swapFromArch(start);
        size  = swapFromArch(size);

        idxfp[testmt-1]->write(&start, sizeof(start));
        idxfp[testmt-1]->write(&size, sizeof(size));
    }

    template <typename StartType, typename SizeType, int MULTIPLIER>
    void doLinkEntry(char testmt, long destidxoff, long srcidxoff) {
        StartType start;
        SizeType size;

        destidxoff *= MULTIPLIER;
        srcidxoff  *= MULTIPLIER;

        if (!testmt)
            testmt = ((idxfp[1]) ? 1:2);

        // get source
        idxfp[testmt-1]->seek(srcidxoff, SEEK_SET);
        idxfp[testmt-1]->read(&start, sizeof(start));
        idxfp[testmt-1]->read(&size, sizeof(size));

        // write dest
        idxfp[testmt-1]->seek(destidxoff, SEEK_SET);
        idxfp[testmt-1]->write(&start, sizeof(start));
        idxfp[testmt-1]->write(&size, sizeof(size));
    }

    template <typename OffsetType, typename SizeType>
    static char createModule(char const * ipath, char const * v11n) {
        assert(ipath);
        std::string path(ipath);
        removeTrailingDirectorySlashes(path);

        std::string const otPath(path + "/ot");
        std::string const ntPath(path + "/nt");
        std::string const otVssPath(path + "/ot.vss");
        std::string const ntVssPath(path + "/nt.vss");

        FileMgr & fileMgr = *FileMgr::getSystemFileMgr();

        static auto const openFile =
                [](FileMgr & fileMgr, std::string const & filename) {
                    auto * const fd =
                            fileMgr.open(filename.c_str(),
                                         FileMgr::CREAT | FileMgr::WRONLY,
                                         FileMgr::IREAD | FileMgr::IWRITE);
                    fd->getFd();
                    return fd;
                };

        static auto const touchFile =
                [](FileMgr & fileMgr, std::string const & filename)
                { return fileMgr.close(openFile(fileMgr, filename.c_str())); };

        FileMgr::removeFile(otPath.c_str());
        touchFile(fileMgr, otPath);

        FileMgr::removeFile(ntPath.c_str());
        touchFile(fileMgr, ntPath);

        FileMgr::removeFile(otVssPath.c_str());
        FileDesc * const fd = openFile(fileMgr, otVssPath);

        FileMgr::removeFile(ntVssPath.c_str());
        FileDesc * const fd2 = openFile(fileMgr, ntVssPath);

        VerseKey vk;
        vk.setVersificationSystem(v11n);
        vk.setIntros(true);
        OffsetType offset = 0;
        SizeType size = 0;
        offset = swapFromArch(offset);
        size = swapFromArch(size);

        for (vk = Position::Top; !vk.popError(); ++vk) {
            if (vk.getTestament() < 2) {
                fd->write(&offset, sizeof(offset));
                fd->write(&size, sizeof(size));
            } else {
                fd2->write(&offset, sizeof(offset));
                fd2->write(&size, sizeof(size));
            }
        }
        fd2->write(&offset, sizeof(offset));
        fd2->write(&size, sizeof(size));

        FileMgr::getSystemFileMgr()->close(fd);
        FileMgr::getSystemFileMgr()->close(fd2);

        return 0;
    }

}; /* class RawVerseBase */

} /* namespace swordxx */

#endif /* RAWVERSEBASE_H */
