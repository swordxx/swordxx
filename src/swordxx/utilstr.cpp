/******************************************************************************
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "utilstr.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iterator>
#include <memory>
#include <new>
#include <type_traits>
#include <unicode/locid.h>
#include <unicode/translit.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <unicode/unorm2.h>
#include <unicode/ushape.h>
#include <unicode/ustring.h>
#include <unicode/utypes.h>
#include "max_v.h"
#include "sysdata.h"


namespace swordxx {

static_assert(max_v<unsigned char> == 255u,
              "Required by the constexpr tables in header file.");

void addTrailingDirectorySlash(std::string & buf) {
    assert(!buf.empty());
    switch (buf.back()) {
        case '/': case '\\':
            break;
        default:
            buf.push_back('/');
            break;
    }
}

void removeTrailingDirectorySlashes(std::string & buf) {
    while (buf.size() > 1u) {
        switch (buf.back()) {
            case '/': case '\\':
                buf.pop_back();
                break;
            default:
                return;
        }
    }
}

void leftTrimString(std::string & str) {
    auto begin(str.begin());
    str.erase(begin, std::find_if_not(begin, str.end(), charIsSpace));
}

void rightTrimString(std::string & str) {
    str.erase(std::find_if_not(str.rbegin(), str.rend(), charIsSpace).base(),
              str.end());
}

void trimString(std::string & str) {
    leftTrimString(str);
    rightTrimString(str);
}

std::string_view leftTrimmedView(std::string_view sv) noexcept {
    while (!sv.empty() && charIsSpace(sv.front()))
        sv.remove_prefix(1u);
    return sv;
}

std::string_view rightTrimmedView(std::string_view sv) noexcept {
    while (!sv.empty() && charIsSpace(sv.back()))
        sv.remove_suffix(1u);
    return sv;
}

std::string_view trimmedView(std::string_view sv) noexcept
{ return rightTrimmedView(leftTrimmedView(sv)); }

bool startsWith(std::string_view str, std::string_view prefix) {
    #if __cplusplus > 201703L
    return str.starts_with(prefix);
    #else
    return str.substr(0u, prefix.size()) == prefix;
    #endif
}

bool endsWith(std::string_view str, std::string_view suffix) {
    #if __cplusplus > 201703L
    return str.ends_with(suffix);
    #else
    if (str.size() < suffix.size())
        return false;
    str.remove_prefix(str.size() - suffix.size());
    return str == suffix;
    #endif
}

std::pair<bool, std::size_t> getPrefixSize(char const * const buf,
                                           char const separator)
{
    if (const char * const m = std::strchr(buf, separator))
        return std::make_pair(true, std::size_t(m - buf));
    return std::make_pair(false, std::size_t(0u));
}

std::pair<bool, std::size_t> getPrefixSize(std::string const & buf,
                                           char const separator)
{ return getPrefixSize(buf.c_str(), separator); }

std::string stripPrefix(std::string & str, char const separator) {
    char const * m = str.c_str();
    while (*m && *m != separator)
        ++m;
    auto const prefixSize(static_cast<std::size_t>(m - str.c_str()));
    if (prefixSize == str.size()) {
        std::string r(std::move(str));
        str.clear();
        return r;
    }
    std::string r(str.c_str(), prefixSize);
    str.erase(0u, prefixSize + 1u);
    return r;
}

bool caseInsensitiveEquals(std::string_view a, std::string_view b) noexcept {
    if (a.size() != b.size())
        return false;
    return std::equal(a.begin(), a.end(), b.begin(),
                      [](char const ac, char const bc) noexcept
                      { return asciiCharToUpper(ac) == asciiCharToUpper(bc); });
}


/******************************************************************************
 * stristr - Scans a string for the occurrence of a given substring, no case
 *
 * ENT:    scans haystack for the first occurrence of the substring needle,
           ingnoring case
 *
 * RET:    a pointer to the element in haystack, where needle begins (points to
           needle in haystack).
 *         If needle does not occur in haystack, returns nullptr.
 */

const char * stristr(std::string const & haystack, std::string const & needle)
        noexcept
{
    static auto const caseInsensitiveAsciiCompare =
            [](char const a, char const b) noexcept
            { return asciiCharToUpper(a) == asciiCharToUpper(b); };
    auto const first(haystack.begin());
    auto const last(haystack.end());
    auto const it(std::search(first, last,
                              needle.begin(), needle.end(),
                              caseInsensitiveAsciiCompare));
    if (it == last)
        return nullptr;
    return haystack.c_str() + std::distance(first, it);
}

