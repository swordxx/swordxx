/******************************************************************************
 *
 *  ztext.cpp -    code for class 'zText'- a module that reads compressed text
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

#include "ztextbase.h"

#include <cctype>
#include <cstdio>
#include <fcntl.h>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../sysdata.h"


namespace swordxx {

/******************************************************************************
 * zText Constructor - Initializes data for instance of zText
 *
 * ENT:    ipath - path to data files
 *        iname - Internal name for module
 *        idesc - Name to display to user for module
 *        iblockType - verse, chapter, book, etc. of index chunks
 *        icomp - Compressor object
 */

template <typename BaseZVerse>
zTextBase<BaseZVerse>::zTextBase(const char *ipath, const char *iname, const char *idesc, BlockType iblockType, SWCompress *icomp, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char *ilang, const char *versification)
        : BaseZVerse(ipath, FileMgr::RDWR, iblockType, icomp), SWText(iname, idesc, enc, dir, mark, ilang, versification) {
    blockType = iblockType;
    lastWriteKey = nullptr;
}


/******************************************************************************
 * zText Destructor - Cleans up instance of zText
 */
template <typename BaseZVerse>
zTextBase<BaseZVerse>::~zTextBase()
{
    this->flushCache();

    delete lastWriteKey;
}

template <typename BaseZVerse>
bool zTextBase<BaseZVerse>::isWritable() const {
    return (this->idxfp[0]->getFd() > 0)
           && ((this->idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR);
}


/******************************************************************************
 * zText::getRawEntry    - Returns the current verse buffer
 *
 * RET: buffer with verse
 */
template <typename BaseZVerse>
std::string & zTextBase<BaseZVerse>::getRawEntryBuf() const {
    VerseOffsetType start = 0;
    VerseSizeType size = 0;
    BufferNumberType buffnum = 0;
    VerseKey const & key = getVerseKey();

    this->findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size, &buffnum);
    entrySize = size;        // support getEntrySize call

    entryBuf = "";

    this->zReadText(key.getTestament(), start, size, buffnum, entryBuf);
    rawFilter(entryBuf, &key);

//    if (!isUnicode())
        prepText(entryBuf);

    return entryBuf;
}

