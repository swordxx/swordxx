/******************************************************************************
 *
 * Copyright 2017 Jaak Ristioja
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

#ifndef SWORDXX_RAWSTR4_H
#define SWORDXX_RAWSTR4_H

#include "rawstrbase.h"


namespace swordxx {

class FileDesc;

class SWDLLEXPORT RawStr4: public RawStrBase {

public: /* Types: */

    using SizeType = std::uint32_t;

protected: /* Constants: */

    static constexpr std::size_t const IDXENTRYSIZE =
            sizeof(StartType) + sizeof(SizeType);

public: /* Methods: */

    using RawStrBase::RawStrBase;

    template <typename ... Args>
    signed char findOffset(Args && ... args) const
    { return RawStrBase::findOffset_<SizeType>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void readText(Args && ... args) const
    { return RawStrBase::readText_<SizeType>(std::forward<Args>(args)...); }

protected: /* Methods: */

    template <typename ... Args>
    void doSetText(Args && ... args)
    { return RawStrBase::doSetText_<SizeType>(std::forward<Args>(args)...); }

    template <typename ... Args>
    void doLinkEntry(Args && ... args)
    { return RawStrBase::doLinkEntry_<SizeType>(std::forward<Args>(args)...); }

}; /* class RawStr4 */

} /* namespace swordxx */

#endif /* SWORDXX_RAWSTR4_H */
