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
#include <cctype>
#include <cstring>
#include <iterator>
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

const char * stristr(std::string const & haystack, std::string const & needle) {
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

int stricmp(char const * s1, char const * s2) {
    if (s1 == s2)
        return 0;
    int r;
    while ((r = static_cast<unsigned char>(asciiCharToUpper(*s1))
                - static_cast<unsigned char>(asciiCharToUpper(*s2))) == 0) {
        if ((*s1) == '\0')
            break;
        ++s1;
        ++s2;
    }
    return r;
}

/******************************************************************************
 * strnicmp - compares the first n bytes of 2 strings ignoring case
 *
 * ENT:    compares s1 to s2 comparing the first n byte ingnoring case
 *
 * RET:    same as strcmp
 */
int strnicmp(char const * s1, char const * s2, std::size_t n) {
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

uint32_t getUniCharFromUTF8(const unsigned char **buf) {
    uint32_t ch = 0;
    unsigned char multibuf[7];

    //case: We're at the end
    if (!(**buf)) {
        return ch;
    }

    //case: ANSI
    if (!(**buf & 128)) {
        ch = **buf;
        (*buf)++;
        return ch;
    }

    //case: Invalid UTF-8 (illegal continuing byte in initial position)
    if ((**buf & 128) && (!(**buf & 64))) {
        (*buf)++;
        return ch;
    }

    //case: 2+ byte codepoint
    multibuf[0] = **buf;
    multibuf[0] <<= 1;
    int subsequent;
    for (subsequent = 1; (multibuf[0] & 128) && (subsequent < 7); subsequent++) {
        multibuf[0] <<= 1;
        multibuf[subsequent] = (*buf)[subsequent];
        multibuf[subsequent] &= 63;
        // subsequent byte did not begin with 10XXXXXX
        // move our buffer to here and error out
        if (((*buf)[subsequent] - multibuf[subsequent]) != 128) {
            *buf += subsequent;
            return 0;
        }
        ch <<= 6;
        ch |= multibuf[subsequent];
    }
    subsequent--;
    multibuf[0] <<= 1;
    char significantFirstBits = 8 - (2+subsequent);

    ch |= (((int16_t)multibuf[0]) << (((6*subsequent)+significantFirstBits)-8));
    *buf += (subsequent+1);
    return ch;
}


std::string getUTF8FromUniChar(uint32_t uchar) {
    std::string retVal;
    retVal.reserve(7u);
    unsigned int i;

    if (uchar < 0x80) {
        retVal.push_back((unsigned char)uchar);
        retVal.resize(1);
    }
    else if (uchar < 0x800) {
        retVal.resize(2);
        i = uchar & 0x3f;
        retVal[1] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x1f;
        retVal[0] = (unsigned char)(0xc0 | i);
    }
    else if (uchar < 0x10000) {
        retVal.resize(3);
        i = uchar & 0x3f;
        retVal[2] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[1] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x0f;
        retVal[0] = (unsigned char)(0xe0 | i);
    }
    else if (uchar < 0x200000) {
        retVal.resize(4);
        i = uchar & 0x3f;
        retVal[3] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[2] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[1] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x07;
        retVal[0] = (unsigned char)(0xf0 | i);
    }
    else if (uchar < 0x4000000) {
        retVal.resize(5);
        i = uchar & 0x3f;
        retVal[4] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[3] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[2] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[1] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x03;
        retVal[0] = (unsigned char)(0xf8 | i);
    }
    else if (uchar < 0x80000000) {
        retVal.resize(6);
        i = uchar & 0x3f;
        retVal[5] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[4] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[3] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[2] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x3f;
        retVal[1] = (unsigned char)(0x80 | i);
        uchar >>= 6;

        i = uchar & 0x01;
        retVal[0] = (unsigned char)(0xfc | i);
    }

    return retVal;
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
