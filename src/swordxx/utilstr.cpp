/******************************************************************************
 *
 *  utilstr.cpp -    String utility functions
 *
 * $Id$
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
#include <type_traits>
#include "sysdata.h"


namespace swordxx {

void addTrailingDirectorySlash(std::string & buf) {
    assert(!buf.empty());
    switch (*buf.rbegin()) {
        case '/': case '\\':
            break;
        default:
            buf.push_back('/');
            break;
    }
}

void removeTrailingDirectorySlashes(std::string & buf) {
    while (buf.size() > 1u) {
        switch (*buf.rbegin()) {
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
    while (!sv.empty() && charIsSpace(*sv.begin()))
        sv.remove_prefix(1u);
    return sv;
}

std::string_view rightTrimmedView(std::string_view sv) noexcept {
    while (!sv.empty() && charIsSpace(*sv.rbegin()))
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

bool caseInsensitiveEquals(std::string const & a,
                           std::string const & b) noexcept
{
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

} /* namespace swordxx */