/******************************************************************************
 * strnicmp - compares the first n bytes of 2 strings ignoring case
 *
 * ENT:    compares s1 to s2 comparing the first n byte ingnoring case
 *
 * RET:    same as std::strcmp
 */
int strnicmp(char const * s1, char const * s2, std::size_t n) noexcept {
    if ((!n) || (s1 == s2))
        return 0;
    int r;
    while ((r = static_cast<unsigned char>(asciiCharToUpper(*s1))
                - static_cast<unsigned char>(asciiCharToUpper(*s2))) == 0) {
        if ((*s1) == '\0' || ((--n) == 0u))
            break;
        ++s1;
        ++s2;
    }
    return r;
}

std::string utf8ToUpper(std::string_view sv)
{ return utf16ToUtf8(utf16ToUpper(utf8ToUtf16(sv))); }

std::basic_string<char16_t> utf16ToUpper(std::basic_string_view<char16_t> sv) {
    static_assert(max_v<std::int32_t> <= max_v<std::size_t>, "");
    if (sv.empty())
        return {};
    if (sv.size() > max_v<std::int32_t>)
        throw std::bad_array_new_length();

    // Calculate destination buffer size:
    std::int32_t ucSizeInUtf16;
    {
        assert(sv.size() <= max_v<std::int32_t>);
        ::UErrorCode err = ::U_ZERO_ERROR;
        ucSizeInUtf16 =
                ::u_strToUpper(nullptr,
                               0,
                               sv.data(),
                               static_cast<std::int32_t>(sv.size()),
                               nullptr,
                               &err);
        if (::U_FAILURE(err) && (err != ::U_BUFFER_OVERFLOW_ERROR))
            throw std::runtime_error("u_strToUpper() failed!");
    }
    assert(ucSizeInUtf16 >= 0);

    // Convert to uppercase:
    std::basic_string<char16_t> ucInUtf16;
    ucInUtf16.resize(static_cast<std::size_t>(ucSizeInUtf16));
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        ::u_strToUpper(ucInUtf16.data(),
                       ucSizeInUtf16,
                       sv.data(),
                       static_cast<std::int32_t>(sv.size()),
                       nullptr,
                       &err);
        if (::U_FAILURE(err))
            throw std::runtime_error("u_strToUpper() failed!");
    }
    return ucInUtf16;
}

namespace {

std::basic_string<char16_t> utf16Normalize(
        std::basic_string_view<char16_t> sv,
        ::UNormalizer2 const * const normalizer)
{
    static_assert(max_v<std::int32_t> <= max_v<std::size_t>, "");
    if (sv.empty())
        return {};
    if (sv.size() > max_v<std::int32_t>)
        throw std::bad_array_new_length();

    // Calculate destination buffer size:
    std::int32_t len;
    {
        char16_t buf;
        ::UErrorCode err = ::U_ZERO_ERROR;
        len = ::unorm2_normalize(normalizer,
                                 sv.data(),
                                 static_cast<std::int32_t>(sv.size()),
                                 &buf,
                                 1,
                                 &err);
        if (::U_FAILURE(err) && (err != ::U_BUFFER_OVERFLOW_ERROR))
            throw std::runtime_error("unorm2_normalize() failed!");
    }

    // Do the actual normalization:
    std::basic_string<char16_t> r;
    r.resize(static_cast<decltype(r)::size_type>(len));
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        ::unorm2_normalize(normalizer,
                           sv.data(),
                           static_cast<std::int32_t>(sv.size()),
                           r.data(),
                           len,
                           &err);
        if (::U_FAILURE(err))
            throw std::runtime_error("unorm2_normalize() failed!");
    }
    return r;
}

} // anonymous namespace

