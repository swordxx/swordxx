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
#include <cstring>
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
    : m_caseSensitive(caseSensitive)
    , m_compressor(icomp ? std::move(icomp) : std::make_unique<SWCompress>())
{
    assert(ipath);

    m_lastoff = -1;

    this->m_blockCount = blockCount;

    if (fileMode == -1) { // try read/write if possible
        fileMode = FileMgr::RDWR;
    }

    auto const fileMgr(FileMgr::getSystemFileMgr());
    idxfd = fileMgr->open(formatted("%s.idx", ipath).c_str(), fileMode, true);
    datfd = fileMgr->open(formatted("%s.dat", ipath).c_str(), fileMode, true);
    zdxfd = fileMgr->open(formatted("%s.zdx", ipath).c_str(), fileMode, true);
    zdtfd = fileMgr->open(formatted("%s.zdt", ipath).c_str(), fileMode, true);

    if (!zdtfd || zdtfd->getFd() < 0) {
        // couldn't find datafile but this might be fine if we're
        // merely instantiating a remote InstallMgr SWMgr
        SWLog::getSystemLog()->logDebug("Couldn't open file: %s. errno: %d",
                                        formatted("%s.zdt", ipath).c_str(),
                                        errno);
    }

    m_cacheBlockIndex = -1;
    m_cacheDirty = false;
}


/******************************************************************************
 * zStr Destructor - Cleans up instance of zStr
 */

zStr::~zStr() { flushCache(); }


/******************************************************************************
 * zStr::getidxbufdat    - Gets the index string at the given dat offset
 *                NOTE: buf is std::calloc'd, or if not null, realloc'd and must
 *                    be free'd by calling function
 *
 * ENT:    ioffset    - offset in dat file to lookup
 *        buf        - address of pointer to allocate for storage of string
 */

std::string zStr::getKeyFromDatOffset(long ioffset) const
{
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
        if (!m_caseSensitive)
            toupperstr_utf8(r);
        return r;
    } else {
        return "";
    }
}


/******************************************************************************
 * zStr::getidxbuf    - Gets the index string at the given idx offset
 *                        NOTE: buf is std::calloc'd, or if not null, realloc'd
 *                            and must be freed by calling function
 *
 * ENT:    ioffset    - offset in idx file to lookup
 *        buf        - address of pointer to allocate for storage of string
 */

