/******************************************************************************
 *
 *  rawtext.cpp -    code for class 'RawText'- a module that reads raw text
 *            files:    ot and nt using indexs ??.bks ??.cps ??.vss
 *
 * $Id$
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
#include "../../stringmgr.h"
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
RawTextBase<BaseRawVerse>::RawTextBase(char const * ipath,
                                       char const * iname,
                                       char const * idesc,
                                       TextEncoding enc,
                                       SWTextDirection dir,
                                       SWTextMarkup mark,
                                       char const * ilang,
                                       char const * versification)
        : SWText(iname, idesc, enc, dir, mark, ilang, versification)
        , BaseRawVerse(ipath)
{}

template <typename BaseRawVerse>
RawTextBase<BaseRawVerse>::~RawTextBase() {}

template <typename BaseRawVerse>
bool RawTextBase<BaseRawVerse>::isWritable() const {
    return (this->idxfp[0]->getFd() > 0)
            && ((this->idxfp[0]->mode() & FileMgr::RDWR) == FileMgr::RDWR);
}

template <typename BaseRawVerse>
char RawTextBase<BaseRawVerse>::createModule(char const *path,
                                             char const *v11n)
{ return BaseRawVerse::createModule(path, v11n); }


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
    VerseKey const & key_ = getVerseKey();

    this->findOffset(key_.getTestament(),
                     key_.getTestamentIndex(),
                     &start,
                     &size);
    entrySize = size;        // support getEntrySize call

    std::string entry;
    this->readText(key_.getTestament(), start, size, entry);

    rawFilter(entry, nullptr);    // hack, decipher
    rawFilter(entry, &key_);

    return entry;
}

template <typename BaseRawVerse>
void RawTextBase<BaseRawVerse>::setEntry(char const *inbuf, long len) {
    VerseKey const & key_ = getVerseKey();
    this->doSetText(key_.getTestament(), key_.getTestamentIndex(), inbuf, len);
}

template <typename BaseRawVerse>
void RawTextBase<BaseRawVerse>::linkEntry(SWKey const & inkey) {
    VerseKey const & destkey = getVerseKey();
    VerseKey const & srckey = getVerseKey(&inkey);
    this->doLinkEntry(destkey.getTestament(),
                      destkey.getTestamentIndex(),
                      srckey.getTestamentIndex());
}


/******************************************************************************
 * RawText::deleteEntry    - deletes this entry
 *
 * RET: *this
 */
template <typename BaseRawVerse>
void RawTextBase<BaseRawVerse>::deleteEntry() {
    VerseKey const & key_ = getVerseKey();
    this->doSetText(key_.getTestament(), key_.getTestamentIndex(), "");
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
    VerseKey const * tmpkey = &getVerseKey();

    this->findOffset(tmpkey->getTestament(),
                     tmpkey->getTestamentIndex(),
                     &start,
                     &size);

    SWKey lastgood = *tmpkey;
    while (steps) {
        StartType laststart = start;
        SizeType lastsize = size;
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
        this->findOffset(tmpkey->getTestament(), index, &start, &size);

        if ((((laststart != start) || (lastsize != size)) // we're a different entry
             && (size)) // and we actually have a size
            || !isSkipConsecutiveLinks()) // or we don't want to skip consecutive links
        {
            steps += (steps < 0) ? 1 : -1;
            lastgood.positionFrom(*tmpkey);
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}

template <typename BaseRawVerse>
bool RawTextBase<BaseRawVerse>::isLinked(SWKey const & k1, SWKey const & k2) const {
    StartType start1, start2;
    SizeType size1, size2;
    VerseKey const & vk1 = getVerseKey(&k1);
    VerseKey const & vk2 = getVerseKey(&k2);
    if (vk1.getTestament() != vk2.getTestament()) return false;

    this->findOffset(vk1.getTestament(),
                     vk1.getTestamentIndex(),
                     &start1,
                     &size1);
    this->findOffset(vk2.getTestament(),
                     vk2.getTestamentIndex(),
                     &start2,
                     &size2);
    if (!size1 || !size2) return false;
    return start1 == start2;
}

template <typename BaseRawVerse>
bool RawTextBase<BaseRawVerse>::hasEntry(SWKey const & k) const {
    StartType start;
    SizeType size;
    VerseKey const & vk = getVerseKey(&k);
    this->findOffset(vk.getTestament(), vk.getTestamentIndex(), &start, &size);
    return size;
}

template class RawTextBase<RawVerse>;
template class RawTextBase<RawVerse4>;

} /* namespace swordxx */
