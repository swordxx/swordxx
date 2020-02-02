/******************************************************************************
 *
 *  rawstr.cpp -    code for class 'RawStr'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class StrKey
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
#include <cstring>
#include <fcntl.h>
#include <string>
#include "../../filemgr.h"
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
    : m_caseSensitive(caseSensitive)
{
    m_lastoff = -1;

    if (fileMode == -1) // try read/write if possible
        fileMode = FileMgr::RDWR;

    auto const fileMgr(FileMgr::getSystemFileMgr());
    idxfd = fileMgr->open((path.str() + ".idx").c_str(), fileMode, true);
    datfd = fileMgr->open((path.str() + ".dat").c_str(), fileMode, true);

    if (!datfd || datfd->getFd() < 0) {
        // couldn't find datafile but this might be fine if we're
        // merely instantiating a remote InstallMgr SWMgr
        SWLog::getSystemLog()->logDebug("Couldn't open file: %s. errno: %d",
                                        (path.str() + ".dat").c_str(),
                                        errno);
    }
}


template <typename SizeType_> RawStrBase<SizeType_>::~RawStrBase() = default;


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
    std::string r;
    if (datfd && datfd->getFd() >= 0) {
        datfd->seek(ioffset, SEEK_SET);
        /// \todo Read more than a byte of data at once:
        for (;;) {
            char ch;
            /// \todo Handle EAGAIN etc errno:
            if (datfd->read(&ch, 1) != 1)
                break;
            if ((ch == '\\') || (ch == 10) || (ch == 13))
                break;
            r += ch;
        }
        if (!m_caseSensitive)
            return utf8ToUpper(r);
    }
    return r;
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
signed char RawStrBase<SizeType_>::findOffset(std::string_view ikey,
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
        if (!ikey.empty() && retval != -2) {
            headoff = 0;

            std::string key(ikey);
            if (!m_caseSensitive)
                key = utf8ToUpper(key);

            bool substr = false;

            std::string maxbuf(getIDXBuf(maxoff));

            while (headoff < tailoff) {
                tryoff = (m_lastoff == -1) ? headoff + ((((tailoff / entrySize) - (headoff / entrySize))) / 2) * entrySize : m_lastoff;
                m_lastoff = -1;
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

                diff = key.compare(trybuf);

                if (!diff)
                    break;

                if (startsWith(trybuf, key)) substr = true;

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
                if (!substr && ((tryoff != maxoff)||(std::strncmp(key.c_str(), maxbuf.c_str(), key.size())<0))) {
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

        m_lastoff = tryoff;
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
                                            SizeType isize,
                                            std::string & buf) const
{
    using namespace std::literals::string_view_literals;

    auto idxbuflocal(getIDXBufDat(istart));
    StartType start = istart;

    for (;;) { // while resolving links
        buf.clear();
        buf.resize(isize);

        datfd->seek(start, SEEK_SET);
        datfd->read(buf.data(), isize);

        {
            std::size_t ch = buf.find_first_of('\n');
            if (ch == std::string::npos) {
                buf.clear();
                break;
            } else {
                buf.erase(0u, ++ch);
            }
        }

        if (!startsWith(buf, "@LINK"sv))
            break;

        // Resolve link:
        {
            std::size_t ch = buf.find_first_of('\n', 5u);
            if (ch != std::string::npos)
                buf.resize(ch);
        }
        std::string_view bufSv(buf);
        bufSv.remove_prefix(6u);
        findOffset(bufSv, &start, &isize);
    }

    auto const localsize = idxbuflocal.size();
    idxbuflocal.resize((localsize < isize) ? localsize : isize);
    return idxbuflocal;
}


/******************************************************************************
 * RawLD::settext    - Sets text for current offset
 *
 * ENT: key    - key for this entry
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */
template <typename SizeType_>
void RawStrBase<SizeType_>::doSetText(char const * ikey, std::string_view text){
    using namespace std::literals::string_view_literals;

    StartType start, outstart;
    IndexOffsetType idxoff;
    SizeType size;
    SizeType outsize;
    char * tmpbuf = nullptr;
    char * ch = nullptr;

    char errorStatus = findOffset(ikey, &start, &size, 0, &idxoff);
    std::string key(ikey);
    if (!m_caseSensitive)
        key = utf8ToUpper(key);

    auto const dbKey(getIDXBufDat(start));

    static constexpr auto const entrySize =
            sizeof(StartType) + sizeof(SizeType);

    auto const keyCompare(key.compare(dbKey));
    if (keyCompare > 0) {
        if (errorStatus != (char)-2)    // not a new file
            idxoff += entrySize;
        else idxoff = 0;
    }
    else if ((!keyCompare) && (!text.empty() /*we're not deleting*/)) { // got absolute entry
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
            if (startsWith(tmpbuf, "@LINK"sv) && !text.empty()) {
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

    auto const outbuf(std::make_unique<char[]>(text.size() + key.size() + 5));
    std::sprintf(outbuf.get(), "%s%c%c", key.c_str(), 13, 10);
    size = std::strlen(outbuf.get());
    std::memcpy(outbuf.get() + size, text.data(), text.size());
    size = outsize = size + text.size();

    start = outstart = datfd->seek(0, SEEK_END);

    outstart = swapFromArch(start);
    outsize  = swapFromArch(size);

    idxfd->seek(idxoff, SEEK_SET);
    if (!text.empty()) {
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
            idxfd->truncate(); // truncate index
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
{ doSetText(srckey, std::string("@LINK ") + destkey); }

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

    auto const fileMgr(FileMgr::getSystemFileMgr());

    FileMgr::removeFile(datFilename.c_str());
    fileMgr->touch(datFilename.c_str());

    FileMgr::removeFile(idxFilename.c_str());
    fileMgr->touch(idxFilename.c_str());
    return 0;
}

// Explicit instantiations:

template class RawStrBase<std::uint16_t>;
template class RawStrBase<std::uint32_t>;

} /* namespace swordxx */
