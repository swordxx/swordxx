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

template <typename SizeType_>
RawStrBase<SizeType_>::RawStrBase(NormalizedPath const & path,
                                  int fileMode,
                                  bool caseSensitive)
    : caseSensitive(caseSensitive)
{
    lastoff = -1;

    if (fileMode == -1) // try read/write if possible
        fileMode = FileMgr::RDWR;

    idxfd = FileMgr::getSystemFileMgr()->open((path.str() + ".idx").c_str(), fileMode, true);
    datfd = FileMgr::getSystemFileMgr()->open((path.str() + ".dat").c_str(), fileMode, true);

    if (!datfd || datfd->getFd() < 0) {
        // couldn't find datafile but this might be fine if we're
        // merely instantiating a remote InstallMgr SWMgr
        SWLog::getSystemLog()->logDebug("Couldn't open file: %s. errno: %d",
                                        (path.str() + ".dat").c_str(),
                                        errno);
    }
}


/******************************************************************************
 * RawStr Destructor - Cleans up instance of RawStr
 */
template <typename SizeType_>
RawStrBase<SizeType_>::~RawStrBase() {
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
template <typename SizeType_>
std::string RawStrBase<SizeType_>::getIDXBufDat(long ioffset) const {
    if (datfd && datfd->getFd() >= 0) {
        datfd->seek(ioffset, SEEK_SET);
        std::size_t size;
        char ch;
        for (size = 0; datfd->read(&ch, 1) == 1; size++) {
            if ((ch == '\\') || (ch == 10) || (ch == 13))
                break;
        }

        auto const buf(std::make_unique<char[]>(size));
        if (size) {
            datfd->seek(ioffset, SEEK_SET);
            datfd->read(buf.get(), size);
        }
        std::string r(buf.get(), size);
        if (!caseSensitive)
            toupperstr_utf8(r);
        return r;
    } else {
        return "";
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
template <typename SizeType_>
std::string RawStrBase<SizeType_>::getIDXBuf(long ioffset) const {
    IndexOffsetType offset;

    if (idxfd && idxfd->getFd() > 0) {
        idxfd->seek(ioffset, SEEK_SET);
        idxfd->read(&offset, sizeof(offset));

        offset = swapToArch(offset);

        return getIDXBufDat(offset);
    }
    return "";
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
template <typename SizeType_>
signed char RawStrBase<SizeType_>::findOffset(char const * ikey,
                                              StartType * start,
                                              SizeType * size,
                                              long away,
                                              IndexOffsetType * idxoff) const
{
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

            std::string key(ikey);
            if (!caseSensitive)
                toupperstr_utf8(key);

            bool substr = false;

            std::string maxbuf(getIDXBuf(maxoff));

            while (headoff < tailoff) {
                tryoff = (lastoff == -1) ? headoff + ((((tailoff / entrySize) - (headoff / entrySize))) / 2) * entrySize : lastoff;
                lastoff = -1;
                auto trybuf(getIDXBuf(tryoff));

                if (trybuf.empty() && tryoff) {        // In case of extra entry at end of idx (not first entry)
                    if (tryoff > (maxoff / 2)) {
                        tryoff -= entrySize;
                    } else {
                        tryoff += entrySize;
                    }
                    retval = -1;
                    break;
                }

                diff = strcmp(key.c_str(), trybuf.c_str());

                if (!diff)
                    break;

                if (!strncmp(trybuf.c_str(), key.c_str(), key.size())) substr = true;

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
                if (!substr && ((tryoff != maxoff)||(strncmp(key.c_str(), maxbuf.c_str(), key.size())<0))) {
                    awayFromSubstrCheck = true;
                    away--;    // if our entry doesn't startwith our key, prefer the previous entry over the next
                }
            }
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
template <typename SizeType_>
std::string RawStrBase<SizeType_>::readText(StartType istart,
                                            SizeType * isize,
                                            std::string & buf) const
{
    auto const idxbuflocal(getIDXBufDat(istart));
    StartType start = istart;

    do {
        buf.clear();
        buf.resize(++(*isize), '\0');

        datfd->seek(start, SEEK_SET);
        datfd->read(&buf[0u], (int)((*isize) - 1));

        {
            std::size_t ch = buf.find_first_of(10);
            if (ch == std::string::npos) {
                buf.clear();
            } else {
                buf.erase(0u, ++ch);
            }
        }
        // resolve link
        if (!strncmp(buf.c_str(), "@LINK", 5)) {
            for (std::size_t ch = 5; buf[ch]; ++ch) {        // null before nl
                if (buf[ch] == '\n') {
                    buf[ch] = '\0';
                    break;
                }
            }
            findOffset(buf.c_str() + 6, &start, isize);
        }
        else break;
    }
    while (true);    // while we're resolving links

    auto localsize = idxbuflocal.size();
    localsize = (localsize < (*isize - 1)) ? localsize : (*isize - 1);
    return std::string(idxbuflocal.c_str(), localsize);
}


/******************************************************************************
 * RawLD::settext    - Sets text for current offset
 *
 * ENT: key    - key for this entry
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */
template <typename SizeType_>
void RawStrBase<SizeType_>::doSetText(char const * ikey,
                                      char const * buf,
                                      long len)
{
    StartType start, outstart;
    IndexOffsetType idxoff;
    SizeType size;
    SizeType outsize;
    char * tmpbuf = nullptr;
    char * ch = nullptr;

    char errorStatus = findOffset(ikey, &start, &size, 0, &idxoff);
    std::string key(ikey);
    if (!caseSensitive)
        toupperstr_utf8(key);

    len = (len < 0) ? strlen(buf) : len;

    auto const dbKey(getIDXBufDat(start));

    static constexpr auto const entrySize =
            sizeof(StartType) + sizeof(SizeType);

    if (strcmp(key.c_str(), dbKey.c_str()) < 0) {
    }
    else if (strcmp(key.c_str(), dbKey.c_str()) > 0) {
        if (errorStatus != (char)-2)    // not a new file
            idxoff += entrySize;
        else idxoff = 0;
    }
    else if ((!strcmp(key.c_str(), dbKey.c_str())) && (len>0 /*we're not deleting*/)) { // got absolute entry
        do {
            tmpbuf = new char [ size + 2 ];
            std::memset(tmpbuf, 0, size + 2);
            datfd->seek(start, SEEK_SET);
            datfd->read(tmpbuf, (int)(size - 1));

            for (ch = tmpbuf; *ch; ch++) {        // skip over index string
                if (*ch == 10) {
                    ch++;
                    break;
                }
            }
            std::memmove(tmpbuf, ch, size - std::size_t(ch - tmpbuf));

            // resolve link
            if (!strncmp(tmpbuf, "@LINK", 5) && (len)) {
                for (ch = tmpbuf; *ch; ch++) {        // null before nl
                    if (*ch == 10) {
                        *ch = 0;
                        break;
                    }
                }
                findOffset(tmpbuf + entrySize, &start, &size, 0, &idxoff);
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

    auto const outbuf(std::make_unique<char[]>(len + key.size() + 5));
    std::sprintf(outbuf.get(), "%s%c%c", key.c_str(), 13, 10);
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
}


/******************************************************************************
 * RawLD::linkentry    - links one entry to another
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    destidxoff    - dest offset into .vss
 *    srcidxoff        - source offset into .vss
 */
template <typename SizeType_>
void RawStrBase<SizeType_>::doLinkEntry(char const * destkey,
                                        char const * srckey)
{ doSetText(srckey, (std::string("@LINK ") + destkey).c_str()); }

/******************************************************************************
 * RawLD::CreateModule    - Creates new module files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */
template <typename SizeType_>
signed char RawStrBase<SizeType_>::createModule(NormalizedPath const & path) {
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

template class RawStrBase<std::uint16_t>;
template class RawStrBase<std::uint32_t>;

} /* namespace swordxx */
