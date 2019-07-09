/***************************************************************************
 *
 * Copyright 2016 Jaak Ristioja
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

#ifndef SWORDXX_OFFONOPTIONFILTER_H
#define SWORDXX_OFFONOPTIONFILTER_H

#include "swoptfilter.h"


namespace swordxx {

class OffOnOptionFilter: public SWOptionFilter {

public: /* Methods: */

    OffOnOptionFilter(char const * name, char const * tip);
    ~OffOnOptionFilter() noexcept;

    bool isOptionOn() const noexcept
    { return getSelectedOptionValueIndex() == 1u; }

    bool isOptionOff() const noexcept
    { return getSelectedOptionValueIndex() == 0u; }

};

} /* namespace swordxx */

#endif /* SWORDXX_OFFONOPTIONFILTER_H */
