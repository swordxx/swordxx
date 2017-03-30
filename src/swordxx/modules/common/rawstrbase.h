/*****************************************************************************
 *
 *  rawstr.h -    code for class 'RawStr'- a module that reads raw text
 *        files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *        and provides lookup and parsing functions based on
 *        class StrKey
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

#ifndef SWORDXX_RAWSTRBASE_H
#define SWORDXX_RAWSTRBASE_H

#include <cstdint>
#include <string>
#include "../../defs.h"
#include "../../normalizedpath.h"


namespace swordxx {

class FileDesc;

class RawStrBase {

public: /* Types: */

    using StartType = std::uint32_t;
    using IndexOffsetType = std::uint32_t;

public: /* Methods: */

    RawStrBase(NormalizedPath const & path,
               int fileMode = -1,
               bool caseSensitive = false);

    virtual ~RawStrBase();

    std::string getIDXBuf(long ioffset) const;

    static signed char createModule(NormalizedPath const & path);

protected: /* Methods: */

    template <typename SizeType>
    signed char findOffset_(char const * key,
                            StartType * start,
                            SizeType * size,
                            long away = 0,
                            IndexOffsetType * idxoff = nullptr) const;

    template <typename SizeType>
    std::string readText_(StartType start,
                          SizeType * size,
                          std::string & buf) const;

    template <typename SizeType>
    void doSetText_(char const * key, char const * buf, long len = -1);

    template <typename SizeType>
    void doLinkEntry_(char const * destkey, char const * srckey);

private: /* Methods: */

    std::string getIDXBufDat(long ioffset) const;

protected: /* Fields: */

    FileDesc *idxfd;
    FileDesc *datfd;

private: /* Fields: */

    bool caseSensitive;
    mutable long lastoff; // for caching and optimizing

}; /* class RawStrBase */

extern template
signed char RawStrBase::findOffset_<std::uint16_t>(
        char const * key,
        StartType * start,
        std::uint16_t * size,
        long away = 0,
        IndexOffsetType * idxoff = nullptr) const;
extern template
signed char RawStrBase::findOffset_<std::uint32_t>(
        char const * key,
        StartType * start,
        std::uint32_t * size,
        long away = 0,
        IndexOffsetType * idxoff = nullptr) const;

extern template
std::string RawStrBase::readText_<std::uint16_t>(StartType start,
                                                 std::uint16_t * size,
                                                 std::string & buf) const;
extern template
std::string RawStrBase::readText_<std::uint32_t>(StartType start,
                                                 std::uint32_t * size,
                                                 std::string & buf) const;

extern template
void RawStrBase::doSetText_<std::uint16_t>(char const * key,
                                           char const * buf,
                                           long len = -1);
extern template
void RawStrBase::doSetText_<std::uint32_t>(char const * key,
                                           char const * buf,
                                           long len = -1);

extern template
void RawStrBase::doLinkEntry_<std::uint16_t>(char const * destkey,
                                             char const * srckey);
extern template
void RawStrBase::doLinkEntry_<std::uint32_t>(char const * destkey,
                                             char const * srckey);

template <typename SizeType_>
class RawStrBaseImpl: public RawStrBase {

public: /* Types: */

    using SizeType = SizeType_;

protected: /* Constants: */

    static constexpr std::size_t const IDXENTRYSIZE =
            sizeof(StartType) + sizeof(SizeType);

public: /* Methods: */

    using RawStrBase::RawStrBase;

    template <typename ... Args>
    signed char findOffset(Args && ... args) const
    { return RawStrBase::findOffset_<SizeType>(std::forward<Args>(args)...); }

    template <typename ... Args>
    std::string readText(Args && ... args) const
    { return RawStrBase::readText_<SizeType>(std::forward<Args>(args)...); }

protected: /* Methods: */

    template <typename ... Args>
    void doSetText(Args && ... args)
    { return RawStrBase::doSetText_<SizeType>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void doLinkEntry(Args && ... args)
    { return RawStrBase::doLinkEntry_<SizeType>(std::forward<Args>(args)...); }

}; /* class RawStrBaseImpl */

} /* namespace swordxx */

#endif /* SWORDXX_RAWSTRBASE_H */
