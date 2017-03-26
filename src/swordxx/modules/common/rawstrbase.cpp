/******************************************************************************
 *
 *  rawstr.cpp -    code for class 'RawStr'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class StrKey
 *
 * $Id$
 *
 * Copyright 1998-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "rawstrbase.h"

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include "../../filemgr.h"
#include "../../stringmgr.h"
#include "../../swlog.h"
#include "../../sysdata.h"
#include "../../utilstr.h"


namespace swordxx {

/******************************************************************************
 * RawStr Constructor - Initializes data for instance of RawStr
 *
 * ENT:    ipath - path of the directory where data and index files are located.
 *        be sure to include the trailing separator (e.g. '/' or '\')
 *        (e.g. 'modules/texts/rawtext/webster/')
 */

RawStrBase::RawStrBase(NormalizedPath const & path,
                       int fileMode,
                       bool caseSensitive)
    : caseSensitive(caseSensitive)
{
    lastoff = -1;

    if (fileMode == -1) // try read/write if possible
        fileMode = FileMgr::RDWR;

    idxfd = FileMgr::getSystemFileMgr()->open((path.str() + ".idx").c_str(), fileMode, true);
    datfd = FileMgr::getSystemFileMgr()->open((path.str() + ".dat").c_str(), fileMode, true);

    if (!datfd)
        SWLog::getSystemLog()->logError("%d", errno);
}


/******************************************************************************
 * RawStr Destructor - Cleans up instance of RawStr
 */

RawStrBase::~RawStrBase() {
    FileMgr::getSystemFileMgr()->close(idxfd);
    FileMgr::getSystemFileMgr()->close(datfd);
}


/******************************************************************************
 * RawStr::getidxbufdat    - Gets the index string at the given idx offset
 *                        NOTE: buf is allocated and must be freed by
 *                            calling function
 *
 * ENT:    ioffset    - offset in dat file to lookup
 *        buf        - address of pointer to allocate for storage of string
 */

void RawStrBase::getIDXBufDat(long ioffset, char ** buf) const {
    int size;
    char ch;
    if (datfd) {
        datfd->seek(ioffset, SEEK_SET);
        for (size = 0; datfd->read(&ch, 1) == 1; size++) {
            if ((ch == '\\') || (ch == 10) || (ch == 13))
                break;
        }
        *buf = (*buf) ? (char *)realloc(*buf, size*2 + 1) : (char *)malloc(size*2 + 1);
        if (size) {
            datfd->seek(ioffset, SEEK_SET);
            datfd->read(*buf, size);
        }
        (*buf)[size] = 0;
        if (!caseSensitive) toupperstr_utf8(*buf, size*2);
    }
    else {
        *buf = (*buf) ? (char *)realloc(*buf, 1) : (char *)malloc(1);
        **buf = 0;
    }
}


/******************************************************************************
 * RawStr::getidxbuf    - Gets the index string at the given idx offset
 *                        NOTE: buf is allocated and must be freed by
 *                            calling function
 *
 * ENT:    ioffset    - offset in idx file to lookup
 *        buf        - address of pointer to allocate for storage of string
 */

void RawStrBase::getIDXBuf(long ioffset, char ** buf) const {
    IndexOffsetType offset;

    if (idxfd) {
        idxfd->seek(ioffset, SEEK_SET);
        idxfd->read(&offset, sizeof(offset));

        offset = swapToArch(offset);

        getIDXBufDat(offset, buf);
    }
}


/******************************************************************************
 * RawStr::findoffset    - Finds the offset of the key string from the indexes
 *
 * ENT:    key        - key string to lookup
 *        start    - address to store the starting offset
 *        size        - address to store the size of the entry
 *        away        - number of entries before of after to jump
 *                    (default = 0)
 *
 * RET: error status -1 general error; -2 new file
 */
