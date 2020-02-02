/******************************************************************************
 *
 *  zcom.h -     code for class 'zCom'- a module that reads compressed text
 *                    files: ot and nt using indexs ??.vss
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

#ifndef SWORDXX_ZCOMBASE_H
#define SWORDXX_ZCOMBASE_H

#include "../../swcom.h"
#include "../common/zverse.h"
#include "../common/zverse4.h"

#include <memory>
#include "../../defs.h"
#include "../common/swcomprs.h"


namespace swordxx {

template <typename Base>
class SWDLLEXPORT zComBase : public Base, public SWCom {

public: /* Types: */

    using VerseOffsetType = typename Base::VerseOffsetType;
    using VerseSizeType = typename Base::VerseSizeType;
    using BufferNumberType = typename Base::BufferNumberType;

public: /* Methods: */

    zComBase(char const * ipath,
             char const * iname = nullptr,
             char const * idesc = nullptr,
             BlockType blockType = CHAPTERBLOCKS,
             std::unique_ptr<SWCompress> icomp = nullptr,
             TextEncoding encoding = ENC_UNKNOWN,
             SWTextDirection dir = DIRECTION_LTR,
             SWTextMarkup markup = FMT_UNKNOWN,
             char const * ilang = nullptr,
             std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    ~zComBase() override;


    void increment(int steps = 1) override;
    void decrement(int steps = 1) override;

    bool isWritable() const noexcept override;

    static char createModule(
            const char * path,
            BlockType blockBound,
            std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    void setEntry(std::string_view inBuf) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;

    void rawZFilter(std::string & buf, char direction = 0) const override;

    void flush() override;

    bool isLinked(SWKey const & k1, SWKey const & k2) const override;
    bool hasEntry(SWKey const & k) const override;

protected: /* Methods: */

    std::string getRawEntryImpl() const override;

private: /* Fields: */

    BlockType const blockType;
    std::shared_ptr<VerseKey const> lastWriteKey;

};

extern template class zComBase<zVerse>;
extern template class zComBase<zVerse4>;

} /* namespace swordxx */

#endif /* SWORDXX_ZCOMBASE_H */
