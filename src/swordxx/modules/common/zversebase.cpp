/******************************************************************************
 *
 *  zverse.cpp -    code for class 'zVerse'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey for compressed modules
 *
 * $Id$
 *
 * Copyright 1996-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "zversebase.h"

#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../sysdata.h"
#include "../../utilstr.h"
#include "../common/swcomprs.h"


namespace swordxx {

/******************************************************************************
 * zVerse Constructor - Initializes data for instance of zVerse
 *
 * ENT:    ipath - path of the directory where data and index files are located.
 *        be sure to include the trailing separator (e.g. '/' or '\')
 *        (e.g. 'modules/texts/rawtext/webster/')
 *        fileMode - open mode for the files (FileMgr::RDONLY, etc.)
 *        blockType - verse, chapter, book, etc.
 */
template <typename VerseSizeType_>
zVerseBase<VerseSizeType_>::zVerseBase(NormalizedPath const & path,
                                       int fileMode,
                                       BlockType blockType,
                                       std::unique_ptr<SWCompress> icomp)
    : compressor(icomp ? std::move(icomp) : std::make_unique<SWCompress>())
{
    // this line, instead of just defaulting, to keep FileMgr out of header
    if (fileMode == -1) fileMode = FileMgr::RDONLY;

    cacheBufIdx = -1;
    cacheTestament = 0;
    cacheBuf = nullptr;
    dirtyCache = false;

    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    char const blockChar(static_cast<char>(blockType));
    idxfp[0] = FileMgr::getSystemFileMgr()->open(formatted("%s/ot.%czs", path.c_str(), blockChar).c_str(), fileMode, true);
    idxfp[1] = FileMgr::getSystemFileMgr()->open(formatted("%s/nt.%czs", path.c_str(), blockChar).c_str(), fileMode, true);
    textfp[0] = FileMgr::getSystemFileMgr()->open(formatted("%s/ot.%czz", path.c_str(), blockChar).c_str(), fileMode, true);
    textfp[1] = FileMgr::getSystemFileMgr()->open(formatted("%s/nt.%czz", path.c_str(), blockChar).c_str(), fileMode, true);
    compfp[0] = FileMgr::getSystemFileMgr()->open(formatted("%s/ot.%czv", path.c_str(), blockChar).c_str(), fileMode, true);
    compfp[1] = FileMgr::getSystemFileMgr()->open(formatted("%s/nt.%czv", path.c_str(), blockChar).c_str(), fileMode, true);
}

/******************************************************************************
 * zVerse Destructor - Cleans up instance of zVerse
 */
template <typename VerseSizeType_>
zVerseBase<VerseSizeType_>::~zVerseBase() {
    int loop1;

    if (cacheBuf) {
        flushCache();
        free(cacheBuf);
    }

    for (loop1 = 0; loop1 < 2; loop1++) {
        FileMgr::getSystemFileMgr()->close(idxfp[loop1]);
        FileMgr::getSystemFileMgr()->close(textfp[loop1]);
        FileMgr::getSystemFileMgr()->close(compfp[loop1]);
    }
}

/******************************************************************************
 * zVerseBase::findoffset    - Finds the offset of the key verse from the indexes
 *
 *
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    book    - book      to find (0 - testament    introduction)
 *    chapter    - chapter   to find (0 - book         introduction)
 *    verse    - verse     to find (0 - chapter      introduction)
 *    start    - address to store the starting offset
 *    size    - address to store the size of the entry
 */
