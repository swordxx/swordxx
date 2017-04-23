/******************************************************************************
 *
 *  ztext.h -    code for class 'zText'- a module that reads compressed text
 *        files: ot and nt using indexs ??.vss
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

#ifndef SWORDXX_ZTEXTBASE_H
#define SWORDXX_ZTEXTBASE_H

#include "../../swtext.h"

#include <memory>
#include "../../defs.h"
#include "../common/swcomprs.h"
#include "../common/zverse.h"
#include "../common/zverse4.h"


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
template <typename BaseZVerse>
class SWDLLEXPORT zTextBase: public BaseZVerse, public SWText {

    VerseKey *lastWriteKey;
    bool sameBlock(VerseKey const & lastWriteKey, VerseKey const & key);
    int blockType;

public: /* Types: */

    using VerseOffsetType = typename BaseZVerse::VerseOffsetType;
    using VerseSizeType = typename BaseZVerse::VerseSizeType;
    using BufferNumberType = typename BaseZVerse::BufferNumberType;

public:
    zTextBase(char const * ipath,
              char const * iname = nullptr,
              char const * idesc = nullptr,
              BlockType blockType = CHAPTERBLOCKS,
              std::unique_ptr<SWCompress> icomp = nullptr,
              TextEncoding encoding = ENC_UNKNOWN,
              SWTextDirection dir = DIRECTION_LTR,
              SWTextMarkup markup = FMT_UNKNOWN,
              char const * ilang = nullptr,
              char const * versification = "KJV");

    ~zTextBase() override;

    std::string & getRawEntryBuf() const override;

    void increment(int steps = 1) override;
    void decrement(int steps = 1) override { increment(-steps); }

    bool isWritable() const override;
    static char createModule(const char *path, BlockType blockBound, const char *v11n = "KJV") {
        return BaseZVerse::createModule(path, blockBound, v11n);
    }

    void setEntry(char const * inbuf, long len = -1) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;

    void rawZFilter(std::string & buf, char direction = 0) const override {
        // hack, use key as direction for enciphering
        rawFilter(buf, (SWKey *)(long)direction);
    }

    void flush() override { BaseZVerse::flushCache(); }

    bool isLinked(SWKey const * k1, SWKey const * k2) const override;
    bool hasEntry(SWKey const * k) const override;

}; /* class zTextBase */

extern template
zTextBase<zVerse>::zTextBase(
        char const * ipath,
        char const * iname = nullptr,
        char const * idesc = nullptr,
        BlockType blockType = CHAPTERBLOCKS,
        std::unique_ptr<SWCompress> icomp = nullptr,
        TextEncoding encoding = ENC_UNKNOWN,
        SWTextDirection dir = DIRECTION_LTR,
        SWTextMarkup markup = FMT_UNKNOWN,
        char const * ilang = nullptr,
        char const * versification = "KJV");
extern template
zTextBase<zVerse4>::zTextBase(
        char const * ipath,
        char const * iname = nullptr,
        char const * idesc = nullptr,
        BlockType blockType = CHAPTERBLOCKS,
        std::unique_ptr<SWCompress> icomp = nullptr,
        TextEncoding encoding = ENC_UNKNOWN,
        SWTextDirection dir = DIRECTION_LTR,
        SWTextMarkup markup = FMT_UNKNOWN,
        char const * ilang = nullptr,
        char const * versification = "KJV");

extern template zTextBase<zVerse>::~zTextBase();
extern template zTextBase<zVerse4>::~zTextBase();

extern template
std::string & zTextBase<zVerse>::getRawEntryBuf() const;
extern template
std::string & zTextBase<zVerse4>::getRawEntryBuf() const;

extern template
void zTextBase<zVerse>::increment(int steps = 1);
extern template
void zTextBase<zVerse4>::increment(int steps = 1);

extern template
bool zTextBase<zVerse>::isWritable() const;
extern template
bool zTextBase<zVerse4>::isWritable() const;

extern template
void zTextBase<zVerse>::setEntry(char const * inbuf, long len = -1);
extern template
void zTextBase<zVerse4>::setEntry(char const * inbuf, long len = -1);

extern template
void zTextBase<zVerse>::linkEntry(SWKey const & linkKey);
extern template
void zTextBase<zVerse4>::linkEntry(SWKey const & linkKey);

extern template
void zTextBase<zVerse>::deleteEntry();
extern template
void zTextBase<zVerse4>::deleteEntry();

extern template
bool zTextBase<zVerse>::isLinked(SWKey const * k1, SWKey const * k2) const;
extern template
bool zTextBase<zVerse4>::isLinked(SWKey const * k1, SWKey const * k2) const;

extern template
bool zTextBase<zVerse>::hasEntry(SWKey const * k) const;
extern template
bool zTextBase<zVerse4>::hasEntry(SWKey const * k) const;

} /* namespace swordxx */

#endif /* SWORDXX_ZTEXTBASE_H */
