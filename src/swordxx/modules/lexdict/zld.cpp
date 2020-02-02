/******************************************************************************
 *
 *  zld.cpp -    code for class 'zLD'- a module that reads zlib compressed
 *        lexicon and dictionary files
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "zld.h"

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <memory>
#include <utility>
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

zLD::zLD(const char *ipath, const char *iname, const char *idesc, long blockCount, std::unique_ptr<SWCompress> icomp, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang, bool caseSensitive, bool strongsPadding) : zStr(ipath, -1, blockCount, std::move(icomp), caseSensitive), SWLD(iname, idesc, enc, dir, mark, ilang, strongsPadding) {

}


/******************************************************************************
 * RawLD Destructor - Cleans up instance of RawLD
 */

zLD::~zLD() {
    flushCache();
}


bool zLD::isWritable() const noexcept {
    return ((idxfd->getFd() > 0) && ((idxfd->mode() & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * zLD::getEntry    - Looks up entry from data file.  'Snaps' to closest
 *                entry and sets 'entrybuf'.
 *
 * ENT: away - number of entries offset from key (default = 0)
 *
 * RET: error status
 */

char zLD::getEntry(std::string & entry, long away) const {
    char retval = 0;
    long index;
    auto const key(getKey());
    auto const keyText(key->getText());
    if (this->m_strongsPadding) {
        retval = findKeyIndex(strongsPadBuf(keyText.c_str()).get(), &index, away);
    } else {
        retval = findKeyIndex(keyText.c_str(), &index, away);
    }
    if (!retval) {
        std::string idxbuf;
        std::string ebuf;
        getText(index, idxbuf, ebuf);
        entry = ebuf;

        rawFilter(entry, key.get());

        m_entkeytxt = std::move(idxbuf); // set entry key text that module 'snapped' to.
    } else {
        entry.clear();
    }

    return retval;
}


/******************************************************************************
 * zLD::getRawEntry    - Returns the correct entry when char * cast
 *                            is requested
 *
 * RET: string buffer with entry
 */

std::string zLD::getRawEntryImpl() const {
    std::string entry;
    getEntry(entry);
    return entry;
}


/******************************************************************************
 * zLD::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */

void zLD::increment(int steps) {
    char tmperror;

    if (getKey()->isTraversable()) {
        getKey()->increment(steps);
        error = getKey()->popError();
        steps = 0;
    }

    std::string unusedEntry; /// \todo remove this variable
    tmperror = (getEntry(unusedEntry, steps)) ? KEYERR_OUTOFBOUNDS : 0;
    error = (error)?error:tmperror;
    getKey()->setText(m_entkeytxt);
}

void zLD::decrement(int steps) { increment(-steps); }


void zLD::setEntry(std::string_view text) {
    auto const keyText = getKey()->getText();
    if (this->m_strongsPadding) {
        setText(strongsPadBuf(keyText.c_str()).get(), text);
    } else {
        setText(keyText.c_str(), text);
    }
}


void zLD::linkEntry(SWKey const & inkey) {
    auto const keyText = getKey()->getText();
    if (this->m_strongsPadding) {
        zStr::linkEntry(strongsPadBuf(keyText.c_str()).get(), inkey.getText().c_str());
    } else {
        zStr::linkEntry(keyText.c_str(), inkey.getText().c_str());
    }
}


/******************************************************************************
 * RawFiles::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void zLD::deleteEntry() {
    auto const keyText = getKey()->getText();
    using namespace std::literals::string_view_literals;
    if (this->m_strongsPadding) {
        setText(strongsPadBuf(keyText.c_str()).get(), ""sv);
    } else {
        setText(keyText.c_str(), ""sv);
    }
}

void zLD::rawZFilter(std::string & buf, char direction) const {
    // hack, use key as direction for enciphering
    rawFilter(buf, (SWKey *)(long)direction);
}

void zLD::flush() { flushCache(); }

long zLD::getEntryCount() const {
    if (!idxfd || idxfd->getFd() < 0)
        return 0;
    return idxfd->seek(0, SEEK_END) / IDXENTRYSIZE;
}


long zLD::getEntryForKey(char const * key_) const {
    long offset;
    if (this->m_strongsPadding) {
        findKeyIndex(strongsPadBuf(key_).get(), &offset);
    } else {
        findKeyIndex(key_, &offset);
    }
    return offset/IDXENTRYSIZE;
}


std::string zLD::getKeyForEntry(long entry) const
{ return getKeyFromIdxOffset(entry * IDXENTRYSIZE); }


} /* namespace swordxx */

