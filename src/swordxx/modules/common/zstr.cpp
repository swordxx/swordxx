/******************************************************************************
 *
 *  zstr.cpp -    code for class 'zStr'- a module that reads compressed text
 *        files and provides lookup and parsing functions based on
 *        class StrKey
 *
 * $Id$
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

#include "zstr.h"

#include <cassert>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <string>
#include <utility>
#include "../../filemgr.h"
#include "../../stringmgr.h"
#include "../../swlog.h"
#include "../../sysdata.h"
#include "../../utilstr.h"
#include "entriesblk.h"


namespace swordxx {

/******************************************************************************
 * zStr Statics
 */

const int zStr::IDXENTRYSIZE = 8;
const int zStr::ZDXENTRYSIZE = 8;


/******************************************************************************
 * zStr Constructor - Initializes data for instance of zStr
 *
 * ENT:    ipath - path of the directory where data and index files are located.
 */

zStr::zStr(char const * ipath,
           int fileMode,
           long blockCount,
           std::unique_ptr<SWCompress> icomp,
           bool caseSensitive)
    : caseSensitive(caseSensitive)
    , compressor(icomp ? std::move(icomp) : std::make_unique<SWCompress>())
{
    assert(ipath);

    lastoff = -1;

    this->blockCount = blockCount;

    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    idxfd = FileMgr::getSystemFileMgr()->open(formatted("%s.idx", ipath).c_str(), fileMode, true);
    datfd = FileMgr::getSystemFileMgr()->open(formatted("%s.dat", ipath).c_str(), fileMode, true);
    zdxfd = FileMgr::getSystemFileMgr()->open(formatted("%s.zdx", ipath).c_str(), fileMode, true);
    zdtfd = FileMgr::getSystemFileMgr()->open(formatted("%s.zdt", ipath).c_str(), fileMode, true);

    if (!datfd) {
        SWLog::getSystemLog()->logError("%d", errno);
    }

    cacheBlock = nullptr;
    cacheBlockIndex = -1;
    cacheDirty = false;
}


/******************************************************************************
 * zStr Destructor - Cleans up instance of zStr
 */

zStr::~zStr() {
    flushCache();

    FileMgr::getSystemFileMgr()->close(idxfd);
    FileMgr::getSystemFileMgr()->close(datfd);
    FileMgr::getSystemFileMgr()->close(zdxfd);
    FileMgr::getSystemFileMgr()->close(zdtfd);
}


/******************************************************************************
 * zStr::getidxbufdat    - Gets the index string at the given dat offset
 *                NOTE: buf is calloc'd, or if not null, realloc'd and must
 *                    be free'd by calling function
 *
 * ENT:    ioffset    - offset in dat file to lookup
 *        buf        - address of pointer to allocate for storage of string
 */

