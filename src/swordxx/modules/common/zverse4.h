/******************************************************************************
 *
 *  zverse4.h -        code for class 'zVerse4'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
 *
 * $Id$
 *
 * Copyright 2000-2014 CrossWire Bible Society (http://www.crosswire.org)
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


#ifndef ZVERSE4_H
#define ZVERSE4_H

#include <string>
#include "../../defs.h"
#include "../../normalizedpath.h"


namespace swordxx {

class FileDesc;
class SWCompress;

class SWDLLEXPORT zVerse4 {
    SWCompress *compressor;

protected:

    FileDesc *idxfp[2];
    FileDesc *textfp[2];
    FileDesc *compfp[2];
    void doSetText(char testmt, long idxoff, const char *buf, long len = 0);
    void doLinkEntry(char testmt, long destidxoff, long srcidxoff);
    void flushCache() const;
    mutable char *cacheBuf;
    mutable unsigned int cacheBufSize;
    mutable char cacheTestament;
    mutable long cacheBufIdx;
    mutable bool dirtyCache;

public:

    // fileMode default = RDONLY
    zVerse4(NormalizedPath const & path,
            int fileMode = -1,
            BlockType blockType = CHAPTERBLOCKS,
            SWCompress * icomp = nullptr);
    virtual ~zVerse4();

    void findOffset(char testmt, long idxoff, long *start, unsigned long *size, unsigned long *buffnum) const;
    void zReadText(char testmt, long start, unsigned long size, unsigned long buffnum, std::string &buf) const;
    virtual void rawZFilter(std::string &buf, char direction = 0) const { (void) buf; (void) direction; }
    static char createModule(NormalizedPath const & path, BlockType blockBound, const char *v11n = "KJV");
};

} /* namespace swordxx */
#endif
