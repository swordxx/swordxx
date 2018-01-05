/******************************************************************************
 *
 *  zld.cpp -    code for class 'zLD'- a module that reads zlib compressed
 *        lexicon and dictionary files
 *
 * $Id$
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
#include <fcntl.h>
#include <memory>
#include <utility>
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

zLD::zLD(const char *ipath, const char *iname, const char *idesc, long blockCount, std::unique_ptr<SWCompress> icomp, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang, bool caseSensitive, bool strongsPadding) : zStr(ipath, -1, blockCount, std::move(icomp), caseSensitive), SWLD(iname, idesc, enc, dir, mark, ilang, strongsPadding) {

}


/******************************************************************************
 * RawLD Destructor - Cleans up instance of RawLD
 */

zLD::~zLD() {
    flushCache();
}


bool zLD::isWritable() const {
    return ((idxfd->getFd() > 0) && ((idxfd->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * zLD::getEntry    - Looks up entry from data file.  'Snaps' to closest
 *                entry and sets 'entrybuf'.
 *
 * ENT: away - number of entries offset from key (default = 0)
 *
 * RET: error status
 */

char zLD::getEntry(long away) const {
    char * idxbuf = nullptr;
    char * ebuf = nullptr;
    char retval = 0;
    long index;
    unsigned long size;
    auto const keyText = key->getText();
    if (strongsPadding) {
        retval = findKeyIndex(strongsPadBuf(keyText).get(), &index, away);
    } else {
        retval = findKeyIndex(keyText, &index, away);
    }
    if (!retval) {
        getText(index, &idxbuf, &ebuf);
        size = std::strlen(ebuf) + 1;
        entryBuf = ebuf;

        rawFilter(entryBuf, key);

        entrySize = size;        // support getEntrySize call
        if (!key->isPersist())            // If we have our own key
            key->setText(idxbuf);                // reset it to entry index buffer

        m_entkeytxt.assign(idxbuf); // set entry key text that module 'snapped' to.
        free(idxbuf);
        free(ebuf);
    } else {
        entryBuf.clear();
    }

    return retval;
}


/******************************************************************************
 * zLD::getRawEntry    - Returns the correct entry when char * cast
 *                            is requested
 *
 * RET: string buffer with entry
 */

std::string &zLD::getRawEntryBuf() const {
    if (!getEntry() /*&& !isUnicode()*/) {
        prepText(entryBuf);
    }

    return entryBuf;
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

    if (key->isTraversable()) {
        *key += steps;
        error = key->popError();
        steps = 0;
    }

    tmperror = (getEntry(steps)) ? KEYERR_OUTOFBOUNDS : 0;
    error = (error)?error:tmperror;
    key->setText(m_entkeytxt.c_str());
}


void zLD::setEntry(const char *inbuf, long len) {
    auto const keyText = key->getText();
    if (strongsPadding) {
        setText(strongsPadBuf(keyText).get(), inbuf, len);
    } else {
        setText(keyText, inbuf, len);
    }
}


void zLD::linkEntry(SWKey const & inkey) {
    auto const keyText = key->getText();
    if (strongsPadding) {
        zStr::linkEntry(strongsPadBuf(keyText).get(), inkey.getText());
    } else {
        zStr::linkEntry(keyText, inkey.getText());
    }
}


/******************************************************************************
 * RawFiles::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void zLD::deleteEntry() {
    auto const keyText = key->getText();
    if (strongsPadding) {
        setText(strongsPadBuf(keyText).get(), "");
    } else {
        setText(keyText, "");
    }
}


long zLD::getEntryCount() const {
    if (!idxfd || idxfd->getFd() < 0)
        return 0;
    return idxfd->seek(0, SEEK_END) / IDXENTRYSIZE;
}


long zLD::getEntryForKey(const char* key) const
{
    long offset;
    if (strongsPadding) {
        findKeyIndex(strongsPadBuf(key).get(), &offset);
    } else {
        findKeyIndex(key, &offset);
    }
    return offset/IDXENTRYSIZE;
}


std::string zLD::getKeyForEntry(long entry) const
{ return getKeyFromIdxOffset(entry * IDXENTRYSIZE); }


} /* namespace swordxx */

