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

#ifndef SWORDXX_ZCOM_H
#define SWORDXX_ZCOM_H

#include "zcombase.h"


namespace swordxx {

class SWDLLEXPORT zCom : public zComBase<zVerse> {

public: /* Methods: */

    using zComBase::zComBase;
    ~zCom() noexcept override;

}; /* class zCom */

} /* namespace swordxx */

#endif /* SWORDXX_ZCOM_H */
