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

#ifndef SWORDXX_ROMAN_H
#define SWORDXX_ROMAN_H

#include <cstdint>
#include <optional>
#include <string_view>
#include "defs.h"


namespace swordxx {

/**
  \brief Checks whether the given input is a Roman numeral digit.
  \param[in] c The input to check.
  \returns whether the given input is a Roman numeral digit.
*/
bool isRomanDigit(char const c) noexcept;

/**
  \brief Checks whether the given input is a Roman numeral.
  \param[in] sv The input to check.
  \returns whether the given input is a Roman numeral.
*/
bool isRoman(std::string_view sv) noexcept;

/**
  \brief Attempts to parse the value for the given Roman numeral input.
  \param[in] sv The input to parse.
  \returns the numeric value of the Roman numeral input, or empty on parse
           errors or when the numeric value doesn't fit into the return type.
*/
std::optional<std::uintmax_t> parseRomanNumeral(std::string_view sv) noexcept;

} /* namespace swordxx */

#endif /* SWORDXX_ROMAN_H */
