/******************************************************************************
 *
 *  rawcom.cpp -    code for class 'RawCom'- a module that reads raw
 *            commentary files:
 *            ot and nt using indexs ??.bks ??.cps ??.vss
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

#include "rawcombase.h"

#include <cstdio>
#include <fcntl.h>
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../ShareRef.h"


namespace swordxx {

 /******************************************************************************
 * RawCom Constructor - Initializes data for instance of RawCom
 *
 * ENT:    iname - Internal name for module
 *    idesc - Name to display to user for module
 */
template <typename Base>
RawComBase<Base>::RawComBase(
        const char * ipath,
        const char * iname,
        const char * idesc,
        TextEncoding encoding,
        SWTextDirection dir,
        SWTextMarkup markup,
        const char * ilang,
        std::shared_ptr<VersificationMgr::System const> v11n)
    : Base(ipath)
    , SWCom(iname, idesc, encoding, dir, markup, ilang, std::move(v11n))
{}


/******************************************************************************
 * RawCom Destructor - Cleans up instance of RawCom
 */
template <typename Base>
RawComBase<Base>::~RawComBase() = default;


/******************************************************************************
 * RawComBase<Base>::increment    - Increments module key a number of entries
 *
 * ENT:    steps    - Number of entries to jump forward
 *
 * RET: *this
 */
template <typename Base>
void RawComBase<Base>::increment(int steps) {
    StartType start;
    SizeType size;
    auto tmpkey(getVerseKey());

    this->findOffset(tmpkey->getTestament(),
                     tmpkey->getTestamentIndex(),
                     &start,
                     &size);

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
        this->findOffset(tmpkey->getTestament(), index, &start, &size);
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
void RawComBase<Base>::decrement(int steps) { increment(-steps); }

template <typename Base>
bool RawComBase<Base>::isWritable() const noexcept
{ return this->idxfp[0]->isWritable(); }

template <typename Base>
char RawComBase<Base>::createModule(
        const char * path,
        std::shared_ptr<VersificationMgr::System const> v11n)
{ return Base::createModule(path, std::move(v11n)); }

template <typename Base>
void RawComBase<Base>::setEntry(std::string_view text) {
    auto const key_(getVerseKey());
    this->doSetText(key_->getTestament(),
                    key_->getTestamentIndex(),
                    text);
}

template <typename Base>
void RawComBase<Base>::linkEntry(SWKey const & inkey) {
    auto const destkey(getVerseKey());
    auto const srckey(getVerseKey(shareRef(inkey)));

    this->doLinkEntry(destkey->getTestament(),
                      destkey->getTestamentIndex(),
                      srckey->getTestamentIndex());
}


/******************************************************************************
 * RawComBase<Base>::deleteEntry    - deletes this entry
 *
 * RET: *this
 */
template <typename Base>
void RawComBase<Base>::deleteEntry() {
    auto const key_(getVerseKey());
    using namespace std::literals::string_view_literals;
    this->doSetText(key_->getTestament(), key_->getTestamentIndex(), ""sv);
}

template <typename Base>
bool RawComBase<Base>::isLinked(SWKey const & k1, SWKey const & k2) const {
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
    if (!size1 || !size2)
        return false;
    return start1 == start2;
}

template <typename Base>
bool RawComBase<Base>::hasEntry(SWKey const & k) const {
    StartType start;
    SizeType size;
    auto const vk(getVerseKey(shareRef(k)));
    this->findOffset(vk->getTestament(),
                     vk->getTestamentIndex(),
                     &start,
                     &size);
    return size;
}

/******************************************************************************
 * RawComBase<Base>::getRawEntryImpl() - Returns the correct verse when char *
 *                                       cast is requested
 *
 * RET: string buffer with verse
 */
template <typename Base>
std::string RawComBase<Base>::getRawEntryImpl() const {
    StartType start = 0;
    SizeType size = 0;
    auto const key_(getVerseKey());
    this->findOffset(key_->getTestament(),
                     key_->getTestamentIndex(),
                     &start,
                     &size);

    auto entry(this->readText(key_->getTestament(), start, size));
    rawFilter(entry, nullptr);    // hack, decipher
    rawFilter(entry, key_.get());
    return entry;
}

template class RawComBase<RawVerse>;
template class RawComBase<RawVerse4>;

} /* namespace swordxx */
