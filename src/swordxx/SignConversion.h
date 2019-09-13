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

#ifndef SWORDXX_SIGNCONVERSION_H
#define SWORDXX_SIGNCONVERSION_H

#include <type_traits>


namespace swordxx {

template <typename T>
constexpr auto toSigned(T v) noexcept
        -> std::enable_if_t<std::is_integral_v<T>, std::make_signed_t<T>>
{ return static_cast<std::make_signed_t<T>>(v); }

template <typename T>
constexpr auto toUnsigned(T v) noexcept
        -> std::enable_if_t<std::is_integral_v<T>, std::make_unsigned_t<T>>
{ return static_cast<std::make_unsigned_t<T>>(v); }

} /* namespace swordxx */

#endif /* SWORDXX_SIGNCONVERSION_H */
