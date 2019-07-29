/******************************************************************************
 *
 *  roman.h -    roman numeral functions
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

/* char* to_rom(int num, char *p); */

/** Converts a roman numeral to a string.
* @param s Roman numeral to convert.
*/
int from_rom(const char *s);

} /* namespace swordxx */

#endif /* SWORDXX_ROMAN_H */