template <typename BaseZVerse>
bool zTextBase<BaseZVerse>::sameBlock(VerseKey const & k1, VerseKey const & k2) {
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

template <typename BaseZVerse>
void zTextBase<BaseZVerse>::setEntry(const char *inbuf, long len) {
    VerseKey const & key = getVerseKey();

    // see if we've jumped across blocks since last write
    if (lastWriteKey) {
        if (!sameBlock(*lastWriteKey, key)) {
            this->flushCache();
        }
        delete lastWriteKey;
    }

    this->doSetText(key.getTestament(), key.getTestamentIndex(), inbuf, len);

    lastWriteKey = (VerseKey *)key.clone();    // must delete
}

template <typename BaseZVerse>
void zTextBase<BaseZVerse>::linkEntry(const SWKey *inkey) {
    VerseKey const & destkey = getVerseKey();
    VerseKey const & srckey = getVerseKey(inkey);
    this->doLinkEntry(destkey.getTestament(), destkey.getTestamentIndex(), srckey.getTestamentIndex());
}


/******************************************************************************
 * zFiles::deleteEntry    - deletes this entry
 *
 */
template <typename BaseZVerse>
void zTextBase<BaseZVerse>::deleteEntry() {
    VerseKey const & key = getVerseKey();
    this->doSetText(key.getTestament(), key.getTestamentIndex(), "");
}


/******************************************************************************
 * zText::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 */
template <typename BaseZVerse>
void zTextBase<BaseZVerse>::increment(int steps) {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    VerseKey const * tmpkey = &getVerseKey();

    this->findOffset(tmpkey->getTestament(), tmpkey->getTestamentIndex(), &start, &size, &buffnum);

    SWKey lastgood = *tmpkey;
    while (steps) {
        VerseOffsetType laststart = start;
        VerseSizeType lastsize = size;
        SWKey lasttry = *tmpkey;
        (steps > 0) ? ++(*key) : --(*key);
        tmpkey = &getVerseKey();

        if ((error = key->popError())) {
            *key = lastgood;
            break;
        }
        long index = tmpkey->getTestamentIndex();
        this->findOffset(tmpkey->getTestament(), index, &start, &size, &buffnum);

        if (
            (
                ((laststart != start) || (lastsize != size))    // we're a different entry
//                && (start > 0)
                && (size)    // and we actually have a size
            )
            || !skipConsecutiveLinks
        ) {    // or we don't want to skip consecutive links
            steps += (steps < 0) ? 1 : -1;
            lastgood = *tmpkey;
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

template <typename BaseZVerse>
bool zTextBase<BaseZVerse>::isLinked(const SWKey *k1, const SWKey *k2) const {
    VerseOffsetType start1, start2;
    VerseSizeType size1, size2;
    BufferNumberType buffnum1, buffnum2;
    VerseKey const & vk1 = getVerseKey(k1);
    VerseKey const & vk2 = getVerseKey(k2);
    if (vk1.getTestament() != vk2.getTestament()) return false;

    this->findOffset(vk1.getTestament(), vk1.getTestamentIndex(), &start1, &size1, &buffnum1);
    this->findOffset(vk2.getTestament(), vk2.getTestamentIndex(), &start2, &size2, &buffnum2);
    return start1 == start2 && buffnum1 == buffnum2;
}

template <typename BaseZVerse>
bool zTextBase<BaseZVerse>::hasEntry(const SWKey *k) const {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    VerseKey const & vk = getVerseKey(k);
    this->findOffset(vk.getTestament(), vk.getTestamentIndex(), &start, &size, &buffnum);
    return size;
}

// Explicit instantions:

template
zTextBase<zVerse>::zTextBase(
        char const * ipath,
        char const * iname = nullptr,
        char const * idesc = nullptr,
        BlockType blockType = CHAPTERBLOCKS,
        SWCompress * icomp = nullptr,
        TextEncoding encoding = ENC_UNKNOWN,
        SWTextDirection dir = DIRECTION_LTR,
        SWTextMarkup markup = FMT_UNKNOWN,
        char const * ilang = nullptr,
        char const * versification = "KJV");
template
zTextBase<zVerse4>::zTextBase(
        char const * ipath,
        char const * iname = nullptr,
        char const * idesc = nullptr,
        BlockType blockType = CHAPTERBLOCKS,
        SWCompress * icomp = nullptr,
        TextEncoding encoding = ENC_UNKNOWN,
        SWTextDirection dir = DIRECTION_LTR,
        SWTextMarkup markup = FMT_UNKNOWN,
        char const * ilang = nullptr,
        char const * versification = "KJV");

template zTextBase<zVerse>::~zTextBase();
template zTextBase<zVerse4>::~zTextBase();

template
std::string & zTextBase<zVerse>::getRawEntryBuf() const;
template
std::string & zTextBase<zVerse4>::getRawEntryBuf() const;

template
void zTextBase<zVerse>::increment(int steps = 1);
template
void zTextBase<zVerse4>::increment(int steps = 1);

template
bool zTextBase<zVerse>::isWritable() const;
template
bool zTextBase<zVerse4>::isWritable() const;

template
void zTextBase<zVerse>::setEntry(char const * inbuf, long len = -1);
template
void zTextBase<zVerse4>::setEntry(char const * inbuf, long len = -1);

template
void zTextBase<zVerse>::linkEntry(SWKey const * linkKey);
template
void zTextBase<zVerse4>::linkEntry(SWKey const * linkKey);

template
void zTextBase<zVerse>::deleteEntry();
template
void zTextBase<zVerse4>::deleteEntry();

template
bool zTextBase<zVerse>::isLinked(SWKey const * k1, SWKey const * k2) const;
template
bool zTextBase<zVerse4>::isLinked(SWKey const * k1, SWKey const * k2) const;

template
bool zTextBase<zVerse>::hasEntry(SWKey const * k) const;
template
bool zTextBase<zVerse4>::hasEntry(SWKey const * k) const;

} /* namespace swordxx */