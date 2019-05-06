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

#ifndef SWORDXX_UNICODE_H
#define SWORDXX_UNICODE_H

#include <cstdint>
#include <string>
#include <string_view>
#include <utility>


namespace swordxx {

/**
  \brief Tries to parse the first UTF-8 character from the given input.
  \param[in] sv The input to parse.
  \returns a pair consisting of the codepoint for the first UTF-8 character and
           the number of input characters of its UTF-8 encoding.
  \retval {0u,0u} if the input does not correspond to valid UTF-8 or is
                  incomplete.
*/
std::pair<std::uint32_t, std::string_view::size_type>
codepointFromUtf8(std::string_view sv) noexcept;

/**
  \brief Parses the first UTF-8 character from the given input.
  \param[in] sv The input to parse.
  \pre Assumes that the input does starts with a valid UTF-8 character encoding.
  \returns a pair consisting of the codepoint for the first UTF-8 character and
           the number of input characters of its UTF-8 encoding.
*/
std::pair<std::uint32_t, std::string_view::size_type>
codepointFromValidUtf8(std::string_view sv) noexcept;

/**
  \brief Returns a UTF-8 representation of the given code point.
  \param[in] codepoint The Unicode codepoint to encode as UTF-8.
  \note If the given codepoint is invalid, an UTF-8 encoding for the Unicode
        replacement characters U+FFFD is returned.
  \returns a string consisting of the UTF-8 encoding for the given Unicode
           codepoint.
*/
std::string utf8FromCodepoint(std::uint32_t codepoint);

/**
  \brief Returns a UTF-8 representation of the given code point.
  \param[in] codepoint The Unicode codepoint to encode as UTF-8.
  \pre The codepoint given is a valid Unicode codepoint.
  \returns a string consisting of the UTF-8 encoding for the given Unicode
           codepoint.
*/
std::string utf8FromValidCodepoint(std::uint32_t codepoint);

/**
  \brief Lints the input as UTF-8, replacing invalid characters with Unicode
         replacement characters (U+FFFD).
  \param[in] sv The input to lint as UTF-8.
  \returns a string consisting of a valid UTF-8 encoding constructed from the
           input.
*/
std::string assureValidUTF8(std::string_view sv);

} /* namespace swordxx */

#endif /* SWORDXX_UNICODE_H */
