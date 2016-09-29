/******************************************************************************
 *
 *  ztext4.h -    code for class 'zText4'- a module that reads compressed text
 *        files: ot and nt using indexs ??.vss
 *
 * $Id$
 *
 * Copyright 1996-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef ZTEXT4_H
#define ZTEXT4_H

#include "../../swtext.h"
#include "../common/zverse4.h"

#include "../../defs.h"


namespace lucene { namespace index {
class IndexReader;
}}

namespace lucene { namespace search {
class IndexSearcher;
}}

namespace swordxx {

/*** SWModule implementation for compressed modules
* This class handles compressed modules.
* It should not be used in frontends, unless you are doing very special things.
*/
class SWDLLEXPORT zText4:public zVerse4, public SWText {

    VerseKey *lastWriteKey;
    bool sameBlock(VerseKey * lastWriteKey, VerseKey * key);
    int blockType;

public:
    zText4(char const * ipath,
           char const * iname = nullptr,
           char const * idesc = nullptr,
           int blockType = CHAPTERBLOCKS,
           SWCompress * icomp = nullptr,
           TextEncoding encoding = ENC_UNKNOWN,
           SWTextDirection dir = DIRECTION_LTR,
           SWTextMarkup markup = FMT_UNKNOWN,
           const char * ilang = nullptr,
           const char * versification = "KJV");

    ~zText4() override;
    std::string & getRawEntryBuf() const override;

    void increment(int steps = 1) override;
    void decrement(int steps = 1) override { increment(-steps); }

    // write interface ----------------------------
    bool isWritable() const override;
    static char createModule(const char *path, int blockBound, const char *v11n = "KJV") {
        return zVerse4::createModule(path, blockBound, v11n);
    }

    void setEntry(char const * inbuf, long len = -1) override;
    void linkEntry(SWKey const * linkKey) override;
    void deleteEntry() override;
    // end write interface ------------------------

    void rawZFilter(std::string & buf, char direction = 0) const override {
        // hack, use key as direction for enciphering:
        rawFilter(buf, (SWKey *)(long)direction);
    }

    void flush() override { flushCache(); }

    bool isLinked(SWKey const * k1, SWKey const * k2) const override;
    bool hasEntry(SWKey const * k) const override;

    SWMODULE_OPERATORS
};

} /* namespace swordxx */
#endif
