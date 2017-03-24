/******************************************************************************
 *
 *  rawld.cpp -    code for class 'RawLD'- a module that reads raw lexicon and
 *        dictionary files: *.dat *.idx
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

#include "rawld.h"

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

RawLD::RawLD(const char *ipath, const char *iname, const char *idesc, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang, bool caseSensitive, bool strongsPadding) : RawStr(ipath, -1, caseSensitive), SWLD(iname, idesc, enc, dir, mark, ilang, strongsPadding)
{
}


/******************************************************************************
 * RawLD Destructor - Cleans up instance of RawLD
 */

RawLD::~RawLD()
{
}


bool RawLD::isWritable() const {
    return ((idxfd->getFd() > 0) && ((idxfd->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawLD::getEntry    - Looks up entry from data file.  'Snaps' to closest
 *                entry and sets 'entrybuf'.
 *
 * ENT: away - number of entries offset from key (default = 0)
 *
 * RET: error status
 */

char RawLD::getEntry(long away) const
{
    uint32_t start = 0;
    uint16_t size = 0;
    char * idxbuf = nullptr;
    char retval = 0;

    auto const keyText = key->getText();
    char * const buf = new char[strlen(keyText) + 6u];
    strcpy(buf, keyText);

    if (strongsPadding) strongsPad(buf);

    if (!(retval = findOffset(buf, &start, &size, away))) {
        readText(start, &size, &idxbuf, entryBuf);
        rawFilter(entryBuf, nullptr);    // hack, decipher
        rawFilter(entryBuf, key);
        entrySize = size;        // support getEntrySize call
        if (!key->isPersist())            // If we have our own key
            *key = idxbuf;                // reset it to entry index buffer

        stdstr(&entkeytxt, idxbuf);    // set entry key text that module 'snapped' to.
        delete [] idxbuf;
    } else {
        entryBuf.clear();
    }

    delete [] buf;
    return retval;
}


/******************************************************************************
 * RawLD::getRawEntry    - Returns the correct entry when char * cast
 *                            is requested
 *
 * RET: string buffer with entry
 */

std::string &RawLD::getRawEntryBuf() const {

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

void RawLD::increment(int steps) {
    char tmperror;

    if (key->isTraversable()) {
        *key += steps;
        error = key->popError();
        steps = 0;
    }

    tmperror = (getEntry(steps)) ? KEYERR_OUTOFBOUNDS : 0;
    error = (error)?error:tmperror;
    *key = entkeytxt;
}


void RawLD::setEntry(const char *inbuf, long len) {
    auto const keyText = key->getText();
    char * const buf = new char[strlen(keyText) + 6u];
    strcpy(buf, keyText);

    if (strongsPadding) strongsPad(buf);

    doSetText(buf, inbuf, len);

    delete [] buf;
}


void RawLD::linkEntry(const SWKey *inkey) {
    auto const keyText = key->getText();
    char * const buf = new char[strlen(keyText) + 6u];
    strcpy(buf, keyText);

    if (strongsPadding) strongsPad(buf);

    doLinkEntry(buf, inkey->getText());

    delete [] buf;
}


/******************************************************************************
 * RawFiles::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void RawLD::deleteEntry() {
    auto const keyText = key->getText();
    char * const buf = new char[strlen(keyText) + 6u];
    strcpy(buf, keyText);

    if (strongsPadding) strongsPad(buf);

    doSetText(buf, "");

    delete [] buf;
}


long RawLD::getEntryCount() const
{ return idxfd ? (idxfd->seek(0, SEEK_END) / IDXENTRYSIZE) : 0; }


long RawLD::getEntryForKey(const char *key) const {
    uint32_t start, offset;
    uint16_t size;

    char *buf = new char [ strlen(key) + 6 ];
    strcpy(buf, key);

    if (strongsPadding) strongsPad(buf);

    findOffset(buf, &start, &size, 0, &offset);

    delete [] buf;

    return offset / IDXENTRYSIZE;
}


char *RawLD::getKeyForEntry(long entry) const {
    char * key = nullptr;
    getIDXBuf(entry * IDXENTRYSIZE, &key);
    return key;
}


} /* namespace swordxx */