template <typename SizeType>
signed char RawStrBase::findOffset_(char const * ikey,
                                    StartType * start,
                                    SizeType * size,
                                    long away,
                                    IndexOffsetType * idxoff) const
{
    char * trybuf;
    char * maxbuf;
    char quitflag = 0;
    signed char retval = -1;
    long headoff, tailoff, tryoff = 0, maxoff = 0;
    int diff = 0;
    bool awayFromSubstrCheck = false;

    static constexpr auto const entrySize =
            sizeof(StartType) + sizeof(SizeType);

    if (idxfd->getFd() >=0) {
        tailoff = maxoff = idxfd->seek(0, SEEK_END) - entrySize;
        retval = (tailoff >= 0) ? 0 : -2;    // if NOT new file
        if (*ikey && retval != -2) {
            headoff = 0;

            auto const ikeyLen(std::strlen(ikey));
            auto const keyBufLen((ikeyLen + 1u) * 3u);
            auto const key(std::make_unique<char[]>(keyBufLen));
            std::strcpy(key.get(), ikey);
            std::size_t keylen;
            if (!caseSensitive) {
                toupperstr_utf8(key.get(), ikeyLen * 3u);
                keylen = std::strlen(key.get());
            } else {
                keylen = ikeyLen;
            }

            bool substr = false;

            trybuf = maxbuf = nullptr;
            getIDXBuf(maxoff, &maxbuf);

            while (headoff < tailoff) {
                tryoff = (lastoff == -1) ? headoff + ((((tailoff / entrySize) - (headoff / entrySize))) / 2) * entrySize : lastoff;
                lastoff = -1;
                getIDXBuf(tryoff, &trybuf);

                if (!*trybuf && tryoff) {        // In case of extra entry at end of idx (not first entry)
                    if (tryoff > (maxoff / 2)) {
                        tryoff -= entrySize;
                    } else {
                        tryoff += entrySize;
                    }
                    retval = -1;
                    break;
                }

                diff = strcmp(key.get(), trybuf);

                if (!diff)
                    break;

                if (!strncmp(trybuf, key.get(), keylen)) substr = true;

                if (diff < 0)
                    tailoff = (tryoff == headoff) ? headoff : tryoff;
                else headoff = tryoff;

                if (tailoff == headoff + entrySize) {
                    if (quitflag++)
                        headoff = tailoff;
                }
            }

            // didn't find exact match
            if (headoff >= tailoff) {
                tryoff = headoff;
                if (!substr && ((tryoff != maxoff)||(strncmp(key.get(), maxbuf, keylen)<0))) {
                    awayFromSubstrCheck = true;
                    away--;    // if our entry doesn't startwith our key, prefer the previous entry over the next
                }
            }
            free(trybuf);
            free(maxbuf);
        }
        else    tryoff = 0;

        idxfd->seek(tryoff, SEEK_SET);

        StartType tmpStart = 0u;
        SizeType tmpSize = 0u;
        *start = *size = 0u;
        idxfd->read(&tmpStart, sizeof(tmpStart));
        idxfd->read(&tmpSize, sizeof(tmpSize));
        if (idxoff)
            *idxoff = tryoff;

        *start = swapToArch(tmpStart);
        *size  = swapToArch(tmpSize);

        while (away) {
            StartType const laststart = *start;
            SizeType const lastsize = *size;
            long lasttry = tryoff;
            // entrySize must be signed for the code following to work properly:
            int const signedEntrySize = static_cast<int>(entrySize);
            tryoff += (away > 0) ? signedEntrySize : -signedEntrySize;

            bool bad = false;
            if (((tryoff + (away*signedEntrySize)) < -signedEntrySize) || (tryoff + (away*signedEntrySize) > (maxoff+signedEntrySize)))
                bad = true;
            else if (idxfd->seek(tryoff, SEEK_SET) < 0)
                bad = true;
            if (bad) {
                if(!awayFromSubstrCheck)
                    retval = -1;
                *start = laststart;
                *size = lastsize;
                tryoff = lasttry;
                if (idxoff)
                    *idxoff = tryoff;
                break;
            }
            idxfd->read(&tmpStart, sizeof(tmpStart));
            idxfd->read(&tmpSize, sizeof(tmpSize));
            if (idxoff)
                *idxoff = tryoff;

            *start = swapToArch(tmpStart);
            *size  = swapToArch(tmpSize);

            if (((laststart != *start) || (lastsize != *size)) && (*size))
                away += (away < 0) ? 1 : -1;
        }

        lastoff = tryoff;
    }
    else {
        *start = 0;
        *size  = 0;
        if (idxoff)
            *idxoff = 0;
        retval = -1;
    }
    return retval;
}


