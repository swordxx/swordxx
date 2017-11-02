/******************************************************************************
 *
 *  rawldbase.cpp - base class 'RawLD' and 'RawLD4' - a module that reads raw
 *        lexicon and dictionary files: *.dat *.idx
 *
 * $Id$
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
bool RawLdBase<Base>::isWritable() const {
    return ((this->idxfd->getFd() > 0) && ((this->idxfd->mode & FileMgr::RDWR) == FileMgr::RDWR));
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
char RawLdBase<Base>::getEntry(long away) const {
    StartType start = 0;
    SizeType size = 0;
    char retval = 0;

    auto const keyText = key->getText();
    if (strongsPadding) {
        retval = this->findOffset(strongsPadBuf(keyText).get(), &start, &size, away);
    } else {
        retval = this->findOffset(keyText, &start, &size, away);
    }
    if (!retval) {
        auto const idxbuf(this->readText(start, &size, entryBuf));
        rawFilter(entryBuf, nullptr);    // hack, decipher
        rawFilter(entryBuf, key);
        entrySize = size;        // support getEntrySize call
        if (!key->isPersist())            // If we have our own key
            *key = idxbuf.c_str();                // reset it to entry index buffer

        m_entkeytxt = idxbuf; // set entry key text that module 'snapped' to.
    } else {
        entryBuf.clear();
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
std::string & RawLdBase<Base>::getRawEntryBuf() const {
    char ret = getEntry();
    if (!ret) {
//        if (!isUnicode())
            prepText(entryBuf);
    }
    else error = ret;

    return entryBuf;
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

    if (key->isTraversable()) {
        *key += steps;
        error = key->popError();
        steps = 0;
    }

    tmperror = (getEntry(steps)) ? KEYERR_OUTOFBOUNDS : 0;
    error = (error)?error:tmperror;
    *key = m_entkeytxt.c_str();
}

template <typename Base>
void RawLdBase<Base>::setEntry(char const * inbuf, long len) {
    auto const keyText = key->getText();
    if (strongsPadding) {
        this->doSetText(strongsPadBuf(keyText).get(), inbuf, len);
    } else {
        this->doSetText(keyText, inbuf, len);
    }
}

template <typename Base>
void RawLdBase<Base>::linkEntry(SWKey const & inkey) {
    auto const keyText = key->getText();
    if (strongsPadding) {
        this->doLinkEntry(strongsPadBuf(keyText).get(), inkey.getText());
    } else {
        this->doLinkEntry(keyText, inkey.getText());
    }
}


/******************************************************************************
 * RawFiles::deleteEntry    - deletes this entry
 *
 * RET: *this
 */
template <typename Base>
void RawLdBase<Base>::deleteEntry() {
    auto const keyText = key->getText();
    if (strongsPadding) {
        this->doSetText(strongsPadBuf(keyText).get(), "");
    } else {
        this->doSetText(keyText, "");
    }
}

template <typename Base>
long RawLdBase<Base>::getEntryCount() const {
    if (!this->idxfd || this->idxfd->getFd() < 0)
        return 0;
    return this->idxfd->seek(0, SEEK_END) / Base::IDXENTRYSIZE;
}

template <typename Base>
long RawLdBase<Base>::getEntryForKey(char const * key) const {
    StartType start;
    IndexOffsetType offset;
    SizeType size;

    if (strongsPadding) {
        this->findOffset(strongsPadBuf(key).get(), &start, &size, 0, &offset);
    } else {
        this->findOffset(key, &start, &size, 0, &offset);
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
