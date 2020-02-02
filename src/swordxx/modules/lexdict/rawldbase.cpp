/******************************************************************************
 *
 *  rawldbase.cpp - base class 'RawLD' and 'RawLD4' - a module that reads raw
 *        lexicon and dictionary files: *.dat *.idx
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

#include "rawldbase.h"

#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include "../../filemgr.h"
#include "../../swkey.h"
#include "../../utilstr.h"


namespace swordxx {


 /******************************************************************************
 * RawLD Constructor - Initializes data for instance of RawLD
 *
 * ENT:    ipath    - path and filename of files (no extension)
 *        iname    - Internal name for module
 *        idesc    - Name to display to user for module
 */
template <typename Base>
RawLdBase<Base>::RawLdBase(char const * ipath,
                           char const * iname,
                           char const * idesc,
                           TextEncoding enc,
                           SWTextDirection dir,
                           SWTextMarkup mark,
                           char const * ilang,
                           bool caseSensitive,
                           bool strongsPadding)
    : Base(ipath, -1, caseSensitive)
    , SWLD(iname, idesc, enc, dir, mark, ilang, strongsPadding)
{}

template <typename Base>
bool RawLdBase<Base>::isWritable() const noexcept {
    return ((this->idxfd->getFd() > 0) && ((this->idxfd->mode() & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawLD::getEntry    - Looks up entry from data file.  'Snaps' to closest
 *                entry and sets 'entrybuf'.
 *
 * ENT: away - number of entries offset from key (default = 0)
 *
 * RET: error status
 */
template <typename Base>
char RawLdBase<Base>::getEntry(std::string & entry, long away) const {
    StartType start = 0;
    SizeType size = 0;
    char retval = 0;

    auto const key(getKey());
    auto const keyText = key->getText();
    if (this->m_strongsPadding) {
        retval = this->findOffset(strongsPadBuf(keyText.c_str()).get(), &start, &size, away);
    } else {
        retval = this->findOffset(keyText.c_str(), &start, &size, away);
    }
    if (!retval) {
        auto const idxbuf(this->readText(start, size, entry));
        rawFilter(entry, nullptr);    // hack, decipher
        rawFilter(entry, key.get());

        m_entkeytxt = idxbuf; // set entry key text that module 'snapped' to.
    } else {
        entry.clear();
    }

    return retval;
}


/******************************************************************************
 * RawLD::getRawEntry    - Returns the correct entry when char * cast
 *                            is requested
 *
 * RET: string buffer with entry
 */
template <typename Base>
std::string RawLdBase<Base>::getRawEntryImpl() const {
    std::string entry;
    char ret = getEntry(entry);
    if (ret)
        error = ret;
    return entry;
}


/******************************************************************************
 * RawLD::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */
template <typename Base>
void RawLdBase<Base>::increment(int steps) {
    char tmperror;

    if (getKey()->isTraversable()) {
        getKey()->increment(steps);
        error = getKey()->popError();
        steps = 0;
    }

    std::string unusedEntry; /// \todo Remove this variable
    tmperror = (getEntry(unusedEntry, steps)) ? KEYERR_OUTOFBOUNDS : 0;
    error = (error)?error:tmperror;
    getKey()->setText(m_entkeytxt);
}

template <typename Base>
void RawLdBase<Base>::decrement(int steps) { increment(-steps); }

template <typename Base>
void RawLdBase<Base>::setEntry(std::string_view text) {
    auto const keyText = getKey()->getText();
    if (this->m_strongsPadding) {
        this->doSetText(strongsPadBuf(keyText.c_str()).get(), text);
    } else {
        this->doSetText(keyText.c_str(), text);
    }
}

template <typename Base>
void RawLdBase<Base>::linkEntry(SWKey const & inkey) {
    auto const keyText = getKey()->getText();
    if (this->m_strongsPadding) {
        this->doLinkEntry(strongsPadBuf(keyText.c_str()).get(), inkey.getText().c_str());
    } else {
        this->doLinkEntry(keyText.c_str(), inkey.getText().c_str());
    }
}


/******************************************************************************
 * RawFiles::deleteEntry    - deletes this entry
 *
 * RET: *this
 */
template <typename Base>
void RawLdBase<Base>::deleteEntry() {
    using namespace std::literals::string_view_literals;
    auto const keyText = getKey()->getText();
    if (this->m_strongsPadding) {
        this->doSetText(strongsPadBuf(keyText.c_str()).get(), ""sv);
    } else {
        this->doSetText(keyText.c_str(), ""sv);
    }
}

template <typename Base>
long RawLdBase<Base>::getEntryCount() const {
    if (!this->idxfd || this->idxfd->getFd() < 0)
        return 0;
    return this->idxfd->seek(0, SEEK_END) / Base::IDXENTRYSIZE;
}

template <typename Base>
long RawLdBase<Base>::getEntryForKey(char const * key_) const {
    StartType start;
    IndexOffsetType offset;
    SizeType size;

    if (this->m_strongsPadding) {
        this->findOffset(strongsPadBuf(key_).get(), &start, &size, 0, &offset);
    } else {
        this->findOffset(key_, &start, &size, 0, &offset);
    }
    return offset / Base::IDXENTRYSIZE;
}

template <typename Base>
std::string RawLdBase<Base>::getKeyForEntry(long entry) const
{ return this->getIDXBuf(entry * Base::IDXENTRYSIZE); }

// Explicit instantiations:


template class RawLdBase<RawStr>;
template class RawLdBase<RawStr4>;

} /* namespace swordxx */
