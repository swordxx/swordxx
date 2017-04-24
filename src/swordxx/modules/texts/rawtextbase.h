/******************************************************************************
 *
 *  rawtext.h -    code for class 'RawText'- a module that reads raw text files:
 *        ot and nt using indexs ??.bks ??.cps ??.vss
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

#ifndef SWORDXX_RAWTEXTBASE_H
#define SWORDXX_RAWTEXTBASE_H

#include "../../swtext.h"
#include "../common/rawverse.h"
#include "../common/rawverse4.h"

#include "../../defs.h"


namespace swordxx {

template <typename BaseRawVerse>
class SWDLLEXPORT RawTextBase: public SWText, public BaseRawVerse {

public: /* Types: */

    using StartType = typename BaseRawVerse::StartType;
    using SizeType = typename BaseRawVerse::SizeType;

public: /* Methods: */

    RawTextBase(char const * ipath,
                char const * iname = nullptr,
                char const * idesc = nullptr,
                TextEncoding encoding = ENC_UNKNOWN,
                SWTextDirection dir = DIRECTION_LTR,
                SWTextMarkup markup = FMT_UNKNOWN,
                char const * ilang = nullptr,
                char const * versification = "KJV");
    ~RawTextBase() override;
    std::string & getRawEntryBuf() const override;
    void increment(int steps = 1) override;
    bool isWritable() const override;

    static char createModule(char const * path, char const * v11n = "KJV");

    void setEntry(char const * inbuf, long len = -1) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;

    bool isLinked(SWKey const * k1, SWKey const * k2) const override;
    bool hasEntry(SWKey const * k) const override;

}; /* class RawTextBase */

extern template
RawTextBase<RawVerse>::RawTextBase(char const * ipath,
                                   char const * iname = nullptr,
                                   char const * idesc = nullptr,
                                   TextEncoding encoding = ENC_UNKNOWN,
                                   SWTextDirection dir = DIRECTION_LTR,
                                   SWTextMarkup markup = FMT_UNKNOWN,
                                   char const * ilang = nullptr,
                                   char const * versification = "KJV");
extern template
RawTextBase<RawVerse4>::RawTextBase(char const * ipath,
                                    char const * iname = nullptr,
                                    char const * idesc = nullptr,
                                    TextEncoding encoding = ENC_UNKNOWN,
                                    SWTextDirection dir = DIRECTION_LTR,
                                    SWTextMarkup markup = FMT_UNKNOWN,
                                    char const * ilang = nullptr,
                                    char const * versification = "KJV");

extern template RawTextBase<RawVerse>::~RawTextBase();
extern template RawTextBase<RawVerse4>::~RawTextBase();

extern template std::string & RawTextBase<RawVerse>::getRawEntryBuf() const;
extern template std::string & RawTextBase<RawVerse4>::getRawEntryBuf() const;

extern template void RawTextBase<RawVerse>::increment(int steps = 1);
extern template void RawTextBase<RawVerse4>::increment(int steps = 1);

extern template bool RawTextBase<RawVerse>::isWritable() const;
extern template bool RawTextBase<RawVerse4>::isWritable() const;

extern template char RawTextBase<RawVerse>::createModule(
        char const * path,
        char const * v11n = "KJV");
extern template char RawTextBase<RawVerse4>::createModule(
        char const * path,
        char const * v11n = "KJV");

extern template void RawTextBase<RawVerse>::setEntry(char const * inbuf,
                                                     long len = -1);
extern template void RawTextBase<RawVerse4>::setEntry(char const * inbuf,
                                                      long len = -1);

extern template void RawTextBase<RawVerse>::linkEntry(SWKey const & linkKey);
extern template void RawTextBase<RawVerse4>::linkEntry(SWKey const & linkKey);

extern template void RawTextBase<RawVerse>::deleteEntry();
extern template void RawTextBase<RawVerse4>::deleteEntry();

extern template bool RawTextBase<RawVerse>::isLinked(SWKey const * k1,
                                                     SWKey const * k2) const;
extern template bool RawTextBase<RawVerse4>::isLinked(SWKey const * k1,
                                                      SWKey const * k2) const;

extern template bool RawTextBase<RawVerse>::hasEntry(SWKey const * k) const;
extern template bool RawTextBase<RawVerse4>::hasEntry(SWKey const * k) const;

} /* namespace swordxx */

#endif /* SWORDXX_RAWTEXTBASE_H */
