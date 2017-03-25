/******************************************************************************
 *
 *  zverse.h -        code for class 'zVerse'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
 *
 * $Id$
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


namespace swordxx {

class FileDesc;
class SWCompress;

class SWDLLEXPORT zVerseBase {

public: /* Types: */

    using BufferNumberType = std::uint32_t;
    using VerseOffsetType = std::uint32_t;

public: /* Methods: */

    // fileMode default = RDONLY
    zVerseBase(NormalizedPath const & path,
               int fileMode = -1,
               BlockType blockType = CHAPTERBLOCKS,
               std::unique_ptr<SWCompress> icomp = nullptr);
    virtual ~zVerseBase();

    virtual void rawZFilter(std::string & buf, char direction = 0) const
    { (void) buf; (void) direction; }

protected: /* Methods: */

    template <typename VerseSizeType>
    void findOffset_(char testmt,
                     long idxoff,
                     VerseOffsetType * start,
                     VerseSizeType * size,
                     BufferNumberType * buffnum) const;

    template <typename VerseSizeType>
    void zReadText_(char testmt,
                    VerseOffsetType start,
                    VerseSizeType size,
                    BufferNumberType buffnum,
                    std::string & buf) const;

    template <typename VerseSizeType>
    static char createModule_(NormalizedPath const & path,
                              BlockType blockBound,
                              char const * v11n = "KJV");

    template <typename VerseSizeType>
    void doSetText_(char testmt, long idxoff, char const * buf, long len = 0);

    template <typename VerseSizeType>
    void doLinkEntry_(char testmt, long destidxoff, long srcidxoff);

    void flushCache() const;

private: /* Fields: */

    std::unique_ptr<SWCompress> const compressor;

protected: /* Fields: */

    FileDesc *idxfp[2];
    FileDesc *textfp[2];
    FileDesc *compfp[2];
    mutable char *cacheBuf;
    mutable unsigned int cacheBufSize;
    mutable char cacheTestament;
    mutable long cacheBufIdx;
    mutable bool dirtyCache;

};

extern template
void zVerseBase::findOffset_<std::uint16_t>(char testmt,
                                            long idxoff,
                                            VerseOffsetType * start,
                                            std::uint16_t * size,
                                            BufferNumberType * buffnum) const;
extern template
void zVerseBase::findOffset_<std::uint32_t>(char testmt,
                                            long idxoff,
                                            VerseOffsetType * start,
                                            std::uint32_t * size,
                                            BufferNumberType * buffnum) const;

extern template
void zVerseBase::zReadText_<std::uint16_t>(char testmt,
                                           VerseOffsetType start,
                                           std::uint16_t size,
                                           BufferNumberType buffnum,
                                           std::string & buf) const;
extern template
void zVerseBase::zReadText_<std::uint32_t>(char testmt,
                                           VerseOffsetType start,
                                           std::uint32_t size,
                                           BufferNumberType buffnum,
                                          std::string & buf) const;

extern template
char zVerseBase::createModule_<std::uint16_t>(NormalizedPath const & path,
                                              BlockType blockBound,
                                              char const * v11n = "KJV");
extern template
char zVerseBase::createModule_<std::uint32_t>(NormalizedPath const & path,
                                              BlockType blockBound,
                                              char const * v11n = "KJV");

extern template
void zVerseBase::doSetText_<std::uint16_t>(char testmt,
                                           long idxoff,
                                           char const * buf,
                                           long len = 0);
extern template
void zVerseBase::doSetText_<std::uint32_t>(char testmt,
                                           long idxoff,
                                           char const * buf,
                                           long len = 0);

extern template
void zVerseBase::doLinkEntry_<std::uint16_t>(char testmt,
                                             long destidxoff,
                                             long srcidxoff);
extern template
void zVerseBase::doLinkEntry_<std::uint32_t>(char testmt,
                                             long destidxoff,
                                             long srcidxoff);

template <typename VerseSizeType_>
class SWDLLEXPORT zVerseBaseImpl: public zVerseBase {

public: /* Types: */

    using VerseSizeType = VerseSizeType_;

public: /* Methods: */

    using zVerseBase::zVerseBase;

    template <typename ... Args>
    void findOffset(Args && ... args) const
    { return this->findOffset_<VerseSizeType>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void zReadText(Args && ... args) const
    { return this->zReadText_<VerseSizeType>(std::forward<Args>(args)...); }

    template <typename ... Args>
    static char createModule(Args && ... args)
    { return createModule_<VerseSizeType>(std::forward<Args>(args)...); }

protected: /* Methods: */

    template <typename ... Args>
    void doLinkEntry(Args && ... args)
    { return this->doLinkEntry_<VerseSizeType>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void doSetText(Args && ... args)
    { return this->doSetText_<VerseSizeType>(std::forward<Args>(args)...); }

};

} /* namespace swordxx */

#endif /* SWORDXX_ZVERSEBASE_H */
