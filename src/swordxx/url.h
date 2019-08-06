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

#ifndef SWORDXX_URL_H
#define SWORDXX_URL_H

#include <string>
#include <string_view>


namespace swordxx {
namespace URL {

std::string encode(std::string_view url);

} /* namespace URL */
} /* namespace swordxx */

#endif /* SWORDXX_URL_H */
