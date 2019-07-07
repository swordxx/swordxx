/******************************************************************************
 *
 *  rawverse.h -    code for class 'RawVerse'- a module that reads raw text
 *            files:  ot and nt using indexs ??.bks ??.cps ??.vss
 *            and provides lookup and parsing functions based on
 *            class VerseKey
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

#ifndef SWORDXX_RAWVERSEBASE_H
#define SWORDXX_RAWVERSEBASE_H

#include <cstdio>
#include <memory>
#include <string>
#include "../../normalizedpath.h"


namespace swordxx {

class FileDesc;

template <typename SizeType_>
class RawVerseBase {

public: /* Types: */

    using StartType = std::uint32_t;
    using SizeType = SizeType_;

protected: /* Fields: */

    std::shared_ptr<FileDesc> idxfp[2];
    std::shared_ptr<FileDesc> textfp[2];

    NormalizedPath m_path;

public: /* Methods: */

    RawVerseBase(NormalizedPath path, int fileMode = -1);
    virtual ~RawVerseBase() noexcept;

protected: /* Methods: */

    void readText(char testmt,
                  StartType start,
                  SizeType size,
                  std::string & buf) const;

    /**
      \brief Finds the offset of the key verse from the indexes.
      \param testmt testament to find (0 - Bible/module introduction)
      \param idxoff offset into .vss
      \param start address to store the starting offset
      \param size address to store the size of the entry
    */
    void findOffset(char testmt,
                    long idxoff,
                    StartType * start,
                    SizeType * size) const;

    void doSetText(char testmt, long idxoff, char const * buf, long len = -1);
    void doLinkEntry(char testmt, long destidxoff, long srcidxoff);
    static char createModule(NormalizedPath const & path,
                             char const * v11n = "KJV");

}; /* class RawVerseBase */

extern template class RawVerseBase<std::uint16_t>;
extern template class RawVerseBase<std::uint32_t>;

} /* namespace swordxx */

#endif /* SWORDXX_RAWVERSEBASE_H */
