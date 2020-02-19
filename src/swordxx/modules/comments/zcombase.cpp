/******************************************************************************
 *
 *  zcom.cpp -    code for class 'zCom'- a module that reads raw commentary
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

#include "zcombase.h"

#include <cstdio>
#include <fcntl.h>
#include <string>
#include <utility>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../ShareRef.h"


namespace swordxx {
namespace {

bool sameBlock(BlockType blockType, VerseKey const & k1, VerseKey const & k2) {
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

} // anonymous namespace

/******************************************************************************
 * zCom Constructor - Initializes data for instance of zCom
 *
 * ENT:    ipath - path to data files
 *        iname - Internal name for module
 *        idesc - Name to display to user for module
 *        iblockType - verse, chapter, book, etc. of index chunks
 *        icomp - Compressor object
 */
template <typename Base>
zComBase<Base>::zComBase(const char *ipath,
                         const char *iname,
                         const char *idesc,
                         BlockType iblockType,
                         std::unique_ptr<SWCompress> icomp,
                         TextEncoding enc,
                         SWTextDirection dir,
                         SWTextMarkup mark,
                         const char *ilang,
                         std::shared_ptr<VersificationMgr::System const> v11n)
    : Base(ipath, -1, iblockType, std::move(icomp))
    , SWCom(iname, idesc, enc, dir, mark, ilang, std::move(v11n))
    , blockType(iblockType)
{}

/******************************************************************************
 * zCom Destructor - Cleans up instance of zCom
 */
template <typename Base>
zComBase<Base>::~zComBase() { this->flushCache(); }


/******************************************************************************
 * zComBase<Base>::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */
template <typename Base>
void zComBase<Base>::increment(int steps) {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    auto tmpkey(getVerseKey());
    this->findOffset(tmpkey->getTestament(),
                     tmpkey->getTestamentIndex(),
                     &start,
                     &size,
                     &buffnum);

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
        this->findOffset(tmpkey->getTestament(),
                         index,
                         &start,
                         &size,
                         &buffnum);
        if (// we're a different entry:
            (((laststart != start) || (lastsize != size))
             && size) // and we actually have a size
            // or we don't want to skip consecutive links:
            || !isSkipConsecutiveLinks())
        {
            steps += (steps < 0) ? 1 : -1;
            lastgood = tmpkey;
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

template <typename Base>
void zComBase<Base>::decrement(int steps) { increment(-steps); }

template <typename Base>
bool zComBase<Base>::isWritable() const noexcept
{ return this->idxfp[0]->isWritable(); }

template <typename Base>
char zComBase<Base>::createModule(
        const char * path,
        BlockType blockBound,
        std::shared_ptr<VersificationMgr::System const> v11n)
{ return Base::createModule(path, blockBound, std::move(v11n)); }

template <typename Base>
void zComBase<Base>::setEntry(std::string_view inBuf) {
    auto const key_(getVerseKey());

    // see if we've jumped across blocks since last write
    if (lastWriteKey) {
        if (!sameBlock(blockType, *lastWriteKey, *key_))
            this->flushCache();
        lastWriteKey.reset();
    }

    this->doSetText(key_->getTestament(),
                    key_->getTestamentIndex(),
                    inBuf);
    lastWriteKey = std::static_pointer_cast<VerseKey const>(key_->clone());
}

template <typename Base>
void zComBase<Base>::linkEntry(SWKey const & inkey) {
    auto const destkey(getVerseKey());
    auto const srckey(getVerseKey(shareRef(inkey)));
    this-> doLinkEntry(destkey->getTestament(),
                       destkey->getTestamentIndex(),
                       srckey->getTestamentIndex());
}

/******************************************************************************
 * zComBase<Base>::deleteEntry    - deletes this entry
 *
 * RET: *this
 */
template <typename Base>
void zComBase<Base>::deleteEntry() {
    auto const key_(getVerseKey());
    using namespace std::literals::string_view_literals;
    this->doSetText(key_->getTestament(), key_->getTestamentIndex(), ""sv);
}

template <typename Base>
void zComBase<Base>::rawZFilter(std::string & buf, char direction) const {
    // hack, use key as direction for enciphering
    rawFilter(buf, (SWKey *)(long)direction);
}

template <typename Base>
void zComBase<Base>::flush() { this->flushCache(); }

template <typename Base>
bool zComBase<Base>::isLinked(SWKey const & k1, SWKey const & k2) const {
    VerseOffsetType start1, start2;
    VerseSizeType size1, size2;
    BufferNumberType buffnum1, buffnum2;
    auto const vk1(getVerseKey(shareRef(k1)));
    auto const vk2(getVerseKey(shareRef(k2)));
    if (vk1->getTestament() != vk2->getTestament())
        return false;

    this->findOffset(vk1->getTestament(),
                     vk1->getTestamentIndex(),
                     &start1,
                     &size1,
                     &buffnum1);
    this->findOffset(vk2->getTestament(),
                     vk2->getTestamentIndex(),
                     &start2,
                     &size2,
                     &buffnum2);
    return start1 == start2 && buffnum1 == buffnum2;
}

template <typename Base>
bool zComBase<Base>::hasEntry(SWKey const & k) const {
    VerseOffsetType start;
    VerseSizeType size;
    BufferNumberType buffnum;
    auto const vk(getVerseKey(shareRef(k)));
    this->findOffset(vk->getTestament(),
                     vk->getTestamentIndex(),
                     &start,
                     &size,
                     &buffnum);
    return size;
}


/******************************************************************************
 * zComBase<Base>::getRawEntryImpl    - Returns the current verse buffer
 *
 * RET: buffer with verse
 */
template <typename Base>
std::string zComBase<Base>::getRawEntryImpl() const {
    VerseOffsetType start = 0;
    VerseSizeType size = 0;
    BufferNumberType buffnum = 0;
    auto const key_(getVerseKey());
    this->findOffset(key_->getTestament(),
                     key_->getTestamentIndex(),
                     &start,
                     &size,
                     &buffnum);

    auto entry(this->zReadText(key_->getTestament(), start, size, buffnum));
    rawFilter(entry, key_.get());
    return entry;
}

template class zComBase<zVerse>;
template class zComBase<zVerse4>;

} /* namespace swordxx */