/******************************************************************************
 * RawStr::readtext    - gets text at a given offset
 *
 * ENT:
 *    start    - starting offset where the text is located in the file
 *    size        - size of text entry
 *    buf        - buffer to store text
 *
 */
template <typename SizeType>
void RawStrBase::readText_(StartType istart,
                           SizeType * isize,
                           char ** idxbuf,
                           std::string & buf) const
{
    unsigned int ch;
    char * idxbuflocal = nullptr;
    getIDXBufDat(istart, &idxbuflocal);
    StartType start = istart;

    do {
        delete[] *idxbuf;

        buf.clear();
        buf.resize(++(*isize), '\0');

        *idxbuf = new char [ (*isize) ];

        datfd->seek(start, SEEK_SET);
        datfd->read(&buf[0u], (int)((*isize) - 1));

        for (ch = 0; buf[ch]; ch++) {        // skip over index string
            if (buf[ch] == 10) {
                ch++;
                break;
            }
        }
        buf = std::string(buf.c_str()+ch);
        // resolve link
        if (!strncmp(buf.c_str(), "@LINK", 5)) {
            for (ch = 0; buf[ch]; ch++) {        // null before nl
                if (buf[ch] == 10) {
                    buf[ch] = 0;
                    break;
                }
            }
            findOffset_<SizeType>(buf.c_str() + 6, &start, isize);
        }
        else break;
    }
    while (true);    // while we're resolving links

    if (idxbuflocal) {
        std::size_t localsize = strlen(idxbuflocal);
        localsize = (localsize < (*isize - 1)) ? localsize : (*isize - 1);
        strncpy(*idxbuf, idxbuflocal, localsize);
        (*idxbuf)[localsize] = 0;
        free(idxbuflocal);
    }
}


/******************************************************************************
 * RawLD::settext    - Sets text for current offset
 *
 * ENT: key    - key for this entry
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */
template <typename SizeType>
void RawStrBase::doSetText_(char const * ikey, char const * buf, long len) {
    StartType start, outstart;
    IndexOffsetType idxoff;
    SizeType size;
    SizeType outsize;
    char * tmpbuf = nullptr;
    char * dbKey = nullptr;
    char * ch = nullptr;

    char errorStatus = findOffset_<SizeType>(ikey, &start, &size, 0, &idxoff);
    auto const ikeyLen(std::strlen(ikey));
    auto const keyBufLen((ikeyLen + 1u) * 3u);
    auto const key(std::make_unique<char[]>(keyBufLen));
    std::strcpy(key.get(), ikey);
    if (!caseSensitive)
        toupperstr_utf8(key.get(), ikeyLen * 3u);

    len = (len < 0) ? strlen(buf) : len;

    getIDXBufDat(start, &dbKey);

    static constexpr auto const entrySize =
            sizeof(StartType) + sizeof(SizeType);

    if (strcmp(key.get(), dbKey) < 0) {
    }
    else if (strcmp(key.get(), dbKey) > 0) {
        if (errorStatus != (char)-2)    // not a new file
            idxoff += entrySize;
        else idxoff = 0;
    }
    else if ((!strcmp(key.get(), dbKey)) && (len>0 /*we're not deleting*/)) { // got absolute entry
        do {
            tmpbuf = new char [ size + 2 ];
            memset(tmpbuf, 0, size + 2);
            datfd->seek(start, SEEK_SET);
            datfd->read(tmpbuf, (int)(size - 1));

            for (ch = tmpbuf; *ch; ch++) {        // skip over index string
                if (*ch == 10) {
                    ch++;
                    break;
                }
            }
            memmove(tmpbuf, ch, size - std::size_t(ch - tmpbuf));

            // resolve link
            if (!strncmp(tmpbuf, "@LINK", 5) && (len)) {
                for (ch = tmpbuf; *ch; ch++) {        // null before nl
                    if (*ch == 10) {
                        *ch = 0;
                        break;
                    }
                }
                findOffset_<SizeType>(tmpbuf + entrySize, &start, &size, 0, &idxoff);
            }
            else break;
        }
        while (true);    // while we're resolving links
    }

    auto const endoff = idxfd->seek(0, SEEK_END);

    auto const shiftSize = endoff - idxoff;

    std::unique_ptr<char[]> idxBytes;
    if (shiftSize > 0) {
        idxBytes = std::make_unique<char[]>(shiftSize);
        idxfd->seek(idxoff, SEEK_SET);
        idxfd->read(idxBytes.get(), shiftSize);
    }

    auto const outbuf(std::make_unique<char[]>(len + strlen(key.get()) + 5));
    std::sprintf(outbuf.get(), "%s%c%c", key.get(), 13, 10);
    size = std::strlen(outbuf.get());
    std::memcpy(outbuf.get() + size, buf, len);
    size = outsize = size + (len);

    start = outstart = datfd->seek(0, SEEK_END);

    outstart = swapFromArch(start);
    outsize  = swapFromArch(size);

    idxfd->seek(idxoff, SEEK_SET);
    if (len > 0) {
        datfd->seek(start, SEEK_SET);
        datfd->write(outbuf.get(), size);

        // add a new line to make data file easier to read in an editor
        static char const nl = '\n';
        datfd->write(&nl, 1);

        idxfd->write(&outstart, sizeof(outstart));
        idxfd->write(&outsize, sizeof(outsize));
        if (idxBytes)
            idxfd->write(idxBytes.get(), shiftSize);
    }
    else {    // delete entry
        if (idxBytes) {
            idxfd->write(idxBytes.get() + entrySize, shiftSize - entrySize);
            idxfd->seek(-1, SEEK_CUR);    // last valid byte
            FileMgr::getSystemFileMgr()->trunc(idxfd);    // truncate index
        }
    }

    free(dbKey);
}


