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
#include "../../ShareRef.h"


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
    , blockType(iblockType)
{}

/******************************************************************************
 * zCom4 Destructor - Cleans up instance of zCom4
 */

zCom4::~zCom4() {
    flushCache();
}


bool zCom4::isWritable() const {
    return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode() & FileMgr::RDWR) == FileMgr::RDWR));
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
    auto const key_(getVerseKey());

    findOffset(key_->getTestament(), key_->getTestamentIndex(), &start, &size, &buffnum);

    auto entry(zReadText(key_->getTestament(), start, size, buffnum));
    rawFilter(entry, key_.get());
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
    auto const key_(getVerseKey());

    // see if we've jumped across blocks since last write
    if (lastWriteKey) {
        if (!sameBlock(*lastWriteKey, *key_)) {
            flushCache();
        }
        lastWriteKey.reset();
    }

    doSetText(key_->getTestament(), key_->getTestamentIndex(), inbuf, len);

    lastWriteKey = std::static_pointer_cast<VerseKey const>(key_->clone());
}


void zCom4::linkEntry(SWKey const & inkey) {
    auto const destkey(getVerseKey());
    auto const srckey(getVerseKey(shareRef(inkey)));

    doLinkEntry(destkey->getTestament(),
                destkey->getTestamentIndex(),
                srckey->getTestamentIndex());
}

/******************************************************************************
 * zCom4::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void zCom4::deleteEntry() {
    auto const key_(getVerseKey());
    doSetText(key_->getTestament(), key_->getTestamentIndex(), "");
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
    auto tmpkey(getVerseKey());

    findOffset(tmpkey->getTestament(), tmpkey->getTestamentIndex(), &start, &size, &buffnum);

    auto lastgood(tmpkey);
    while (steps) {
        auto const laststart = start;
        auto const lastsize = size;
        if (steps > 0) {
            getKey()->increment();
        } else {
            getKey()->decrement();
        }
        tmpkey = getVerseKey();

        if ((error = getKey()->popError())) {
            getKey()->positionFrom(*lastgood);
            break;
        }
        auto const index = tmpkey->getTestamentIndex();
        findOffset(tmpkey->getTestament(), index, &start, &size, &buffnum);
        if (
            (((laststart != start) || (lastsize != size))    // we're a different entry
//                && (start > 0)
                && (size))    // and we actually have a size
                ||(!isSkipConsecutiveLinks())) {    // or we don't want to skip consecutive links
            steps += (steps < 0) ? 1 : -1;
            lastgood = tmpkey;
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

bool zCom4::isLinked(SWKey const & k1, SWKey const & k2) const {
    VerseOffsetType start1, start2;
    VerseSizeType size1, size2;
    BufferNumberType buffnum1, buffnum2;
    auto const vk1(getVerseKey(shareRef(k1)));
    auto const vk2(getVerseKey(shareRef(k2)));
    if (vk1->getTestament() != vk2->getTestament())
        return false;

    findOffset(vk1->getTestament(), vk1->getTestamentIndex(), &start1, &size1, &buffnum1);
    findOffset(vk2->getTestament(), vk2->getTestamentIndex(), &start2, &size2, &buffnum2);
    return start1 == start2 && buffnum1 == buffnum2;
}

bool zCom4::hasEntry(SWKey const & k) const {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    auto const vk(getVerseKey(shareRef(k)));
    findOffset(vk->getTestament(), vk->getTestamentIndex(), &start, &size, &buffnum);
    return size;
}

} /* namespace swordxx */
