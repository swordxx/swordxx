/******************************************************************************
 *
 *  rawverse4.cpp -    code for class 'RawVerse4'- a module that reads raw
 *            text files:
 *            ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
 *
 * $Id$
 *
 * Copyright 2007-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "rawverse4.h"


namespace swordxx {

RawVerse4::RawVerse4(char const * ipath, int fileMode)
    : RawVerseBase(ipath, fileMode)
{}


/******************************************************************************
 * RawVerse4::findoffset    - Finds the offset of the key verse from the indexes
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    idxoff    - offset into .vss
 *    start    - address to store the starting offset
 *    size    - address to store the size of the entry
 */
void RawVerse4::findOffset(char testmt,
                           long idxoff,
                           long * start,
                           unsigned long * size) const
{
    return RawVerseBase::findOffset<8, 4, uint32_t, uint32_t>(testmt,
                                                              idxoff,
                                                              start,
                                                              size);
}


/******************************************************************************
 * RawVerse4::settext    - Sets text for current offset
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    idxoff    - offset into .vss
 *    buf    - buffer to store
 *      len     - length of buffer (0 - null terminated)
 */
void RawVerse4::doSetText(char testmt, long idxoff, const char *buf, long len)
{
    return RawVerseBase::doSetText<uint32_t, uint32_t, 8>(testmt,
                                                          idxoff,
                                                          buf,
                                                          len);
}


/******************************************************************************
 * RawVerse4::linkentry    - links one entry to another
 *
 * ENT: testmt    - testament to find (0 - Bible/module introduction)
 *    destidxoff    - dest offset into .vss
 *    srcidxoff        - source offset into .vss
 */
void RawVerse4::doLinkEntry(char testmt, long destidxoff, long srcidxoff) {
    return RawVerseBase::doLinkEntry<uint32_t, uint32_t, 8>(testmt,
                                                            destidxoff,
                                                            srcidxoff);
}


/******************************************************************************
 * RawVerse4::CreateModule    - Creates new module files
 *
 * ENT: path    - directory to store module files
 * RET: error status
 */
char RawVerse4::createModule(const char *ipath, const char *v11n)
{ return RawVerseBase::createModule<uint32_t, uint32_t>(ipath, v11n); }

} /* namespace swordxx */