/******************************************************************************
 * RawLD::linkentry    - links one entry to another
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    destidxoff    - dest offset into .vss
 *    srcidxoff        - source offset into .vss
 */
template <typename SizeType>
void RawStrBase::doLinkEntry_(char const * destkey, char const * srckey)
{ doSetText_<SizeType>(srckey, (std::string("@LINK ") + destkey).c_str()); }

/******************************************************************************
 * RawLD::CreateModule    - Creates new module files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */

signed char RawStrBase::createModule(NormalizedPath const & path) {
    std::string const datFilename(path.str() + ".dat");
    std::string const idxFilename(path.str() + ".idx");

    FileMgr::removeFile(datFilename.c_str());
    FileDesc * const fd =
            FileMgr::getSystemFileMgr()->open(datFilename.c_str(),
                                              FileMgr::CREAT | FileMgr::WRONLY,
                                              FileMgr::IREAD | FileMgr::IWRITE);
    fd->getFd();
    FileMgr::getSystemFileMgr()->close(fd);

    FileMgr::removeFile(idxFilename.c_str());
    FileDesc * const fd2 =
            FileMgr::getSystemFileMgr()->open(idxFilename.c_str(),
                                              FileMgr::CREAT | FileMgr::WRONLY,
                                              FileMgr::IREAD | FileMgr::IWRITE);
    fd2->getFd();
    FileMgr::getSystemFileMgr()->close(fd2);

    return 0;
}

// Explicit instantiations:

template
signed char RawStrBase::findOffset_<std::uint16_t>(
        char const * key,
        StartType * start,
        std::uint16_t * size,
        long away = 0,
        IndexOffsetType * idxoff = nullptr) const;
template
signed char RawStrBase::findOffset_<std::uint32_t>(
        char const * key,
        StartType * start,
        std::uint32_t * size,
        long away = 0,
        IndexOffsetType * idxoff = nullptr) const;

template
void RawStrBase::readText_<std::uint16_t>(StartType start,
                                         std::uint16_t * size,
                                         char ** idxbuf,
                                         std::string & buf) const;
template
void RawStrBase::readText_<std::uint32_t>(StartType start,
                                         std::uint32_t * size,
                                         char ** idxbuf,
                                         std::string & buf) const;

template
void RawStrBase::doSetText_<std::uint16_t>(char const * key,
                                           char const * buf,
                                           long len = -1);
template
void RawStrBase::doSetText_<std::uint32_t>(char const * key,
                                           char const * buf,
                                           long len = -1);

template
void RawStrBase::doLinkEntry_<std::uint16_t>(char const * destkey,
                                             char const * srckey);
template
void RawStrBase::doLinkEntry_<std::uint32_t>(char const * destkey,
                                             char const * srckey);

} /* namespace swordxx */