template <typename VerseSizeType_>
void zVerseBase<VerseSizeType_>::findOffset(char testmt,
                                            long idxoff,
                                            VerseOffsetType * start,
                                            VerseSizeType * size,
                                            BufferNumberType * buffnum) const
{
    BufferNumberType ulBuffNum    = 0;              // buffer number
    uint32_t ulVerseStart = 0;           // verse offset within buffer
    VerseSizeType usVerseSize  = 0;           // verse size
    // set start to offset in
    // set size to
    // set
    *start = *size = *buffnum = 0;
    //fprintf(stderr, "Finding offset %ld\n", idxoff);
    idxoff *= sizeof(ulBuffNum) + sizeof(ulVerseStart) + sizeof(usVerseSize);
    if (!testmt) {
        testmt = ((idxfp[0]) ? 1:2);
    }

    // assert we have and valid file descriptor
    if (compfp[testmt-1]->getFd() < 1)
        return;

    long newOffset = compfp[testmt-1]->seek(idxoff, SEEK_SET);
    if (newOffset == idxoff) {
        if (compfp[testmt-1]->read(&ulBuffNum, sizeof(ulBuffNum)) != sizeof(ulBuffNum)) {
            fprintf(stderr, "Error reading ulBuffNum\n");
            return;
        }
    }
    else return;

    if (compfp[testmt-1]->read(&ulVerseStart, sizeof(ulVerseStart)) < sizeof(ulVerseStart))
    {
        fprintf(stderr, "Error reading ulVerseStart\n");
        return;
    }
    if (compfp[testmt-1]->read(&usVerseSize, sizeof(usVerseSize)) < sizeof(usVerseSize))
    {
        fprintf(stderr, "Error reading usVerseSize\n");
        return;
    }

    *buffnum = swapToArch(ulBuffNum);
    *start = swapToArch(ulVerseStart);
    *size = swapToArch(usVerseSize);
}

/******************************************************************************
 * zVerseBase::zreadtext    - gets text at a given offset
 *
 * ENT:    testmt    - testament file to search in (0 - Old; 1 - New)
 *    start    - starting offset where the text is located in the file
 *    size    - size of text entry + 1 (null)
 *    buf    - buffer to store text
 *
 */
template <typename VerseSizeType_>
void zVerseBase<VerseSizeType_>::zReadText(char testmt,
                                           VerseOffsetType start,
                                           VerseSizeType size,
                                           BufferNumberType ulBuffNum,
                                           std::string &inBuf) const
{
    uint32_t ulCompOffset = 0;           // compressed buffer start
    uint32_t ulCompSize   = 0;                 // buffer size compressed
    uint32_t ulUnCompSize = 0;              // buffer size uncompressed

    if (!testmt) {
        testmt = ((idxfp[0]) ? 1:2);
    }

    // assert we have and valid file descriptor
    if (compfp[testmt-1]->getFd() < 1)
        return;

    if (size &&
        !(((long) ulBuffNum == cacheBufIdx) && (testmt == cacheTestament) && (cacheBuf))) {
        //fprintf(stderr, "Got buffer number{%ld} versestart{%ld} versesize{%d}\n", ulBuffNum, ulVerseStart, usVerseSize);

        if (idxfp[testmt-1]->seek(ulBuffNum*12, SEEK_SET)!=(long) ulBuffNum*12)
        {
            fprintf(stderr, "Error seeking compressed file index\n");
            return;
        }
        if (idxfp[testmt-1]->read(&ulCompOffset, sizeof(ulCompOffset)) < sizeof(ulCompOffset))
        {
            fprintf(stderr, "Error reading ulCompOffset\n");
            return;
        }
        if (idxfp[testmt-1]->read(&ulCompSize, sizeof(ulCompSize)) < sizeof(ulCompSize))
        {
            fprintf(stderr, "Error reading ulCompSize\n");
            return;
        }
        if (idxfp[testmt-1]->read(&ulUnCompSize, sizeof(ulUnCompSize)) < sizeof(ulUnCompSize))
        {
            fprintf(stderr, "Error reading ulUnCompSize\n");
            return;
        }

        ulCompOffset  = swapToArch(ulCompOffset);
        ulCompSize  = swapToArch(ulCompSize);
        ulUnCompSize  = swapToArch(ulUnCompSize);

        if (textfp[testmt-1]->seek(ulCompOffset, SEEK_SET)!=(long)ulCompOffset)
        {
            fprintf(stderr, "Error: could not seek to right place in compressed text\n");
            return;
        }
        std::string pcCompText(ulCompSize + 5u, '\0');

        if (textfp[testmt-1]->read(&pcCompText[0u], ulCompSize) < ulCompSize) {
            fprintf(stderr, "Error reading compressed text\n");
            return;
        }
        pcCompText.resize(ulCompSize);
        rawZFilter(pcCompText, 0); // 0 = decipher

        unsigned long bufSize = ulCompSize;
        compressor->zBuf(&bufSize, &pcCompText[0u]);

        if (cacheBuf) {
            flushCache();
            free(cacheBuf);
        }

        unsigned long len = 0;
        compressor->Buf(nullptr, &len);
        cacheBuf = (char *)std::calloc(len + 1, 1);
        std::memcpy(cacheBuf, compressor->Buf(), len);
        cacheBufSize = std::strlen(cacheBuf);  // TODO: can we just use len?
        cacheTestament = testmt;
        cacheBufIdx = ulBuffNum;
    }

    inBuf.clear();
    if ((size > 0) && cacheBuf && ((unsigned)start < cacheBufSize)) {
        inBuf.resize(size + 1u, '\0');
        std::strncpy(&inBuf[0u], &(cacheBuf[start]), size);
        inBuf.resize(std::strlen(inBuf.c_str()));
    }
}

