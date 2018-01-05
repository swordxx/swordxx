/******************************************************************************
 *
 *  rawverse.cpp -    code for class 'RawVerse'- a module that reads raw text
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

#include "rawversebase.h"

#include <cassert>
#include <cstring>
#include <fcntl.h>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../sysdata.h"
#include "../../utilstr.h"


namespace swordxx {

/******************************************************************************
 * RawVerse Constructor - Initializes data for instance of RawVerse
 *
 * ENT:    path - path of the directory where data and index files are located.
 *        be sure to include the trailing separator (e.g. '/' or '\')
 *        (e.g. 'modules/texts/rawtext/webster/')
 */
template <typename SizeType_>
RawVerseBase<SizeType_>::RawVerseBase(NormalizedPath path, int fileMode)
    : m_path(std::move(path))
{
    std::string buf(m_path.str() + "/ot.vss");

    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    auto testamentIt(buf.rbegin() + 5u);
    assert(*testamentIt == 'o');
    idxfp[0] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    (*testamentIt) = 'n';
    idxfp[1] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    buf.resize(buf.size() - 4u); // might invalidate testamentIt
    testamentIt = buf.rbegin() + 1;
    assert(*testamentIt == 'n');
    textfp[1] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);

    (*testamentIt) = 'o';
    textfp[0] = FileMgr::getSystemFileMgr()->open(buf.c_str(), fileMode, true);
}

template <typename SizeType_>
RawVerseBase<SizeType_>::~RawVerseBase() noexcept {
    FileMgr::getSystemFileMgr()->close(idxfp[0u]);
    FileMgr::getSystemFileMgr()->close(idxfp[1u]);
    FileMgr::getSystemFileMgr()->close(textfp[0u]);
    FileMgr::getSystemFileMgr()->close(textfp[1u]);
}


/******************************************************************************
 * RawVerseBase::readtext    - gets text at a given offset
 *
 * ENT:    testmt    - testament file to search in (0 - Old; 1 - New)
 *    start    - starting offset where the text is located in the file
 *    size    - size of text entry + 2 (null)(null)
 *    buf    - buffer to store text
 *
 */
template <typename SizeType_>
void RawVerseBase<SizeType_>::readText(char testmt,
                                       StartType start,
                                       SizeType size,
                                       std::string & buf) const
{
    buf.clear();
    buf.resize(size + 1u, '\0');
    if (!testmt)
        testmt = ((idxfp[1]) ? 1:2);
    if (size) {
        if (textfp[testmt-1]->getFd() >= 0) {
            textfp[testmt-1]->seek(start, SEEK_SET);
            textfp[testmt-1]->read(&buf[0u], (int)size);
        }
    }
}

template <typename SizeType_>
void RawVerseBase<SizeType_>::findOffset(char testmt,
                                         long idxoff,
                                         StartType * start,
                                         SizeType * size) const
{
    idxoff *= sizeof(StartType) + sizeof(SizeType);
    if (!testmt)
        testmt = ((idxfp[1]) ? 1:2);

    if (idxfp[testmt-1]->getFd() >= 0) {
        idxfp[testmt-1]->seek(idxoff, SEEK_SET);
        StartType tmpStart;
        SizeType tmpSize;
        idxfp[testmt-1]->read(&tmpStart, sizeof(tmpStart));
        idxfp[testmt-1]->read(&tmpSize, sizeof(tmpSize));

        *start = swapToArch(tmpStart);
        *size  = swapToArch(tmpSize);
    }
    else {
        *start = 0;
        *size = 0;
    }
}

template <typename SizeType_>
void RawVerseBase<SizeType_>::doSetText(char testmt,
                                        long idxoff,
                                        char const * buf,
                                        long len)
{
    StartType start;
    SizeType size;

    idxoff *= sizeof(StartType) + sizeof(SizeType);
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

template <typename SizeType_>
void RawVerseBase<SizeType_>::doLinkEntry(char testmt,
                                          long destidxoff,
                                          long srcidxoff)
{
    StartType start;
    SizeType size;

    destidxoff *= sizeof(StartType) + sizeof(SizeType);
    srcidxoff  *= sizeof(StartType) + sizeof(SizeType);

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

template <typename SizeType_>
char RawVerseBase<SizeType_>::createModule(NormalizedPath const & path,
                                           char const * v11n)
{
    std::string const otPath(path.str() + "/ot");
    std::string const ntPath(path.str() + "/nt");
    std::string const otVssPath(path.str() + "/ot.vss");
    std::string const ntVssPath(path.str() + "/nt.vss");

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
    StartType offset = 0;
    SizeType size = 0;
    offset = swapFromArch(offset);
    size = swapFromArch(size);

    for (vk.positionToTop(); !vk.popError(); ++vk) {
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

// Explicit instantiations:
template class RawVerseBase<std::uint16_t>;
template class RawVerseBase<std::uint32_t>;

} /* namespace swordxx */
