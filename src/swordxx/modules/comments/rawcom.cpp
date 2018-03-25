/******************************************************************************
 *
 *  rawcom.cpp -    code for class 'RawCom'- a module that reads raw
 *            commentary files:
 *            ot and nt using indexs ??.bks ??.cps ??.vss
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

#include "rawcom.h"

#include <cstdio>
#include <fcntl.h>
#include "../../filemgr.h"
#include "../../keys/versekey.h"


namespace swordxx {

 /******************************************************************************
 * RawCom Constructor - Initializes data for instance of RawCom
 *
 * ENT:    iname - Internal name for module
 *    idesc - Name to display to user for module
 */

RawCom::RawCom(const char *ipath, const char *iname, const char *idesc, TextEncoding encoding, SWTextDirection dir, SWTextMarkup markup, const char *ilang, const char *versification)
        : RawVerse(ipath),
            SWCom(iname, idesc, encoding, dir, markup, ilang, versification) {
}


/******************************************************************************
 * RawCom Destructor - Cleans up instance of RawCom
 */

RawCom::~RawCom()
{
}


bool RawCom::isWritable() const {
    return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}
/******************************************************************************
 * RawCom::getRawEntry()    - Returns the correct verse when char * cast
 *                    is requested
 *
 * RET: string buffer with verse
 */

std::string RawCom::getRawEntryImpl() const {
    StartType start = 0;
    SizeType size = 0;
    VerseKey const & key = getVerseKey();

    findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size);
    entrySize = size;        // support getEntrySize call

    std::string entry;
    readText(key.getTestament(), start, size, entry);

    rawFilter(entry, nullptr);    // hack, decipher
    rawFilter(entry, &key);

    return entry;
}


/******************************************************************************
 * RawCom::increment    - Increments module key a number of entries
 *
 * ENT:    steps    - Number of entries to jump forward
 *
 * RET: *this
 */

void RawCom::increment(int steps) {
    StartType start;
    SizeType size;
    VerseKey const * tmpkey = &getVerseKey();

    findOffset(tmpkey->getTestament(), tmpkey->getTestamentIndex(), &start, &size);

    SWKey lastgood = *tmpkey;
    while (steps) {
        StartType laststart = start;
        SizeType lastsize = size;
        SWKey lasttry = *tmpkey;
        (steps > 0) ? ++(*key) : --(*key);
        tmpkey = &getVerseKey();

        if ((error = key->popError())) {
            key->positionFrom(lastgood);
            break;
        }
        long index = tmpkey->getTestamentIndex();
        findOffset(tmpkey->getTestament(), index, &start, &size);
        if (
            (((laststart != start) || (lastsize != size))    // we're a different entry
//                && (start > 0)
                && (size))    // and we actually have a size
                ||(!skipConsecutiveLinks)) {    // or we don't want to skip consecutive links
            steps += (steps < 0) ? 1 : -1;
            lastgood.positionFrom(*tmpkey);
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}


void RawCom::setEntry(const char *inbuf, long len) {
    VerseKey const & key = getVerseKey();
    doSetText(key.getTestament(), key.getTestamentIndex(), inbuf, len);
}


void RawCom::linkEntry(SWKey const & inkey) {
    VerseKey const & destkey = getVerseKey();
    VerseKey const & srckey = getVerseKey(&inkey);

    doLinkEntry(destkey.getTestament(), destkey.getTestamentIndex(), srckey.getTestamentIndex());

    if (&inkey != &srckey) // free our key if we created a VerseKey
        delete &srckey;
}


/******************************************************************************
 * RawCom::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void RawCom::deleteEntry() {
    VerseKey const & key = getVerseKey();
    doSetText(key.getTestament(), key.getTestamentIndex(), "");
}

bool RawCom::isLinked(const SWKey *k1, const SWKey *k2) const {
    StartType start1, start2;
    SizeType size1, size2;
    VerseKey const & vk1 = getVerseKey(k1);
    VerseKey const & vk2 = getVerseKey(k2);
    if (vk1.getTestament() != vk2.getTestament()) return false;

    findOffset(vk1.getTestament(), vk1.getTestamentIndex(), &start1, &size1);
    findOffset(vk2.getTestament(), vk2.getTestamentIndex(), &start2, &size2);
    if (!size1 || !size2) return false;
    return start1 == start2;
}

bool RawCom::hasEntry(const SWKey *k) const {
    StartType start;
    SizeType size;
    VerseKey const & vk = getVerseKey(k);
    findOffset(vk.getTestament(), vk.getTestamentIndex(), &start, &size);
    return size;
}

} /* namespace swordxx */