/******************************************************************************
 * zVerseBase::doSetText_ - Sets text for current offset
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    idxoff    - offset into .vss
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */
template <typename VerseSizeType_>
void zVerseBase<VerseSizeType_>::doSetText(char testmt,
                                           long idxoff,
                                           const char * buf,
                                           long len)
{
    len = (len < 0) ? std::strlen(buf) : len;
    if (!testmt)
        testmt = ((idxfp[0]) ? 1:2);
    if ((!dirtyCache) || (cacheBufIdx < 0)) {
        cacheBufIdx = idxfp[testmt-1]->seek(0, SEEK_END) / 12;
        cacheTestament = testmt;
        if (cacheBuf)
            free(cacheBuf);
        cacheBuf = (char *)std::calloc(len + 1, 1);
    }
    else cacheBuf = (char *)((cacheBuf)?realloc(cacheBuf, std::strlen(cacheBuf)+(len + 1)):std::calloc((len + 1), 1));

    dirtyCache = true;

    uint32_t outBufIdx = cacheBufIdx;
    uint32_t start;
    VerseSizeType size;

    idxoff *= sizeof(outBufIdx) + sizeof(start) + sizeof(size);
    size = len;

    start = std::strlen(cacheBuf);

    if (!size)
        start = outBufIdx = 0;

    outBufIdx = swapFromArch(outBufIdx);
    start  = swapFromArch(start);
    size   = swapFromArch(size);

    compfp[testmt-1]->seek(idxoff, SEEK_SET);
    compfp[testmt-1]->write(&outBufIdx, sizeof(outBufIdx));
    compfp[testmt-1]->write(&start, sizeof(start));
    compfp[testmt-1]->write(&size, sizeof(size));
    std::strcat(cacheBuf, buf);
}

template <typename VerseSizeType_>
void zVerseBase<VerseSizeType_>::flushCache() const {
    if (dirtyCache) {
        uint32_t idxoff;
        uint32_t start, outstart;
        uint32_t size, outsize;
        uint32_t zsize, outzsize;

        idxoff = cacheBufIdx * 12;
        if (cacheBuf) {
            size = outsize = zsize = outzsize = std::strlen(cacheBuf);
            if (size) {
                compressor->Buf(cacheBuf);
                unsigned long tmpSize;
                compressor->zBuf(&tmpSize);
                outzsize = zsize = tmpSize;

                std::string buf(zsize + 5u, '\0');
                /// \bug undefined order of evaluation of arguments:
                std::memcpy(&buf[0u], compressor->zBuf(&tmpSize), tmpSize);
                outzsize = zsize = tmpSize;
                buf.resize(zsize);
                rawZFilter(buf, 1); // 1 = encipher

                start = outstart = textfp[cacheTestament-1]->seek(0, SEEK_END);

                outstart  = swapFromArch(start);
                outsize   = swapFromArch(size);
                outzsize  = swapFromArch(zsize);

                textfp[cacheTestament-1]->write(buf.c_str(), zsize);

                idxfp[cacheTestament-1]->seek(idxoff, SEEK_SET);
                idxfp[cacheTestament-1]->write(&outstart, sizeof(outstart));
                idxfp[cacheTestament-1]->write(&outzsize, sizeof(outzsize));
                idxfp[cacheTestament-1]->write(&outsize, sizeof(outsize));
            }
            free(cacheBuf);
            cacheBuf = nullptr;
        }
        dirtyCache = false;
    }
}

/******************************************************************************
 * zVerseBase::doLinkEntry    - links one entry to another
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    destidxoff    - dest offset into .vss
 *    srcidxoff        - source offset into .vss
 */
