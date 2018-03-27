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

#ifndef SWORDXX_ZTEXT_H
#define SWORDXX_ZTEXT_H

#include "ztextbase.h"
#include "../common/zverse.h"


namespace swordxx {

class zText: public zTextBase<zVerse> {

public: /* Methods: */

    using zTextBase::zTextBase;
    ~zText() noexcept override;

};

} /* namespace swordxx */

#endif /* SWORDXX_ZTEXT_H */