#define SWORDXX_DEFINE_NORMALIZER(Name,getter) \
    std::string utf8 ## Name ## Normalize(std::string_view sv) \
    { return utf16ToUtf8(utf16 ## Name ## Normalize(utf8ToUtf16(sv))); } \
    std::basic_string<char16_t> utf16 ## Name ## Normalize( \
            std::basic_string_view<char16_t> sv) \
    { \
        static auto const normalizer = \
            []() { \
                UErrorCode err = ::U_ZERO_ERROR; \
                auto const r = ::getter(&err); \
                if (::U_FAILURE(err)) \
                    throw std::runtime_error(#getter "() failed!"); \
                return r; \
            }(); \
        return utf16Normalize(sv, normalizer); \
    }
SWORDXX_DEFINE_NORMALIZER(Nfkd, unorm2_getNFKDInstance)
SWORDXX_DEFINE_NORMALIZER(Nfc, unorm2_getNFCInstance)
#undef SWORDXX_DEFINE_NORMALIZER

std::string utf8ShapeArabic(std::string_view sv)
{ return utf16ToUtf8(utf16ShapeArabic(utf8ToUtf16(sv))); }

std::basic_string<char16_t>
utf16ShapeArabic(std::basic_string_view<char16_t> sv) {
    static_assert(max_v<decltype(sv)::size_type> >= max_v<std::int32_t>, "");
    if (sv.size() > max_v<std::int32_t>)
        throw std::bad_array_new_length();

    static constexpr auto const options =
            U_SHAPE_LETTERS_SHAPE | U_SHAPE_DIGITS_EN2AN;

    std::int32_t destSize;
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        destSize = ::u_shapeArabic(sv.data(),
                                   static_cast<std::int32_t>(sv.size()),
                                   nullptr,
                                   0,
                                   options,
                                   &err);

        if (::U_FAILURE(err) && (err != ::U_BUFFER_OVERFLOW_ERROR))
            throw std::runtime_error("u_shapeArabic() failed!");
    }
    std::basic_string<char16_t> r;
    r.resize(static_cast<decltype(r)::size_type>(destSize));
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        destSize = ::u_shapeArabic(sv.data(),
                                   static_cast<std::int32_t>(sv.size()),
                                   r.data(),
                                   destSize,
                                   options,
                                   &err);

        if (::U_FAILURE(err))
            throw std::runtime_error("u_shapeArabic() failed!");
    }
    return r;
}

std::basic_string<char16_t> utf8ToUtf16(std::string_view sv) {
    static_assert(max_v<std::int32_t> <= max_v<std::size_t>, "");
    if (sv.empty())
        return {};
    if (sv.size() > max_v<std::int32_t>)
        throw std::bad_array_new_length();

    // Calculate destination buffer size:
    std::int32_t sizeInUtf16;
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        ::u_strFromUTF8(nullptr,
                        0,
                        &sizeInUtf16,
                        sv.data(),
                        static_cast<std::int32_t>(sv.size()),
                        &err);
        if (::U_FAILURE(err) && (err != ::U_BUFFER_OVERFLOW_ERROR))
            throw std::runtime_error("u_strFromUTF8() failed!");
    }
    assert(sizeInUtf16 >= 0);

    // Do the conversion:
    std::basic_string<char16_t> r;
    r.resize(static_cast<std::size_t>(sizeInUtf16));
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        ::u_strFromUTF8(r.data(),
                        sizeInUtf16,
                        nullptr,
                        sv.data(),
                        static_cast<std::int32_t>(sv.size()),
                        &err);
        if (::U_FAILURE(err))
            throw std::runtime_error("u_strFromUTF8() failed!");
    }
    return r;
}

std::string utf16ToUtf8(std::basic_string_view<char16_t> sv) {
    static_assert(max_v<std::int32_t> <= max_v<std::size_t>, "");
    if (sv.empty())
        return {};
    if (sv.size() > max_v<std::int32_t>)
        throw std::bad_array_new_length();

    // Calculate destination buffer size:
    std::int32_t sizeInUtf8;
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        ::u_strToUTF8(nullptr,
                      0,
                      &sizeInUtf8,
                      sv.data(),
                      static_cast<std::int32_t>(sv.size()),
                      &err);
        if (::U_FAILURE(err) && (err != ::U_BUFFER_OVERFLOW_ERROR))
            throw std::runtime_error("u_strToUTF8() failed!");
    }

    // Do the conversion:
    std::string r;
    r.resize(static_cast<std::size_t>(sizeInUtf8));
    {
        ::UErrorCode err = ::U_ZERO_ERROR;
        ::u_strToUTF8(r.data(),
                      sizeInUtf8,
                      nullptr,
                      sv.data(),
                      static_cast<std::int32_t>(sv.size()),
                      &err);
        if (::U_FAILURE(err))
            throw std::runtime_error("u_strToUTF8() failed!");
    }
    return r;
}

} /* namespace swordxx */