template <typename VerseSizeType_>
void zVerseBase<VerseSizeType_>::doLinkEntry(char testmt,
                                             long destidxoff,
                                             long srcidxoff)
{
    int32_t bufidx;
    int32_t start;
    VerseSizeType size;

    constexpr auto const entrySize =
            sizeof(bufidx) + sizeof(start) + sizeof(size);
    destidxoff *= entrySize;
    srcidxoff  *= entrySize;

    if (!testmt)
        testmt = ((idxfp[1]) ? 1:2);

    // get source
    compfp[testmt-1]->seek(srcidxoff, SEEK_SET);
    compfp[testmt-1]->read(&bufidx, sizeof(bufidx));
    compfp[testmt-1]->read(&start, sizeof(start));
    compfp[testmt-1]->read(&size, sizeof(size));

    // write dest
    compfp[testmt-1]->seek(destidxoff, SEEK_SET);
    compfp[testmt-1]->write(&bufidx, sizeof(bufidx));
    compfp[testmt-1]->write(&start, sizeof(start));
    compfp[testmt-1]->write(&size, sizeof(size));
}

/******************************************************************************
 * zVerseBase::createModule - Creates new module files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */
template <typename VerseSizeType_>
char zVerseBase<VerseSizeType_>::createModule(NormalizedPath const & path,
                                              BlockType blockBound,
                                              const char * v11n)
{
    char retVal = 0;
    FileDesc *fd, *fd2;
    int32_t offset = 0;
    VerseSizeType size = 0;
    VerseKey vk;

    std::string buf(formatted("%s/ot.%czs", path.c_str(), static_cast<char>(blockBound)));
    auto const testamentIt(buf.begin() + path.size() + 1);
    assert(*testamentIt == 'o');
    auto const lastCharIt(buf.rbegin());
    assert(*lastCharIt == 's');
    FileMgr::removeFile(buf.c_str());
    fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    if (fd->getFd() < 1) goto erroropen1;
    FileMgr::getSystemFileMgr()->close(fd);

    (*testamentIt) = 'n';
    FileMgr::removeFile(buf.c_str());
    fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    if (fd->getFd() < 1) goto erroropen1;
    FileMgr::getSystemFileMgr()->close(fd);

    (*testamentIt) = 'o';
    (*lastCharIt) = 'z';
    FileMgr::removeFile(buf.c_str());
    fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    if (fd->getFd() < 1) goto erroropen1;
    FileMgr::getSystemFileMgr()->close(fd);

    (*testamentIt) = 'n';
    FileMgr::removeFile(buf.c_str());
    fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    if (fd->getFd() < 1) goto erroropen1;
    FileMgr::getSystemFileMgr()->close(fd);

    (*testamentIt) = 'o';
    (*lastCharIt) = 'v';
    FileMgr::removeFile(buf.c_str());
    fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    if (fd->getFd() < 1) goto erroropen1;

    (*testamentIt) = 'n';
    FileMgr::removeFile(buf.c_str());
    fd2 = FileMgr::getSystemFileMgr()->open(buf.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    if (fd2->getFd() < 1) goto erroropen2;

    vk.setVersificationSystem(v11n);
    vk.setIntros(true);

    offset = swapFromArch(offset);
    size   = swapFromArch(size);

    for (vk.positionToTop(); !vk.popError(); vk.increment()) {
        if (vk.getTestament() < 2) {
            if (fd->write(&offset, sizeof(offset)) != sizeof(offset)) goto writefailure;    //compBufIdxOffset
            if (fd->write(&offset, sizeof(offset)) != sizeof(offset)) goto writefailure;
            if (fd->write(&size, sizeof(size)) != sizeof(size)) goto writefailure;
        }
        else {
            if (fd2->write(&offset, sizeof(offset)) != sizeof(offset)) goto writefailure;    //compBufIdxOffset
            if (fd2->write(&offset, sizeof(offset)) != sizeof(offset)) goto writefailure;
            if (fd2->write(&size, sizeof(size)) != sizeof(size)) goto writefailure;
        }
    }
    fd2->write(&offset, sizeof(offset));    //compBufIdxOffset
    fd2->write(&offset, sizeof(offset));
    fd2->write(&size, sizeof(size));

    goto cleanup;

erroropen1:
    retVal = -1;
    goto cleanup1;

erroropen2:
    retVal = -1;
    goto cleanup;

writefailure:
    retVal = -2;

cleanup:
    FileMgr::getSystemFileMgr()->close(fd2);
cleanup1:
    FileMgr::getSystemFileMgr()->close(fd);

    return retVal;
}

// Explicit instantions:
template class zVerseBase<std::uint16_t>;
template class zVerseBase<std::uint32_t>;

} /* namespace swordxx */
