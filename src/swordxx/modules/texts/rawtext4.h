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

#ifndef SWORDXX_RAWTEXT4_H
#define SWORDXX_RAWTEXT4_H

#include "rawtextbase.h"
#include "../common/rawverse4.h"


namespace swordxx {

class RawText4: public RawTextBase<RawVerse4> {

public: /* Methods: */

    using RawTextBase::RawTextBase;

};

} /* namespace swordxx */

#endif /* SWORDXX_RAWTEXT4_H */
