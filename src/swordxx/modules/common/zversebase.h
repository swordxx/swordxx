/******************************************************************************
 *
 *  zverse.h -        code for class 'zVerse'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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


#ifndef SWORDXX_ZVERSEBASE_H
#define SWORDXX_ZVERSEBASE_H

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>
#include <utility>
#include "../../defs.h"
#include "../../normalizedpath.h"
#include "../../versificationmgr.h"


namespace swordxx {

class FileDesc;
class SWCompress;

template <typename VerseSizeType_>
class SWDLLEXPORT zVerseBase {

public: /* Types: */

    using BufferNumberType = std::uint32_t;
    using VerseSizeType = VerseSizeType_;
    using VerseOffsetType = std::uint32_t;

public: /* Methods: */

    // fileMode default = RDONLY
    zVerseBase(NormalizedPath const & path,
               int fileMode = -1,
               BlockType blockType = CHAPTERBLOCKS,
               std::unique_ptr<SWCompress> icomp = nullptr);
    virtual ~zVerseBase();

    virtual void rawZFilter(std::string & buf, char direction = 0) const;

protected: /* Methods: */

    void findOffset(char testmt,
                    long idxoff,
                    VerseOffsetType * start,
                    VerseSizeType * size,
                    BufferNumberType * buffnum) const;

    std::string zReadText(char testmt,
                          VerseOffsetType start,
                          VerseSizeType size,
                          BufferNumberType buffnum) const;

    static char createModule(
            NormalizedPath const & path,
            BlockType blockBound,
            std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    void doSetText(char testmt, long idxoff, std::string_view buf);

    void doLinkEntry(char testmt, long destidxoff, long srcidxoff);

    void flushCache() const;

private: /* Fields: */

    std::unique_ptr<SWCompress> const compressor;
    mutable std::string m_cacheBuf;

protected: /* Fields: */

    std::shared_ptr<FileDesc> idxfp[2];
    std::shared_ptr<FileDesc> textfp[2];
    std::shared_ptr<FileDesc> compfp[2];
    mutable char cacheTestament = 0;
    mutable long cacheBufIdx = -1;
    mutable bool dirtyCache = false;

};

extern template class zVerseBase<std::uint16_t>;
extern template class zVerseBase<std::uint32_t>;

} /* namespace swordxx */

#endif /* SWORDXX_ZVERSEBASE_H */
