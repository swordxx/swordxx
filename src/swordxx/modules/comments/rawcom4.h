/******************************************************************************
 *
 * Copyright Jaak Ristioja
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

#ifndef SWORDXX_RAWCOM4_H
#define SWORDXX_RAWCOM4_H

#include "rawcombase.h"


namespace swordxx {

class SWDLLEXPORT RawCom4 : public RawComBase<RawVerse4> {

public: /* Methods: */

    using RawComBase::RawComBase;
    ~RawCom4() noexcept override;

}; /* class RawCom */

} /* namespace swordxx */

#endif /* SWORDXX_RAWCOM4_H */
