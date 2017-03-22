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

#ifndef RAWVERSEBASE_H
#define RAWVERSEBASE_H

#include <cstdio>
#include <string>
#include "../../normalizedpath.h"


namespace swordxx {

class FileDesc;

class RawVerseBase {

public: /* Types: */

    using StartType = std::uint32_t;

protected: /* Fields: */

    FileDesc * idxfp[2];
    FileDesc * textfp[2];

    NormalizedPath m_path;

public: /* Methods: */

    RawVerseBase(NormalizedPath path, int fileMode = -1);
    virtual ~RawVerseBase() noexcept;

protected: /* Methods: */

    template <typename SizeType>
    void readText_(char testmt,
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
    template <typename SizeType>
    void findOffset_(char testmt,
                     long idxoff,
                     StartType * start,
                     SizeType * size) const;

    template <typename SizeType>
    void doSetText_(char testmt, long idxoff, char const * buf, long len);

    template <typename SizeType>
    void doLinkEntry_(char testmt, long destidxoff, long srcidxoff);

    template <typename SizeType>
    static char createModule_(NormalizedPath const & path, char const * v11n);

}; /* class RawVerseBase */

extern template
void RawVerseBase::readText_<std::uint16_t>(char testmt,
                                            StartType start,
                                            std::uint16_t size,
                                            std::string & buf) const;
extern template
void RawVerseBase::readText_<std::uint32_t>(char testmt,
                                            StartType start,
                                            std::uint32_t size,
                                            std::string & buf) const;

extern template
void RawVerseBase::findOffset_<std::uint16_t>(char testmt,
                                              long idxoff,
                                              StartType * start,
                                              std::uint16_t * size) const;
extern template
void RawVerseBase::findOffset_<std::uint32_t>(char testmt,
                                              long idxoff,
                                              StartType * start,
                                              std::uint32_t * size) const;

extern template
void RawVerseBase::doSetText_<std::uint16_t>(char testmt,
                                             long idxoff,
                                             char const * buf,
                                             long len);
extern template
void RawVerseBase::doSetText_<std::uint32_t>(char testmt,
                                             long idxoff,
                                             char const * buf,
                                             long len);

extern template
void RawVerseBase::doLinkEntry_<std::uint16_t>(char testmt,
                                               long destidxoff,
                                               long srcidxoff);
extern template
void RawVerseBase::doLinkEntry_<std::uint32_t>(char testmt,
                                               long destidxoff,
                                               long srcidxoff);

extern template
char RawVerseBase::createModule_<std::uint16_t>(
        NormalizedPath const & path,
        char const * v11n);
extern template
char RawVerseBase::createModule_<std::uint32_t>(
        NormalizedPath const & path,
        char const * v11n);

template <typename SizeType_>
class RawVerseBaseImpl: public RawVerseBase {

public: /* Types: */

    using SizeType = SizeType_;

public: /* Methods: */

    RawVerseBaseImpl(const char * ipath, int fileMode = -1)
        : RawVerseBase(ipath, fileMode)
    {}

    void readText(char testmt,
                  StartType start,
                  SizeType size,
                  std::string & buf) const
    { return this->readText_(testmt, start, size, buf); }

    void findOffset(char testmt,
                    long idxoff,
                    StartType * start,
                    SizeType * size) const
    { return this->findOffset_(testmt, idxoff, start, size); }

    static char createModule(const char *path, const char * v11n = "KJV")
    { return RawVerseBase::createModule_<SizeType>(path, v11n); }

protected: /* Methods: */

    void doSetText(char testmt, long idxoff, const char * buf, long len = -1)
    { return this->doSetText_<SizeType>(testmt, idxoff, buf, len); }

    void doLinkEntry(char testmt, long destidxoff, long srcidxoff)
    { return this->doLinkEntry_<SizeType>(testmt, destidxoff, srcidxoff); }

};

} /* namespace swordxx */

#endif /* RAWVERSEBASE_H */
