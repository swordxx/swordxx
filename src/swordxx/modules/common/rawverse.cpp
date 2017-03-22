/******************************************************************************
 *
 *  rawverse.cpp -    code for class 'RawVerse'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
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

#include "rawverse.h"


namespace swordxx {

RawVerse::RawVerse(char const * ipath, int fileMode)
    : RawVerseBase(ipath, fileMode)
{}


/******************************************************************************
 * RawVerse::findoffset    - Finds the offset of the key verse from the indexes
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    idxoff    - offset into .vss
 *    start    - address to store the starting offset
 *    size    - address to store the size of the entry
 */
void RawVerse::findOffset(char testmt,
                          long idxoff,
                          long * start,
                          SizeType * size) const
{
    return RawVerseBase::findOffset<2, int32_t, SizeType>(testmt,
                                                          idxoff,
                                                          start,
                                                          size);
}


/******************************************************************************
 * RawVerse::settext    - Sets text for current offset
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    idxoff    - offset into .vss
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */
void RawVerse::doSetText(char testmt, long idxoff, const char *buf, long len)
{ return RawVerseBase::doSetText<int32_t, SizeType>(testmt, idxoff, buf, len); }


/******************************************************************************
 * RawVerse::linkentry    - links one entry to another
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    destidxoff    - dest offset into .vss
 *    srcidxoff        - source offset into .vss
 */
void RawVerse::doLinkEntry(char testmt, long destidxoff, long srcidxoff) {
    return RawVerseBase::doLinkEntry<int32_t, SizeType>(testmt,
                                                        destidxoff,
                                                        srcidxoff);
}


/******************************************************************************
 * RawVerse::createModule    - Creates new module files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */
char RawVerse::createModule(const char *ipath, const char *v11n)
{ return RawVerseBase::createModule<int32_t, SizeType>(ipath, v11n); }

} /* namespace swordxx */
