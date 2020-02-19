/******************************************************************************
 *
 *  ztext.cpp -    code for class 'zText'- a module that reads compressed text
 *        files
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

#include <cstdio>
#include <fcntl.h>
#include <utility>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../ShareRef.h"
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
zTextBase<BaseZVerse>::zTextBase(
        const char * ipath,
        const char * iname,
        const char * idesc,
        BlockType iblockType,
        std::unique_ptr<SWCompress> icomp,
        TextEncoding enc,
        SWTextDirection dir,
        SWTextMarkup mark,
        const char * ilang,
        std::shared_ptr<VersificationMgr::System const> v11n)
    : BaseZVerse(ipath, FileMgr::RDWR, iblockType, std::move(icomp))
    , SWText(iname, idesc, enc, dir, mark, ilang, std::move(v11n))
    , blockType(iblockType)
{}


/******************************************************************************
 * zText Destructor - Cleans up instance of zText
 */
template <typename BaseZVerse>
zTextBase<BaseZVerse>::~zTextBase() { this->flushCache(); }

template <typename BaseZVerse>
bool zTextBase<BaseZVerse>::isWritable() const noexcept
{ return this->idxfp[0]->isWritable(); }

template <typename BaseZVerse>
char zTextBase<BaseZVerse>::createModule(
        const char * path,
        BlockType blockBound,
        std::shared_ptr<VersificationMgr::System const> v11n)
{ return BaseZVerse::createModule(path, blockBound, std::move(v11n)); }



/******************************************************************************
 * zText::getRawEntry    - Returns the current verse buffer
 *
 * RET: buffer with verse
 */
template <typename BaseZVerse>
std::string zTextBase<BaseZVerse>::getRawEntryImpl() const {
    VerseOffsetType start = 0;
    VerseSizeType size = 0;
    BufferNumberType buffnum = 0;
    auto const key_(getVerseKey());

    this->findOffset(key_->getTestament(), key_->getTestamentIndex(), &start, &size, &buffnum);

    auto entry(this->zReadText(key_->getTestament(), start, size, buffnum));
    rawFilter(entry, key_.get());
    return entry;
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
void zTextBase<BaseZVerse>::setEntry(std::string_view text) {
    auto const key_(getVerseKey());

    // see if we've jumped across blocks since last write
    if (lastWriteKey) {
        if (!sameBlock(*lastWriteKey, *key_)) {
            this->flushCache();
        }
        lastWriteKey.reset();
    }

    this->doSetText(key_->getTestament(), key_->getTestamentIndex(), text);

    lastWriteKey = std::static_pointer_cast<VerseKey const>(key_->clone());
}

template <typename BaseZVerse>
void zTextBase<BaseZVerse>::linkEntry(SWKey const & inkey) {
    auto const destkey(getVerseKey());
    auto const srckey(getVerseKey(shareRef(inkey)));
    this->doLinkEntry(destkey->getTestament(),
                      destkey->getTestamentIndex(),
                      srckey->getTestamentIndex());
}


/******************************************************************************
 * zFiles::deleteEntry    - deletes this entry
 *
 */
template <typename BaseZVerse>
void zTextBase<BaseZVerse>::deleteEntry() {
    auto const key_(getVerseKey());
    using namespace std::literals::string_view_literals;
    this->doSetText(key_->getTestament(), key_->getTestamentIndex(), ""sv);
}

template <typename BaseZVerse>
void zTextBase<BaseZVerse>::rawZFilter(std::string & buf, char direction) const{
    // hack, use key as direction for enciphering
    rawFilter(buf, (SWKey *)(long)direction);
}

template <typename BaseZVerse>
void zTextBase<BaseZVerse>::flush() { BaseZVerse::flushCache(); }


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
    auto tmpkey(getVerseKey());

    this->findOffset(tmpkey->getTestament(), tmpkey->getTestamentIndex(), &start, &size, &buffnum);

    auto lastgood(tmpkey);
    while (steps) {
        VerseOffsetType laststart = start;
        VerseSizeType lastsize = size;
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
        long index = tmpkey->getTestamentIndex();
        this->findOffset(tmpkey->getTestament(), index, &start, &size, &buffnum);

        if (
            (
                ((laststart != start) || (lastsize != size))    // we're a different entry
//                && (start > 0)
                && (size)    // and we actually have a size
            )
            || !isSkipConsecutiveLinks()
        ) {    // or we don't want to skip consecutive links
            steps += (steps < 0) ? 1 : -1;
            lastgood = tmpkey;
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

template <typename BaseZVerse>
void zTextBase<BaseZVerse>::decrement(int steps) { increment(-steps); }

template <typename BaseZVerse>
bool zTextBase<BaseZVerse>::isLinked(SWKey const & k1, SWKey const & k2) const {
    VerseOffsetType start1, start2;
    VerseSizeType size1, size2;
    BufferNumberType buffnum1, buffnum2;
    auto const vk1(getVerseKey(shareRef(k1)));
    auto const vk2(getVerseKey(shareRef(k2)));
    if (vk1->getTestament() != vk2->getTestament())
        return false;

    this->findOffset(vk1->getTestament(), vk1->getTestamentIndex(), &start1, &size1, &buffnum1);
    this->findOffset(vk2->getTestament(), vk2->getTestamentIndex(), &start2, &size2, &buffnum2);
    return start1 == start2 && buffnum1 == buffnum2;
}

template <typename BaseZVerse>
bool zTextBase<BaseZVerse>::hasEntry(SWKey const & k) const {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    auto const vk(getVerseKey(shareRef(k)));
    this->findOffset(vk->getTestament(), vk->getTestamentIndex(), &start, &size, &buffnum);
    return size;
}

// Explicit instantions:

template class zTextBase<zVerse>;
template class zTextBase<zVerse4>;

} /* namespace swordxx */
