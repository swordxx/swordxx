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

#include "UnicodeConversionFilter.h"

#include "utilstr.h"


namespace swordxx {

UnicodeConversionFilter::UnicodeConversionFilter(char const * const type)
    : m_converter(type)
{}

std::string UnicodeConversionFilter::decode(std::string_view sv)
{ return utf16ToUtf8(m_converter.toUnicode(sv)); }

std::string UnicodeConversionFilter::encode(std::string_view sv)
{ return m_converter.fromUnicode(utf8ToUtf16(sv)); }

} /* namespace swordxx */
