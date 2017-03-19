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

#include "rawtext.h"

#include <cstdio>
#include <fcntl.h>
#include <regex.h>    // GNU
#include "../../filemgr.h"
#include "../../keys/versekey.h"
#include "../../stringmgr.h"
#include "../../sysdata.h"
#include "../../utilstr.h"
#include "../common/rawstr.h"


namespace swordxx {

/******************************************************************************
 * RawText Constructor - Initializes data for instance of RawText
 *
 * ENT:    iname - Internal name for module
 *    idesc - Name to display to user for module
 */

RawText::RawText(const char *ipath, const char *iname, const char *idesc, TextEncoding enc, SWTextDirection dir, SWTextMarkup mark, const char* ilang, const char *versification)
        : SWText(iname, idesc, enc, dir, mark, ilang, versification),
        RawVerse(ipath) {
}


/******************************************************************************
 * RawText Destructor - Cleans up instance of RawText
 */

RawText::~RawText() {
}


bool RawText::isWritable() const {
    return ((idxfp[0]->getFd() > 0) && ((idxfp[0]->mode & FileMgr::RDWR) == FileMgr::RDWR));
}


/******************************************************************************
 * RawText::getRawEntry    - Returns the correct verse when char * cast
 *                    is requested
 *
 * RET: string buffer with verse
 */

std::string &RawText::getRawEntryBuf() const {
    long  start = 0;
    unsigned short size = 0;
    VerseKey const & key = getVerseKey();

    findOffset(key.getTestament(), key.getTestamentIndex(), &start, &size);
    entrySize = size;        // support getEntrySize call

    entryBuf = "";
    readText(key.getTestament(), start, size, entryBuf);

    rawFilter(entryBuf, nullptr);    // hack, decipher
    rawFilter(entryBuf, &key);

//    if (!isUnicode())
        prepText(entryBuf);

    return entryBuf;
}


void RawText::setEntry(const char *inbuf, long len) {
    VerseKey const & key = getVerseKey();
    doSetText(key.getTestament(), key.getTestamentIndex(), inbuf, len);
}


void RawText::linkEntry(const SWKey *inkey) {
    VerseKey const & destkey = getVerseKey();
    VerseKey const & srckey = getVerseKey(inkey);
    doLinkEntry(destkey.getTestament(), destkey.getTestamentIndex(), srckey.getTestamentIndex());
}


/******************************************************************************
 * RawText::deleteEntry    - deletes this entry
 *
 * RET: *this
 */

void RawText::deleteEntry() {
    VerseKey const & key = getVerseKey();
    doSetText(key.getTestament(), key.getTestamentIndex(), "");
}

/******************************************************************************
 * RawText::increment    - Increments module key a number of entries
 *
 * ENT:    increment    - Number of entries to jump forward
 *
 * RET: *this
 */

void RawText::increment(int steps) {
    long  start;
    unsigned short size;
    VerseKey const * tmpkey = &getVerseKey();

    findOffset(tmpkey->getTestament(), tmpkey->getTestamentIndex(), &start, &size);

    SWKey lastgood = *tmpkey;
    while (steps) {
        long laststart = start;
        unsigned short lastsize = size;
        SWKey lasttry = *tmpkey;
        (steps > 0) ? ++(*key) : --(*key);
        tmpkey = &getVerseKey();

        if ((error = key->popError())) {
            *key = lastgood;
            break;
        }
        long index = tmpkey->getTestamentIndex();
        findOffset(tmpkey->getTestament(), index, &start, &size);

        if (
            (
                ((laststart != start) || (lastsize != size))    // we're a different entry
//                && (start > 0)
                && (size)    // and we actually have a size
            )
            || !skipConsecutiveLinks
        ) {    // or we don't want to skip consecutive links
            steps += (steps < 0) ? 1 : -1;
            lastgood = *tmpkey;
        }
    }
    error = (error) ? KEYERR_OUTOFBOUNDS : 0;
}


bool RawText::isLinked(const SWKey *k1, const SWKey *k2) const {
    long start1, start2;
    unsigned short size1, size2;
    VerseKey const & vk1 = getVerseKey(k1);
    VerseKey const & vk2 = getVerseKey(k2);
    if (vk1.getTestament() != vk2.getTestament()) return false;

    findOffset(vk1.getTestament(), vk1.getTestamentIndex(), &start1, &size1);
    findOffset(vk2.getTestament(), vk2.getTestamentIndex(), &start2, &size2);
    if (!size1 || !size2) return false;
    return start1 == start2;
}

bool RawText::hasEntry(const SWKey *k) const {
    long start;
    unsigned short size;
    VerseKey const & vk = getVerseKey(k);
    findOffset(vk.getTestament(), vk.getTestamentIndex(), &start, &size);
    return size;
}



} /* namespace swordxx */
