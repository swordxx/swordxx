/******************************************************************************
 *
 *  zcom.h -     code for class 'zCom'- a module that reads compressed text
 *                    files: ot and nt using indexs ??.vss
 *
 * $Id$
 *
 * Copyright 1996-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef ZCOM_H
#define ZCOM_H

#include "../../swcom.h"
#include "../common/zverse.h"

#include <memory>
#include "../../defs.h"
#include "../common/swcomprs.h"


namespace swordxx {

class SWDLLEXPORT zCom : public zVerse, public SWCom {

    VerseKey *lastWriteKey;
    bool sameBlock(VerseKey const & lastWriteKey, VerseKey const & key);
    BlockType blockType;


public:

    zCom(char const * ipath,
         char const * iname = nullptr,
         char const * idesc = nullptr,
         BlockType blockType = CHAPTERBLOCKS,
         std::unique_ptr<SWCompress> icomp = nullptr,
         TextEncoding encoding = ENC_UNKNOWN,
         SWTextDirection dir = DIRECTION_LTR,
         SWTextMarkup markup = FMT_UNKNOWN,
         char const * ilang = nullptr,
         char const * versification = "KJV");
    ~zCom() override;
    void increment(int steps = 1) override;
    void decrement(int steps = 1) override { increment(-steps); }

    bool isWritable() const override;
    static char createModule(const char *path, BlockType blockBound, const char *v11n = "KJV") {
        return zVerse::createModule(path, blockBound, v11n);
    }
    void setEntry(char const * inbuf, long len = -1) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;

    void rawZFilter(std::string & buf, char direction = 0) const override {
        // hack, use key as direction for enciphering
        rawFilter(buf, (SWKey *)(long)direction);
    }

    void flush() override { flushCache(); }

    bool isLinked(SWKey const * k1, SWKey const * k2) const override;
    bool hasEntry(SWKey const * k) const override;

protected: /* Methods: */

    std::string getRawEntryImpl() const override;

};

} /* namespace swordxx */

#endif