std::string zStr::getKeyFromIdxOffset(long ioffset) const
{
    uint32_t offset;

    if (idxfd && idxfd->getFd() > 0) {
        idxfd->seek(ioffset, SEEK_SET);
        idxfd->read(&offset, 4);
        offset = swordtoarch32(offset);
        return getKeyFromDatOffset(offset);
    }
    return "";
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
            std::string key(ikey);
            if (!m_caseSensitive)
                toupperstr_utf8(key);

            bool substr = false;

            auto const maxbuf(getKeyFromIdxOffset(maxoff));

            while (headoff < tailoff) {
                tryoff = (m_lastoff == -1) ? headoff + (((((tailoff / IDXENTRYSIZE) - (headoff / IDXENTRYSIZE))) / 2) * IDXENTRYSIZE) : m_lastoff;
                m_lastoff = -1;

                auto const trybuf(getKeyFromIdxOffset(tryoff));

                if (trybuf.empty() && tryoff) {        // In case of extra entry at end of idx (not first entry)
                    tryoff += (tryoff > (maxoff / 2))?-IDXENTRYSIZE:IDXENTRYSIZE;
                    retval = -1;
                    break;
                }

                diff = key.compare(trybuf);

                if (!diff)
                    break;

                if (startsWith(trybuf, key)) substr = true;

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
                if (!substr && ((tryoff != maxoff)||(std::strncmp(key.c_str(), maxbuf.c_str(), key.size())<0))) {
                    awayFromSubstrCheck = true;
                    away--;    // if our entry doesn't startwith our key, prefer the previous entry over the next
                }
            }
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

        m_lastoff = tryoff;
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

void zStr::getText(long offset, std::string & idxbuf, std::string & buf) const {
    using namespace std::literals::string_view_literals;

    auto const idxbuflocal(getKeyFromIdxOffset(offset));
    uint32_t start;
    uint32_t size;

    do {
        idxfd->seek(offset, SEEK_SET);
        idxfd->read(&start, 4);
        idxfd->read(&size, 4);
        start = swordtoarch32(start);
        size = swordtoarch32(size);

        buf.assign(size * 2u + 1u, '\0');
        idxbuf.assign(size * 2u + 1u, '\0');
        datfd->seek(start, SEEK_SET);
        datfd->read(buf.data(), size);

        char const * ch;
        for (ch = buf.c_str(); *ch; ch++) {        // skip over index string
            if (*ch == 10) {
                ch++;
                break;
            }
        }
        std::memmove(buf.data(), ch, size - static_cast<std::size_t>(ch - buf.c_str()));

        // resolve link
        if (startsWith(buf, "@LINK"sv)) {
            if (auto nlPos = buf.find('\n', 5u); nlPos != std::string::npos)
                buf.resize(nlPos);
            findKeyIndex(buf.c_str() + 6, &offset);
        }
        else break;
    }
    while (true);    // while we're resolving links

    auto localsize = idxbuflocal.size();
    localsize = (localsize < (size - 1)) ? localsize : (size - 1);
    std::strncpy(idxbuf.data(), idxbuflocal.c_str(), localsize);
    idxbuf.resize(localsize);

    uint32_t block = 0;
    uint32_t entry = 0;
    std::memmove(&block, buf.c_str(), sizeof(uint32_t));
    std::memmove(&entry, buf.c_str() + sizeof(uint32_t), sizeof(uint32_t));
    block = swordtoarch32(block);
    entry = swordtoarch32(entry);
    getCompressedText(block, entry, buf);
}


/******************************************************************************
 * zStr::getCompressedText - Get text entry from a compressed index / zdata
 *         file.
 */

void zStr::getCompressedText(long block, long entry, std::string & buf) const {

    uint32_t size = 0;

    if (m_cacheBlockIndex != block) {
        uint32_t start = 0;

        zdxfd->seek(block * ZDXENTRYSIZE, SEEK_SET);
        zdxfd->read(&start, 4);
        zdxfd->read(&size, 4);
        start = swordtoarch32(start);
        size = swordtoarch32(size);

        std::string buf2(size + 5u, '\0');
        zdtfd->seek(start, SEEK_SET);
        zdtfd->read(&buf2[0u], size);

        flushCache();

        unsigned long len = size;
        buf2.resize(size);
        rawZFilter(buf2, 0); // 0 = decipher

        m_compressor->zBuf(&len, &buf2[0u]);
        char * rawBuf = m_compressor->Buf(nullptr, &len);
        m_cacheBlock = std::make_unique<EntriesBlock>(rawBuf, len);
        m_cacheBlockIndex = block;
    }
    size = (*m_cacheBlock)[entry].size();
    buf = (*m_cacheBlock)[entry].c_str();
}


/******************************************************************************
 * zLD::settext    - Sets text for current offset
 *
 * ENT: key    - key for this entry
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */

void zStr::setText(const char *ikey, std::string_view text) {
    using namespace std::literals::string_view_literals;

    static const char nl[] = {13, 10};

    uint32_t start, outstart;
    uint32_t size, outsize;
    int32_t endoff;
    long idxoff = 0;
    int32_t shiftSize;
    char * ch = nullptr;

    std::string key(ikey);
    if (!m_caseSensitive)
        toupperstr_utf8(key);

    char notFound = findKeyIndex(ikey, &idxoff, 0);
    if (!notFound) {
        auto diff = key.compare(getKeyFromIdxOffset(idxoff));
        if (diff < 0) {
        }
        else if (diff > 0) {
            idxoff += IDXENTRYSIZE;
        }
        else if ((!diff) && (!text.empty() /*we're not deleting*/)) { // got absolute entry
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
                std::memmove(tmpbuf.get(), ch, size - (unsigned long)(ch-tmpbuf.get()));

                // resolve link
                if (startsWith(tmpbuf.get(), "@LINK"sv) && !text.empty()) {
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

    auto outbuf(std::make_unique<char[]>(text.size() + key.size() + 5));
    std::sprintf(outbuf.get(), "%s%c%c", key.c_str(), 13, 10);
    size = std::strlen(outbuf.get());
    if (!text.empty()) {    // NOT a link
        if (!m_cacheBlock) {
            flushCache();
            m_cacheBlock = std::make_unique<EntriesBlock>();
            m_cacheBlockIndex = (zdxfd->seek(0, SEEK_END) / ZDXENTRYSIZE);
        }
        else if (m_cacheBlock->numEntries() >= m_blockCount) {
            flushCache();
            m_cacheBlock = std::make_unique<EntriesBlock>();
            m_cacheBlockIndex = (zdxfd->seek(0, SEEK_END) / ZDXENTRYSIZE);
        }
        uint32_t entry = m_cacheBlock->addEntry(text);
        m_cacheDirty = true;
        outstart = archtosword32(m_cacheBlockIndex);
        outsize = archtosword32(entry);
        std::memcpy(outbuf.get() + size, &outstart, sizeof(uint32_t));
        std::memcpy(outbuf.get() + size + sizeof(uint32_t), &outsize, sizeof(uint32_t));
        size += (sizeof(uint32_t) * 2);
    }
    else {    // link
        std::memcpy(outbuf.get() + size, text.data(), text.size());
        size += text.size();
    }

    start = datfd->seek(0, SEEK_END);

    outstart = archtosword32(start);
    outsize  = archtosword32(size);

    idxfd->seek(idxoff, SEEK_SET);
    if (!text.empty()) {
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
            idxfd->truncate(); // truncate index
        }
    }
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
    setText(srckey, std::string("@LINK ") + destkey);
}


void zStr::flushCache() const {

    static const char nl[] = {13, 10};

    if (m_cacheBlock) {
        if (m_cacheDirty) {
            uint32_t start = 0;
            uint32_t outstart = 0, outsize = 0;
            std::size_t size;

            {
                auto const rawBuf(m_cacheBlock->serialized());
                size = rawBuf.size();
                m_compressor->Buf(rawBuf.data(), &size);
                m_compressor->zBuf(&size);
            }

            std::string buf(size + 5u, '\0');
            /// \bug order of evaluation of function arguments is undefined:
            std::memcpy(&buf[0u], m_compressor->zBuf(&size), size); // 1 = encipher
            buf.resize(size);
            rawZFilter(buf, 1); // 1 = encipher

            long zdxSize = zdxfd->seek(0, SEEK_END);
            unsigned long zdtSize = zdtfd->seek(0, SEEK_END);

            if ((m_cacheBlockIndex * ZDXENTRYSIZE) > (zdxSize - ZDXENTRYSIZE)) {    // New Block
                start = zdtSize;
            }
            else {
                zdxfd->seek(m_cacheBlockIndex * ZDXENTRYSIZE, SEEK_SET);
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

            zdxfd->seek(m_cacheBlockIndex * ZDXENTRYSIZE, SEEK_SET);
            zdtfd->seek(start, SEEK_SET);
            zdtfd->write(buf.c_str(), size);

            // add a new line to make data file easier to read in an editor
            zdtfd->write(&nl, 2);

            zdxfd->write(&outstart, 4);
            zdxfd->write(&outsize, 4);
        }
        m_cacheBlock.reset();
    }
    m_cacheBlockIndex = -1;
    m_cacheDirty = false;
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

    auto const fileMgr(FileMgr::getSystemFileMgr());

    FileMgr::removeFile(path.c_str());
    fileMgr->touch(path.c_str());

    path.replace(extensionPos, 3u, "idx");
    FileMgr::removeFile(path.c_str());
    fileMgr->touch(path.c_str());

    path.replace(extensionPos, 3u, "zdt");
    FileMgr::removeFile(path.c_str());
    fileMgr->touch(path.c_str());

    path.replace(extensionPos, 3u, "zdx");
    FileMgr::removeFile(path.c_str());
    fileMgr->touch(path.c_str());
    return 0;
}

} /* namespace swordxx */
