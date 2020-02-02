/******************************************************************************
 *
 *  rawcom.h -    code for class 'RawCom'- a module that reads raw commentary
 *        files:    ot and nt using indexs ??.bks ??.cps ??.vss
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

#ifndef SWORDXX_RAWCOMBASE_H
#define SWORDXX_RAWCOMBASE_H

#include "../../swcom.h"
#include "../common/rawverse.h"
#include "../common/rawverse4.h"

#include "../../defs.h"


namespace swordxx {

template <typename Base>
class SWDLLEXPORT RawComBase : public Base, public SWCom {

public: /* Types: */

    using StartType = typename Base::StartType;
    using SizeType = typename Base::SizeType;

public: /* Methods: */

    RawComBase(char const * ipath,
           char const * iname = nullptr,
           char const * idesc = nullptr,
           TextEncoding encoding = ENC_UNKNOWN,
           SWTextDirection dir = DIRECTION_LTR,
           SWTextMarkup markup = FMT_UNKNOWN,
           char const * ilang = nullptr,
           std::shared_ptr<VersificationMgr::System const> v11n = nullptr);
    ~RawComBase() override;

    void increment(int steps = 1) override;
    void decrement(int steps = 1) override;

    bool isWritable() const noexcept override;

    static char createModule(
            const char * path,
            std::shared_ptr<VersificationMgr::System const> v11n = nullptr);

    void setEntry(std::string_view text) override;
    void linkEntry(SWKey const & linkKey) override;
    void deleteEntry() override;
    bool isLinked(SWKey const & k1, SWKey const & k2) const override;
    bool hasEntry(SWKey const & k) const override;

protected: /* Methods: */

    std::string getRawEntryImpl() const override;

};

extern template class RawComBase<RawVerse>;
extern template class RawComBase<RawVerse4>;

} /* namespace swordxx */

#endif /* SWORDXX_RAWCOMBASE_H */
