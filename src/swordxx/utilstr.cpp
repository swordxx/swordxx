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

void leftTrimString(std::string & str) noexcept {
    auto begin(str.begin());
    str.erase(begin, std::find_if_not(begin, str.end(), charIsSpace));
}

void rightTrimString(std::string & str) noexcept {
    str.erase(std::find_if_not(str.rbegin(), str.rend(), charIsSpace).base(),
              str.end());
}

void trimString(std::string & str) noexcept {
    leftTrimString(str);
    rightTrimString(str);
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


/******************************************************************************
 * getUniCharFromUTF8 - retrieves the next Unicode codepoint from a UTF8 string
 *                     and increments buf to start of next codepoint
 *
 * ENT:    buf - address of a utf8 buffer
 *
 * RET:    buf - incremented past last byte used in computing the current codepoint
 *         unicode codepoint value (0 with buf incremented is invalid UTF8 byte
 */

uint32_t getUniCharFromUTF8(unsigned char const ** buf, ConvertFlags flags) {
    std::uint32_t ch = 0;

    //case: We're at the end
    if (!(**buf))
        return ch;

    //case: ANSI
    if (!(**buf & 128)) {
        ch = **buf;
        (*buf)++;
        return ch;
    }

    //case: Invalid UTF-8 (illegal continuing byte in initial position)
    if ((**buf >> 6) == 2) {
        (*buf)++;
        return ch;
    }

    //case: 2+ byte codepoint
    int subsequent = 1;
    if ((**buf & 32) == 0) { subsequent = 1; }
    else if ((**buf & 16) == 0) { subsequent = 2; }
    else if ((**buf &  8) == 0) { subsequent = 3; }
    else if ((**buf &  4) == 0) { subsequent = 4; }
    else if ((**buf &  2) == 0) { subsequent = 5; }
    else if ((**buf &  1) == 0) { subsequent = 6; }
    else subsequent = 7; // is this legal?

    ch = **buf & (0xFF>>(subsequent + 1));

    for (int i = 1; i <= subsequent; ++i) {
        // subsequent byte did not begin with 10XXXXXX
        // move our buffer to here and error out
        // this also catches our null if we hit the string terminator
        if (((*buf)[i] >> 6) != 2) {
            *buf += i;
            return 0;
        }
        ch <<= 6;
        ch |= (*buf)[i] & 63;
    }
    *buf += (subsequent+1);

    using U = std::underlying_type<ConvertFlags>::type;
    if (!(static_cast<U>(flags) & static_cast<U>(ConvertFlags::SkipValidation)))
    {
        // I THINK THIS IS STUPID BUT THE SPEC SAYS NO MORE THAN 4 BYTES
        if (subsequent > 3) ch = 0;
        // AGAIN stupid, but spec says UTF-8 can't use more than 21 bits
        if (ch > 0x1FFFFF) ch = 0;
        // This would be out of Unicode bounds
        if (ch > 0x10FFFF) ch = 0;
        // these would be values which could be represented in less bytes
        if (ch < 0x80 && subsequent > 0) ch = 0;
        if (ch < 0x800 && subsequent > 1) ch = 0;
        if (ch < 0x10000 && subsequent > 2) ch = 0;
        if (ch < 0x200000 && subsequent > 3) ch = 0;
    }

    return ch;
}


std::string getUTF8FromUniChar(uint32_t uchar) {
    // This would be out of Unicode bounds
    if (uchar > 0x10FFFF)
        uchar = 0xFFFD;
#define C(...) static_cast<char>(static_cast<unsigned char>(__VA_ARGS__))
    if (uchar < 0x80) {
        auto const c = C(uchar);
        return std::string(&c, 1u);
    }
    auto const uchar6 = uchar >> 6;
    if (uchar < 0x800) {
        char const cs[] = {
            C(0xc0 | (uchar6 & 0x1f)),
            C(0x80 | (uchar  & 0x3f))
        };
        return std::string(cs, 2u);
    }
    auto const uchar12 = uchar6 >> 6;
    if (uchar < 0x10000) {
        char const cs[] = {
            C(0xe0 | (uchar12 & 0x0f)),
            C(0x80 | (uchar6  & 0x3f)),
            C(0x80 | (uchar   & 0x3f))
        };
        return std::string(cs, 3u);
    }
    auto const uchar18 = uchar12 >> 6;
    char const cs[] = {
        C(0xf0 | (uchar18 & 0x07)),
        C(0x80 | (uchar12 & 0x3f)),
        C(0x80 | (uchar6  & 0x3f)),
        C(0x80 | (uchar   & 0x3f))
    };
#undef C
    return std::string(cs, 4u);
}


std::string assureValidUTF8(const char *buf) {
    assert(buf);
    std::string myCopy = buf;
    const unsigned char *b = (const unsigned char *)myCopy.c_str();
    unsigned char const * q = nullptr;
    bool invalidChar = false;
    while (*b) {
        q = b;
        if (!getUniCharFromUTF8(&b)) {
            long len = b - q;
            if (len) {
                invalidChar = true;
                for (long start = q - (const unsigned char *)myCopy.c_str(); len; len--) {
                    myCopy[start+len-1] = 0x1a;    // unicode replacement character
                }

            }
        }
    }
    if (invalidChar) { /** \bug !?!?!? */ }
    return myCopy;
}

std::wstring utf8ToWChar(const char *buf) {
    assert(buf);
    char const * q = nullptr;
    std::wstring wcharBuf;
    while (*buf) {
        q = buf;
        wchar_t wc = getUniCharFromUTF8((const unsigned char **)&buf);
        if (!wc) {
            // if my buffer was advanced but nothing was converted, I had invalid data
            if (buf - q) {
                // invalid bytes in UTF8 stream
                wcharBuf.push_back((wchar_t)0x1a);        // unicode replacement character
            }
        }
        else wcharBuf.push_back(wc);
    }
    return wcharBuf;
}


/****
 * This can be called to convert a wchar_t[] to a UTF-8 std::string
 *
 */
std::string wcharToUTF8(const wchar_t * buf) {
    assert(buf);
    std::string utf8Buf;
    for (; *buf; ++buf)
        utf8Buf.append(getUTF8FromUniChar(*buf));
    return utf8Buf;
}


} /* namespace swordxx */