void zStr::getKeyFromDatOffset(long ioffset, char **buf) const
{
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
 * zStr::getidxbuf    - Gets the index string at the given idx offset
 *                        NOTE: buf is calloc'd, or if not null, realloc'd
 *                            and must be freed by calling function
 *
 * ENT:    ioffset    - offset in idx file to lookup
 *        buf        - address of pointer to allocate for storage of string
 */

void zStr::getKeyFromIdxOffset(long ioffset, char **buf) const
{
    uint32_t offset;

    if (idxfd) {
        idxfd->seek(ioffset, SEEK_SET);
        idxfd->read(&offset, 4);
        offset = swordtoarch32(offset);
        getKeyFromDatOffset(offset, buf);
    }
}


/******************************************************************************
 * zStr::findoffset    - Finds the offset of the key string from the indexes
 *
 * ENT:    key        - key string to lookup
 *        offset    - address to store the starting offset
 *        size        - address to store the size of the entry
 *        away        - number of entries before of after to jump
 *                    (default = 0)
 *
 * RET: error status
 */

signed char zStr::findKeyIndex(const char *ikey, long *idxoff, long away) const
{
    char * maxbuf = nullptr;
    char * trybuf = nullptr;
    char quitflag = 0;
    signed char retval = 0;
    int32_t headoff, tailoff, tryoff = 0, maxoff = 0;
    uint32_t start, size;
    int diff = 0;
    bool awayFromSubstrCheck = false;

    if (idxfd->getFd() >= 0) {
        tailoff = maxoff = idxfd->seek(0, SEEK_END) - IDXENTRYSIZE;
        if (*ikey) {
            headoff = 0;
            auto const ikeyLen = std::strlen(ikey);
            auto const keyBufLen = (ikeyLen + 1u) * 3u;
            auto const key(std::make_unique<char[]>(keyBufLen));
            std::strcpy(key.get(), ikey);
            std::size_t keylen;
            if (!caseSensitive) {
                toupperstr_utf8(key.get(), keyBufLen);
                keylen = std::strlen(key.get());
            } else {
                keylen = ikeyLen;
            }

            bool substr = false;

            getKeyFromIdxOffset(maxoff, &maxbuf);

            while (headoff < tailoff) {
                tryoff = (lastoff == -1) ? headoff + (((((tailoff / IDXENTRYSIZE) - (headoff / IDXENTRYSIZE))) / 2) * IDXENTRYSIZE) : lastoff;
                lastoff = -1;

                getKeyFromIdxOffset(tryoff, &trybuf);

                if (!*trybuf && tryoff) {        // In case of extra entry at end of idx (not first entry)
                    tryoff += (tryoff > (maxoff / 2))?-IDXENTRYSIZE:IDXENTRYSIZE;
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

                if (tailoff == headoff + IDXENTRYSIZE) {
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
        else    { tryoff = 0; }

        idxfd->seek(tryoff, SEEK_SET);

        start = size = 0;
        retval = (idxfd->read(&start, 4) == 4) ? retval : -1;
        retval = (idxfd->read(&size, 4) == 4) ? retval : -1;
        start = swordtoarch32(start);
        size  = swordtoarch32(size);

        if (idxoff)
            *idxoff = tryoff;

        while (away) {
            uint32_t laststart = start;
            uint32_t lastsize = size;
            int32_t lasttry = tryoff;
            tryoff += (away > 0) ? IDXENTRYSIZE : -IDXENTRYSIZE;

            bool bad = false;
            if (((long)(tryoff + (away*IDXENTRYSIZE)) < -IDXENTRYSIZE) || (tryoff + (away*IDXENTRYSIZE) > (maxoff+IDXENTRYSIZE)))
                bad = true;
            else    if (idxfd->seek(tryoff, SEEK_SET) < 0)
                bad = true;
            if (bad) {
                if(!awayFromSubstrCheck)
                    retval = -1;
                start = laststart;
                size = lastsize;
                tryoff = lasttry;
                if (idxoff)
                    *idxoff = tryoff;
                break;
            }
            idxfd->read(&start, 4);
            idxfd->read(&size, 4);
            start = swordtoarch32(start);
            size  = swordtoarch32(size);

            if (idxoff)
                *idxoff = tryoff;


            if (((laststart != start) || (lastsize != size)) && (size))
                away += (away < 0) ? 1 : -1;
        }

        lastoff = tryoff;
    }
    else {
        if (idxoff)
            *idxoff = 0;
        retval = -1;
    }
    return retval;
}


/******************************************************************************
 * zStr::getText    - gets text at a given offset
 *
 * ENT:
 *    offset    - idxoffset where the key is located.
 *    buf        - buffer to store text
 *    idxbuf    - buffer to store index key
 *            NOTE: buffer will be alloc'd / realloc'd and
 *            should be free'd by the client
 *
 */

void zStr::getText(long offset, char **idxbuf, char **buf) const {
    char *ch;
    char * idxbuflocal = nullptr;
    getKeyFromIdxOffset(offset, &idxbuflocal);
    uint32_t start;
    uint32_t size;

    do {
        idxfd->seek(offset, SEEK_SET);
        idxfd->read(&start, 4);
        idxfd->read(&size, 4);
        start = swordtoarch32(start);
        size = swordtoarch32(size);

        *buf = (*buf) ? (char *)realloc(*buf, size*2 + 1) : (char *)malloc(size*2 + 1);
        *idxbuf = (*idxbuf) ? (char *)realloc(*idxbuf, size*2 + 1) : (char *)malloc(size*2 + 1);
        memset(*buf, 0, size + 1);
        memset(*idxbuf, 0, size + 1);
        datfd->seek(start, SEEK_SET);
        datfd->read(*buf, (int)(size));

        for (ch = *buf; *ch; ch++) {        // skip over index string
            if (*ch == 10) {
                ch++;
                break;
            }
        }
        memmove(*buf, ch, size - (unsigned long)(ch-*buf));

        // resolve link
        if (!strncmp(*buf, "@LINK", 5)) {
            for (ch = *buf; *ch; ch++) {        // null before nl
                if (*ch == 10) {
                    *ch = 0;
                    break;
                }
            }
            findKeyIndex(*buf + 6, &offset);
        }
        else break;
    }
    while (true);    // while we're resolving links

    if (idxbuflocal) {
        uint32_t localsize = strlen(idxbuflocal);
        localsize = (localsize < (size - 1)) ? localsize : (size - 1);
        strncpy(*idxbuf, idxbuflocal, localsize);
        (*idxbuf)[localsize] = 0;
        free(idxbuflocal);
    }
    uint32_t block = 0;
    uint32_t entry = 0;
    memmove(&block, *buf, sizeof(uint32_t));
    memmove(&entry, *buf + sizeof(uint32_t), sizeof(uint32_t));
    block = swordtoarch32(block);
    entry = swordtoarch32(entry);
    getCompressedText(block, entry, buf);
}


/******************************************************************************
 * zStr::getCompressedText - Get text entry from a compressed index / zdata
 *         file.
 */

void zStr::getCompressedText(long block, long entry, char **buf) const {

    uint32_t size = 0;

    if (cacheBlockIndex != block) {
        uint32_t start = 0;

        zdxfd->seek(block * ZDXENTRYSIZE, SEEK_SET);
        zdxfd->read(&start, 4);
        zdxfd->read(&size, 4);
        start = swordtoarch32(start);
        size = swordtoarch32(size);

        std::string buf(size + 5u, '\0');
        zdtfd->seek(start, SEEK_SET);
        zdtfd->read(&buf[0u], size);

        flushCache();

        unsigned long len = size;
        buf.resize(size);
        rawZFilter(buf, 0); // 0 = decipher

        compressor->zBuf(&len, &buf[0u]);
        char * rawBuf = compressor->Buf(nullptr, &len);
        cacheBlock = new EntriesBlock(rawBuf, len);
        cacheBlockIndex = block;
    }
    size = cacheBlock->getEntrySize(entry);
    *buf = (*buf) ? (char *)realloc(*buf, size*2 + 1) : (char *)malloc(size*2 + 1);
    strcpy(*buf, cacheBlock->getEntry(entry));
}


/******************************************************************************
 * zLD::settext    - Sets text for current offset
 *
 * ENT: key    - key for this entry
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */

void zStr::setText(const char *ikey, const char *buf, long len) {

    static const char nl[] = {13, 10};

    uint32_t start, outstart;
    uint32_t size, outsize;
    int32_t endoff;
    long idxoff = 0;
    int32_t shiftSize;
    char * dbKey = nullptr;
    char * ch = nullptr;

    len = (len < 0) ? strlen(buf) : len;
    auto const keyBufLen((std::strlen(ikey) + 1u) * 3u);
    auto key(std::make_unique<char[]>(keyBufLen));
    std::strcpy(key.get(), ikey);
    if (!caseSensitive)
        toupperstr_utf8(key.get(), std::strlen(key.get()) * 3u);

    char notFound = findKeyIndex(ikey, &idxoff, 0);
    if (!notFound) {
        getKeyFromIdxOffset(idxoff, &dbKey);
        int diff = std::strcmp(key.get(), dbKey);
        if (diff < 0) {
        }
        else if (diff > 0) {
            idxoff += IDXENTRYSIZE;
        }
        else if ((!diff) && (len > 0 /*we're not deleting*/)) { // got absolute entry
            do {
                idxfd->seek(idxoff, SEEK_SET);
                idxfd->read(&start, 4);
                idxfd->read(&size, 4);
                start = swordtoarch32(start);
                size = swordtoarch32(size);

                auto const tmpbuf(std::make_unique<char[]>(size + 2));
                std::memset(tmpbuf.get(), 0, size + 2);
                datfd->seek(start, SEEK_SET);
                datfd->read(tmpbuf.get(), size);

                for (ch = tmpbuf.get(); *ch; ch++) {        // skip over index string
                    if (*ch == 10) {
                        ch++;
                        break;
                    }
                }
                memmove(tmpbuf.get(), ch, size - (unsigned long)(ch-tmpbuf.get()));

                // resolve link
                if (!strncmp(tmpbuf.get(), "@LINK", 5) && (len)) {
                    for (ch = tmpbuf.get(); *ch; ch++) {        // null before nl
                        if (*ch == 10) {
                            *ch = 0;
                            break;
                        }
                    }
                    findKeyIndex(tmpbuf.get() + IDXENTRYSIZE, &idxoff);
                }
                else break;
            }
            while (true);    // while we're resolving links
        }
    }

    endoff = idxfd->seek(0, SEEK_END);

    shiftSize = endoff - idxoff;

    std::unique_ptr<char[]> idxBytes;
    if (shiftSize > 0) {
        idxBytes = std::make_unique<char[]>(shiftSize);
        idxfd->seek(idxoff, SEEK_SET);
        idxfd->read(idxBytes.get(), shiftSize);
    }

    auto outbuf(std::make_unique<char[]>(len + std::strlen(key.get()) + 5));
    std::sprintf(outbuf.get(), "%s%c%c", key.get(), 13, 10);
    size = std::strlen(outbuf.get());
    if (len > 0) {    // NOT a link
        if (!cacheBlock) {
            flushCache();
            cacheBlock = new EntriesBlock();
            cacheBlockIndex = (zdxfd->seek(0, SEEK_END) / ZDXENTRYSIZE);
        }
        else if (cacheBlock->getCount() >= blockCount) {
            flushCache();
            cacheBlock = new EntriesBlock();
            cacheBlockIndex = (zdxfd->seek(0, SEEK_END) / ZDXENTRYSIZE);
        }
        uint32_t entry = cacheBlock->addEntry(buf);
        cacheDirty = true;
        outstart = archtosword32(cacheBlockIndex);
        outsize = archtosword32(entry);
        std::memcpy(outbuf.get() + size, &outstart, sizeof(uint32_t));
        std::memcpy(outbuf.get() + size + sizeof(uint32_t), &outsize, sizeof(uint32_t));
        size += (sizeof(uint32_t) * 2);
    }
    else {    // link
        std::memcpy(outbuf.get() + size, buf, len);
        size += len;
    }

    start = datfd->seek(0, SEEK_END);

    outstart = archtosword32(start);
    outsize  = archtosword32(size);

    idxfd->seek(idxoff, SEEK_SET);
    if (len > 0) {
        datfd->seek(start, SEEK_SET);
        datfd->write(outbuf.get(), size);

        // add a new line to make data file easier to read in an editor
        datfd->write(&nl, 2);

        idxfd->write(&outstart, 4);
        idxfd->write(&outsize, 4);
        if (idxBytes) {
            idxfd->write(idxBytes.get(), shiftSize);
        }
    }
    else {    // delete entry
        if (idxBytes) {
            idxfd->write(idxBytes.get() + IDXENTRYSIZE, shiftSize - IDXENTRYSIZE);
            idxfd->seek(-1, SEEK_CUR);    // last valid byte
            FileMgr::getSystemFileMgr()->trunc(idxfd);    // truncate index
        }
    }

    free(dbKey);
}


/******************************************************************************
 * zLD::linkentry    - links one entry to another
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    destidxoff    - dest offset into .vss
 *    srcidxoff        - source offset into .vss
 */

void zStr::linkEntry(const char *destkey, const char *srckey) {
    assert(destkey);
    setText(srckey, (std::string("@LINK ") + destkey).c_str());
}


void zStr::flushCache() const {

    static const char nl[] = {13, 10};

    if (cacheBlock) {
        if (cacheDirty) {
            uint32_t start = 0;
            unsigned long size = 0;
            uint32_t outstart = 0, outsize = 0;

            const char *rawBuf = cacheBlock->getRawData(&size);
            compressor->Buf(rawBuf, &size);
            compressor->zBuf(&size);

            std::string buf(size + 5u, '\0');
            /// \bug order of evaluation of function arguments is undefined:
            memcpy(&buf[0u], compressor->zBuf(&size), size); // 1 = encipher
            buf.resize(size);
            rawZFilter(buf, 1); // 1 = encipher

            long zdxSize = zdxfd->seek(0, SEEK_END);
            unsigned long zdtSize = zdtfd->seek(0, SEEK_END);

            if ((cacheBlockIndex * ZDXENTRYSIZE) > (zdxSize - ZDXENTRYSIZE)) {    // New Block
                start = zdtSize;
            }
            else {
                zdxfd->seek(cacheBlockIndex * ZDXENTRYSIZE, SEEK_SET);
                zdxfd->read(&start, 4);
                zdxfd->read(&outsize, 4);
                start = swordtoarch32(start);
                outsize = swordtoarch32(outsize);
                if (start + outsize >= zdtSize) {    // last entry, just overwrite
                    // start is already set
                }
                else    if (size < outsize) {    // middle entry, but smaller, that's fine and let's preserve bigger size
                    size = outsize;
                }
                else {    // middle and bigger-- we have serious problems, for now let's put it at the end = lots of wasted space
                    start = zdtSize;
                }
            }



            outstart = archtosword32(start);
            outsize  = archtosword32((uint32_t)size);

            zdxfd->seek(cacheBlockIndex * ZDXENTRYSIZE, SEEK_SET);
            zdtfd->seek(start, SEEK_SET);
            zdtfd->write(buf.c_str(), size);

            // add a new line to make data file easier to read in an editor
            zdtfd->write(&nl, 2);

            zdxfd->write(&outstart, 4);
            zdxfd->write(&outsize, 4);
        }
        delete cacheBlock;
        cacheBlock = nullptr;
    }
    cacheBlockIndex = -1;
    cacheDirty = false;
}


/******************************************************************************
 * zLD::CreateModule    - Creates new module files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */

signed char zStr::createModule(const char * ipath) {
    assert(ipath);
    std::string path(ipath);
    removeTrailingDirectorySlashes(path);
    auto const extensionPos(path.size() + 1u);
    path.append(".dat");
    FileDesc *fd, *fd2;

    FileMgr::removeFile(path.c_str());
    fd = FileMgr::getSystemFileMgr()->open(path.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd->getFd();
    FileMgr::getSystemFileMgr()->close(fd);

    path.replace(extensionPos, 3u, "idx");
    FileMgr::removeFile(path.c_str());
    fd2 = FileMgr::getSystemFileMgr()->open(path.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd2->getFd();
    FileMgr::getSystemFileMgr()->close(fd2);

    path.replace(extensionPos, 3u, "zdt");
    FileMgr::removeFile(path.c_str());
    fd2 = FileMgr::getSystemFileMgr()->open(path.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd2->getFd();
    FileMgr::getSystemFileMgr()->close(fd2);

    path.replace(extensionPos, 3u, "zdx");
    FileMgr::removeFile(path.c_str());
    fd2 = FileMgr::getSystemFileMgr()->open(path.c_str(), FileMgr::CREAT|FileMgr::WRONLY, FileMgr::IREAD|FileMgr::IWRITE);
    fd2->getFd();
    FileMgr::getSystemFileMgr()->close(fd2);

    return 0;
}

} /* namespace swordxx */
