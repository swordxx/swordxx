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

#ifndef SWORDXX_RAWSTR_H
#define SWORDXX_RAWSTR_H

#include "rawstrbase.h"


namespace swordxx {

class SWDLLEXPORT RawStr: public RawStrBase<std::uint16_t> {

public: /* Methods: */

    using RawStrBase::RawStrBase;
    ~RawStr() noexcept override;

}; /* class RawStr */

} /* namespace swordxx */

#endif /* SWORDXX_RAWSTR_H */
