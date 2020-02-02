/*****************************************************************************
 *
 *  rawstr.h -    code for class 'RawStr'- a module that reads raw text
 *        files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *        and provides lookup and parsing functions based on
 *        class StrKey
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
#include <memory>
#include <string>
#include <string_view>
#include "../../defs.h"
#include "../../normalizedpath.h"


namespace swordxx {

class FileDesc;

template <typename SizeType_>
class RawStrBase {

public: /* Types: */

    using StartType = std::uint32_t;
    using SizeType = SizeType_;
    using IndexOffsetType = std::uint32_t;

protected: /* Constants: */

    static constexpr std::size_t const IDXENTRYSIZE =
            sizeof(StartType) + sizeof(SizeType);

public: /* Methods: */

    RawStrBase(NormalizedPath const & path,
               int fileMode = -1,
               bool m_caseSensitive = false);

    virtual ~RawStrBase();

    std::string getIDXBuf(long ioffset) const;

    static signed char createModule(NormalizedPath const & path);

protected: /* Methods: */

    signed char findOffset(std::string_view key,
                           StartType * start,
                           SizeType * size,
                           long away = 0,
                           IndexOffsetType * idxoff = nullptr) const;

    std::string readText(StartType start,
                         SizeType size,
                         std::string & buf) const;

    void doSetText(char const * key, std::string_view text);

    void doLinkEntry(char const * destkey, char const * srckey);

private: /* Methods: */

    std::string getIDXBufDat(long ioffset) const;

protected: /* Fields: */

    std::shared_ptr<FileDesc> idxfd;
    std::shared_ptr<FileDesc> datfd;

private: /* Fields: */

    bool m_caseSensitive;
    mutable long m_lastoff; // for caching and optimizing

}; /* class RawStrBase */

extern template class RawStrBase<std::uint16_t>;
extern template class RawStrBase<std::uint32_t>;

} /* namespace swordxx */

#endif /* SWORDXX_RAWSTRBASE_H */
