/******************************************************************************
 *
 *  rawtext.cpp -    code for class 'RawText'- a module that reads raw text
 *            files:    ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "rawtextbase.h"

#include <cstdio>
#include <fcntl.h>
#include <regex.h>    // GNU
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../ShareRef.h"
#include "../../sysdata.h"
#include "../../utilstr.h"


namespace swordxx {

/******************************************************************************
 * RawText Constructor - Initializes data for instance of RawText
 *
 * ENT:    iname - Internal name for module
 *    idesc - Name to display to user for module
 */
template <typename BaseRawVerse>
RawTextBase<BaseRawVerse>::RawTextBase(
        char const * ipath,
        char const * iname,
        char const * idesc,
        TextEncoding enc,
        SWTextDirection dir,
        SWTextMarkup mark,
        char const * ilang,
        std::shared_ptr<VersificationMgr::System const> v11n)
    : SWText(iname, idesc, enc, dir, mark, ilang, std::move(v11n))
    , BaseRawVerse(ipath)
{}

template <typename BaseRawVerse>
RawTextBase<BaseRawVerse>::~RawTextBase() {}

template <typename BaseRawVerse>
bool RawTextBase<BaseRawVerse>::isWritable() const noexcept
{ return this->idxfp[0]->isWritable(); }

template <typename BaseRawVerse>
char RawTextBase<BaseRawVerse>::createModule(
        char const * path,
        std::shared_ptr<VersificationMgr::System const> v11n)
{ return BaseRawVerse::createModule(path, std::move(v11n)); }


/******************************************************************************
 * RawText::getRawEntry    - Returns the correct verse when char * cast
 *                    is requested
 *
 * RET: string buffer with verse
 */
template <typename BaseRawVerse>
std::string RawTextBase<BaseRawVerse>::getRawEntryImpl() const {
    StartType start = 0;
    SizeType size = 0;
    auto const key_(getVerseKey());

    this->findOffset(key_->getTestament(),
                     key_->getTestamentIndex(),
                     &start,
                     &size);

    std::string entry(this->readText(key_->getTestament(), start, size));

    rawFilter(entry, nullptr);    // hack, decipher
    rawFilter(entry, key_.get());

    return entry;
}

template <typename BaseRawVerse>
void RawTextBase<BaseRawVerse>::setEntry(std::string_view text) {
    auto const key_(getVerseKey());
    this->doSetText(key_->getTestament(), key_->getTestamentIndex(), text);
}

template <typename BaseRawVerse>
void RawTextBase<BaseRawVerse>::linkEntry(SWKey const & inkey) {
    auto const destkey(getVerseKey());
    auto const srckey(getVerseKey(shareRef(inkey)));
    this->doLinkEntry(destkey->getTestament(),
                      destkey->getTestamentIndex(),
                      srckey->getTestamentIndex());
}


/******************************************************************************
 * RawText::deleteEntry    - deletes this entry
 *
 * RET: *this
 */
template <typename BaseRawVerse>
void RawTextBase<BaseRawVerse>::deleteEntry() {
    auto const key_(getVerseKey());
    using namespace std::literals::string_view_literals;
    this->doSetText(key_->getTestament(), key_->getTestamentIndex(), ""sv);
}

/******************************************************************************
 * RawText::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */
template <typename BaseRawVerse>
void RawTextBase<BaseRawVerse>::increment(int steps) {
    StartType start;
    SizeType size;
    auto tmpkey(getVerseKey());

    this->findOffset(tmpkey->getTestament(),
                     tmpkey->getTestamentIndex(),
                     &start,
                     &size);

    auto lastgood(tmpkey);
    while (steps) {
        StartType laststart = start;
        SizeType lastsize = size;
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
        this->findOffset(tmpkey->getTestament(), index, &start, &size);

        if ((((laststart != start) || (lastsize != size)) // we're a different entry
             && (size)) // and we actually have a size
            || !isSkipConsecutiveLinks()) // or we don't want to skip consecutive links
        {
            steps += (steps < 0) ? 1 : -1;
            lastgood = tmpkey;
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

template <typename BaseRawVerse>
bool RawTextBase<BaseRawVerse>::isLinked(SWKey const & k1, SWKey const & k2) const {
    StartType start1, start2;
    SizeType size1, size2;
    auto const vk1(getVerseKey(shareRef(k1)));
    auto const vk2(getVerseKey(shareRef(k2)));
    if (vk1->getTestament() != vk2->getTestament())
        return false;

    this->findOffset(vk1->getTestament(),
                     vk1->getTestamentIndex(),
                     &start1,
                     &size1);
    this->findOffset(vk2->getTestament(),
                     vk2->getTestamentIndex(),
                     &start2,
                     &size2);
    if (!size1 || !size2) return false;
    return start1 == start2;
}

template <typename BaseRawVerse>
bool RawTextBase<BaseRawVerse>::hasEntry(SWKey const & k) const {
    StartType start;
    SizeType size;
    auto const vk(getVerseKey(shareRef(k)));
    this->findOffset(vk->getTestament(), vk->getTestamentIndex(), &start, &size);
    return size;
}

template class RawTextBase<RawVerse>;
template class RawTextBase<RawVerse4>;

} /* namespace swordxx */
