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

#ifndef SWORDXX_MAX_V_H
#define SWORDXX_MAX_V_H

#include <limits>


namespace swordxx {

template <typename T> constexpr auto max_v = std::numeric_limits<T>::max();

} /* namespace swordxx */

#endif /* SWORDXX_MAX_V_H */
