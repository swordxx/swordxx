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


#ifndef SWORDXX_RAWVERSE4_H
#define SWORDXX_RAWVERSE4_H

#include "rawversebase.h"

#include <cstdint>


namespace swordxx {

class SWDLLEXPORT RawVerse4: public RawVerseBaseImpl<std::uint32_t> {

public: /* Methods: */

    using RawVerseBaseImpl::RawVerseBaseImpl;

};

} /* namespace swordxx */

#endif /* SWORDXX_RAWVERSE4_H */
