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

#ifndef SWORDXX_UNICODECONVERTER_H
#define SWORDXX_UNICODECONVERTER_H

#include <string>
#include <string_view>


namespace swordxx {

class UnicodeConverter final {

public: /* Methods: */

    UnicodeConverter(char const * type);
    UnicodeConverter(UnicodeConverter &&) noexcept;
    UnicodeConverter(UnicodeConverter const &) = delete;

    ~UnicodeConverter() noexcept;

    UnicodeConverter & operator=(UnicodeConverter &&) noexcept;
    UnicodeConverter & operator=(UnicodeConverter const &) = delete;

    std::basic_string<char16_t> toUnicode(std::string_view sv);
    std::string fromUnicode(std::basic_string_view<char16_t> sv);

private: /* Fields: */

    void * m_p;

};

} /* namespace swordxx */

#endif /* SWORDXX_UNICODECONVERTER_H */
