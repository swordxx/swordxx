/******************************************************************************
 *
 *  zcom.cpp -    code for class 'zCom'- a module that reads raw commentary
 *        files
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

#include "zcom.h"

#include <cstdio>
#include <fcntl.h>
#include <string>
#include <utility>
#include "../../filemgr.h"
#include "../../keys/versekey.h"


namespace swordxx {

/******************************************************************************
 * zCom Constructor - Initializes data for instance of zCom
 *
 * ENT:    ipath - path to data files
 *        iname - Internal name for module
 *        idesc - Name to display to user for module
 *        iblockType - verse, chapter, book, etc. of index chunks
 *        icomp - Compressor object
 */

zCom::zCom(const char *ipath,
           const char *iname,
           const char *idesc,
           BlockType iblockType,
           std::unique_ptr<SWCompress> icomp,
           TextEncoding enc,
           SWTextDirection dir,
           SWTextMarkup mark,
           const char *ilang,
           const char *versification)
    : zVerse(ipath, -1, iblockType, std::move(icomp))
    , SWCom(iname, idesc, enc, dir, mark, ilang, versification)
{
    blockType = iblockType;
    lastWriteKey = nullptr;
}

/******************************************************************************
 * zCom Destructor - Cleans up instance of zCom
 */

zCom::~zCom() {
    flushCache();

    delete lastWriteKey;
}


bool zCom::isWritable() const {
    return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * zCom::getRawEntry    - Returns the current verse buffer
 *
 * RET: buffer with verse
 */

std::string &zCom::getRawEntryBuf() const {
    VerseOffsetType start = 0;
    VerseSizeType size = 0;
    BufferNumberType buffnum = 0;
    VerseKey const & key = getVerseKey();

    findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size, &buffnum);
    entrySize = size;        // support getEntrySize call

    entryBuf = "";

    zReadText(key.getTestament(), start, size, buffnum, entryBuf);
    rawFilter(entryBuf, &key);

//    if (!isUnicode())
        prepText(entryBuf);

    return entryBuf;
}


bool zCom::sameBlock(VerseKey const & k1, VerseKey const & k2) {
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

void zCom::setEntry(const char *inbuf, long len) {
    VerseKey const & key = getVerseKey();

    // see if we've jumped across blocks since last write
    if (lastWriteKey) {
        if (!sameBlock(*lastWriteKey, key)) {
            flushCache();
        }
        delete lastWriteKey;
    }

    doSetText(key.getTestament(), key.getTestamentIndex(), inbuf, len);

    lastWriteKey = static_cast<VerseKey *>(key.clone().release()); // must delete
}


void zCom::linkEntry(SWKey const & inkey) {
    VerseKey const & destkey = getVerseKey();
    VerseKey & srckey = getVerseKey(&inkey);

    doLinkEntry(destkey.getTestament(), destkey.getTestamentIndex(), srckey.getTestamentIndex());

    if (&inkey != &srckey) // free our key if we created a VerseKey
        delete &srckey;
}

/******************************************************************************
 * zCom::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void zCom::deleteEntry() {
    VerseKey const & key = getVerseKey();
    doSetText(key.getTestament(), key.getTestamentIndex(), "");
}


/******************************************************************************
 * zCom::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */

void zCom::increment(int steps) {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    VerseKey const * tmpkey = &getVerseKey();

    findOffset(tmpkey->getTestament(), tmpkey->getTestamentIndex(), &start, &size, &buffnum);

    SWKey lastgood = *tmpkey;
    while (steps) {
        VerseOffsetType laststart = start;
        VerseSizeType lastsize = size;
        SWKey lasttry = *tmpkey;
        (steps > 0) ? ++(*key) : --(*key);
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

bool zCom::isLinked(const SWKey *k1, const SWKey *k2) const {
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

bool zCom::hasEntry(const SWKey *k) const {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    VerseKey const & vk = getVerseKey(k);
    findOffset(vk.getTestament(), vk.getTestamentIndex(), &start, &size, &buffnum);
    return size;
}

} /* namespace swordxx */
