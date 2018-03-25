/******************************************************************************
 *
 *  zcom4.cpp -    code for class 'zCom4'- a module that reads compressed
 *        commentary files
 *
 * $Id$
 *
 * Copyright 1996-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include "zcom4.h"

#include <cstdio>
#include <fcntl.h>
#include <string>
#include <utility>
#include "../../filemgr.h"
#include "../../keys/versekey.h"


namespace swordxx {

/******************************************************************************
 * zCom4 Constructor - Initializes data for instance of zCom4
 *
 * ENT:    ipath - path to data files
 *        iname - Internal name for module
 *        idesc - Name to display to user for module
 *        iblockType - verse, chapter, book, etc. of index chunks
 *        icomp - Compressor object
 */

zCom4::zCom4(const char *ipath,
             const char *iname,
             const char *idesc,
             BlockType iblockType,
             std::unique_ptr<SWCompress> icomp,
             TextEncoding enc,
             SWTextDirection dir,
             SWTextMarkup mark,
             const char *ilang,
             const char *versification)
    : zVerse4(ipath, -1, iblockType, std::move(icomp))
    , SWCom(iname, idesc, enc, dir, mark, ilang, versification)
{
    blockType = iblockType;
    lastWriteKey = nullptr;
}

/******************************************************************************
 * zCom4 Destructor - Cleans up instance of zCom4
 */

zCom4::~zCom4() {
    flushCache();

    delete lastWriteKey;
}


bool zCom4::isWritable() const {
    return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * zCom4::getRawEntry    - Returns the current verse buffer
 *
 * RET: buffer with verse
 */

std::string zCom4::getRawEntryImpl() const {
    VerseOffsetType start = 0;
    VerseSizeType size = 0;
    BufferNumberType buffnum = 0;
    VerseKey const & key = getVerseKey();

    findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size, &buffnum);
    entrySize = size;        // support getEntrySize call

    std::string entry;

    zReadText(key.getTestament(), start, size, buffnum, entry);
    rawFilter(entry, &key);
    return entry;
}


bool zCom4::sameBlock(VerseKey const & k1, VerseKey const & k2) {
    if (k1.getTestament() != k2.getTestament())
        return false;

    switch (blockType) {
    case VERSEBLOCKS:
        if (k1.getVerse() != k2.getVerse())
            return false;
    case CHAPTERBLOCKS:
        if (k1.getChapter() != k2.getChapter())
            return false;
    case BOOKBLOCKS:
        if (k1.getBook() != k2.getBook())
            return false;
    }
    return true;
}

void zCom4::setEntry(const char *inbuf, long len) {
    VerseKey const & key = getVerseKey();

    // see if we've jumped across blocks since last write
    if (lastWriteKey) {
        if (!sameBlock(*lastWriteKey, key)) {
            flushCache();
        }
        delete lastWriteKey;
    }

    doSetText(key.getTestament(), key.getTestamentIndex(), inbuf, len);

    lastWriteKey = static_cast<VerseKey *>(key.clone().release());
}


void zCom4::linkEntry(SWKey const & inkey) {
    VerseKey const & destkey = getVerseKey();
    VerseKey const & srckey = getVerseKey(&inkey);

    doLinkEntry(destkey.getTestament(), destkey.getTestamentIndex(), srckey.getTestamentIndex());

    if (&inkey != &srckey) // free our key if we created a VerseKey
        delete &srckey;
}

/******************************************************************************
 * zCom4::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void zCom4::deleteEntry() {
    VerseKey const & key = getVerseKey();
    doSetText(key.getTestament(), key.getTestamentIndex(), "");
}


/******************************************************************************
 * zCom4::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */

void zCom4::increment(int steps) {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    VerseKey const * tmpkey = &getVerseKey();

    findOffset(tmpkey->getTestament(), tmpkey->getTestamentIndex(), &start, &size, &buffnum);

    SWKey lastgood = *tmpkey;
    while (steps) {
        VerseOffsetType laststart = start;
        unsigned long lastsize = size;
        SWKey lasttry = *tmpkey;
        if (steps > 0) {
            key->increment();
        } else {
            key->decrement();
        }
        tmpkey = &getVerseKey();

        if ((error = key->popError())) {
            key->positionFrom(lastgood);
            break;
        }
        long index = tmpkey->getTestamentIndex();
        findOffset(tmpkey->getTestament(), index, &start, &size, &buffnum);
        if (
            (((laststart != start) || (lastsize != size))    // we're a different entry
//                && (start > 0)
                && (size))    // and we actually have a size
                ||(!skipConsecutiveLinks)) {    // or we don't want to skip consecutive links
            steps += (steps < 0) ? 1 : -1;
            lastgood.positionFrom(*tmpkey);
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

bool zCom4::isLinked(const SWKey *k1, const SWKey *k2) const {
    VerseOffsetType start1, start2;
    VerseSizeType size1, size2;
    BufferNumberType buffnum1, buffnum2;
    VerseKey const & vk1 = getVerseKey(k1);
    VerseKey const & vk2 = getVerseKey(k2);
    if (vk1.getTestament() != vk2.getTestament()) return false;

    findOffset(vk1.getTestament(), vk1.getTestamentIndex(), &start1, &size1, &buffnum1);
    findOffset(vk2.getTestament(), vk2.getTestamentIndex(), &start2, &size2, &buffnum2);
    return start1 == start2 && buffnum1 == buffnum2;
}

bool zCom4::hasEntry(const SWKey *k) const {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    VerseKey const & vk = getVerseKey(k);
    findOffset(vk.getTestament(), vk.getTestamentIndex(), &start, &size, &buffnum);
    return size;
}

} /* namespace swordxx */
