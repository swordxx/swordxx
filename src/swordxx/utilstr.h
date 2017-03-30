/******************************************************************************
 *
 *  utilstr.h -    prototypes for string utility functions
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

#ifndef UTILSTR_H
#define UTILSTR_H

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>
#include <utility>
#include "defs.h"


namespace swordxx {

void addTrailingDirectorySlash(std::string & buf);
void removeTrailingDirectorySlashes(std::string & buf);

inline bool charIsSpace(char const c) noexcept {
    switch (c) {
        case ' ': case '\n': case '\r': case '\t': return true;
        default: return false;
    }
}

void leftTrimString(std::string & str) noexcept;
void rightTrimString(std::string & str) noexcept;
void trimString(std::string & str) noexcept;

inline bool hasPrefix(char const * str, char const * prefix) noexcept {
    while (*prefix != '\0') {
        if (*prefix != *str)
            return false;
        ++prefix;
        ++str;
    }
    return true;
}

inline bool hasPrefix(std::string const & str,
                      char const * const prefix) noexcept
{ return hasPrefix(str.c_str(), prefix); }

inline bool hasPrefix(char const * str,
                      std::string const & prefix) noexcept
{ return hasPrefix(str, prefix.c_str()); }

inline bool hasPrefix(std::string const & str,
                      std::string const & prefix) noexcept
{ return hasPrefix(str.c_str(), prefix.c_str()); }

template <typename T> struct OkFormattedType {
    static constexpr bool const value =
            std::is_arithmetic<T>::value || std::is_pointer<T>::value;
};

template <typename ...> struct OkFormattedTypes;
template <> struct OkFormattedTypes<>
{ static constexpr bool const value = true; };

template <typename T> struct OkFormattedTypes<T>: OkFormattedType<T> {};

template <typename T, typename ... Ts> struct OkFormattedTypes<T, Ts...> {
    static constexpr bool const value =
            OkFormattedType<T>::value && OkFormattedTypes<Ts...>::value;
};

template <typename ... Args>
inline std::string formatted(const char * const formatString,
                             Args && ... args)
{
    static_assert(OkFormattedTypes<std::decay_t<Args>...>::value,
                  "Invalid arguments!");
    std::string buf(std::snprintf(nullptr, 0u, formatString, args...), char());
    std::sprintf(&buf[0u], formatString, std::forward<Args>(args)...);
    return buf;
}

inline std::pair<bool, std::size_t> getPrefixSize(char const * const buf,
                                                  char const separator)
{
    if (const char * const m = std::strchr(buf, separator))
        return std::make_pair(true, std::size_t(m - buf));
    return std::make_pair(false, std::size_t(0u));
}

inline std::pair<bool, std::size_t> getPrefixSize(std::string const & buf,
                                                  char const separator)
{ return getPrefixSize(buf.c_str(), separator); }

std::string stripPrefix(std::string & str, char const separator);

SWDLLEXPORT char const * stristr(std::string const & haystack,
                                 std::string const & needle);
SWDLLEXPORT int stricmp(char const * s1, char const * s2);
SWDLLEXPORT int strnicmp(char const * s1, char const * s2, std::size_t len);

constexpr char const latin1CharToUpperTable[256u] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    -128, -127, -126, -125, -124, -123, -122, -121,
    -120, -119, -118, -117, -116, -115, -114, -113,
    -112, -111, -110, -109, -108, -107, -106, -105,
    -104, -103, -102, -101, -100,  -99,  -98,  -97,
     -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,
     -88,  -87,  -86,  -85,  -84,  -83,  -82,  -81,
     -80,  -79,  -78,  -77,  -76,  -75,  -74,  -73,
     -72,  -71,  -70,  -69,  -68,  -67,  -66,  -65,
     -64,  -63,  -62,  -61,  -60,  -59,  -58,  -57,
     -56,  -55,  -54,  -53,  -52,  -51,  -50,  -49,
     -48,  -47,  -46,  -45,  -44,  -43,  -42,  -41,
     -40,  -39,  -38,  -37,  -36,  -35,  -34,  -33,
     -64,  -63,  -62,  -61,  -60,  -59,  -58,  -57,
     -56,  -55,  -54,  -53,  -52,  -51,  -50,  -49,
     -48,  -47,  -46,  -45,  -44,  -43,  -42,   -9,
     -40,  -39,  -38,  -37,  -36,  -35,  -34,   -1
};
constexpr char latin1CharToUpper(char const c) noexcept
{ return latin1CharToUpperTable[static_cast<unsigned char>(c)]; }

constexpr char const asciiCharToUpperTable[256u] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
    0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f,
    0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    0x60, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47,
    0x48, 0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f,
    0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57,
    0x58, 0x59, 0x5a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f,
    -128, -127, -126, -125, -124, -123, -122, -121,
    -120, -119, -118, -117, -116, -115, -114, -113,
    -112, -111, -110, -109, -108, -107, -106, -105,
    -104, -103, -102, -101, -100,  -99,  -98,  -97,
     -96,  -95,  -94,  -93,  -92,  -91,  -90,  -89,
     -88,  -87,  -86,  -85,  -84,  -83,  -82,  -81,
     -80,  -79,  -78,  -77,  -76,  -75,  -74,  -73,
     -72,  -71,  -70,  -69,  -68,  -67,  -66,  -65,
     -64,  -63,  -62,  -61,  -60,  -59,  -58,  -57,
     -56,  -55,  -54,  -53,  -52,  -51,  -50,  -49,
     -48,  -47,  -46,  -45,  -44,  -43,  -42,  -41,
     -40,  -39,  -38,  -37,  -36,  -35,  -34,  -33,
     -32,  -31,  -30,  -29,  -28,  -27,  -26,  -25,
     -24,  -23,  -22,  -21,  -20,  -19,  -18,  -17,
     -16,  -15,  -14,  -13,  -12,  -11,  -10,   -9,
      -8,   -7,   -6,   -5,   -4,   -3,   -2,   -1
};
constexpr char asciiCharToUpper(char const c) noexcept
{ return asciiCharToUpperTable[static_cast<unsigned char>(c)]; }

/******************************************************************************
 * getUniCharFromUTF8 - retrieves the next Unicode codepoint from a UTF8 string
 *                     and increments buf to start of next codepoint
 *
 * ENT:    buf - address of a utf8 buffer
 *
 * RET:    buf - incremented past last byte used in computing the current codepoint
 *         unicode codepoint value (0 with buf incremented is invalid UTF8 byte
 */

uint32_t getUniCharFromUTF8(const unsigned char **buf);


/******************************************************************************
 * getUTF8FromUniChar - retrieves us UTF8 string from a
 *                     Unicode codepoint
 *
 * ENT:    uchar - unicode codepoint value
 *
 * RET:    buf - a UTF8 string which consists of the proper UTF8 sequence of
 *                 bytes for the given Unicode codepoint
 */

std::string getUTF8FromUniChar(uint32_t uchar);


/******************************************************************************
 * assureValidUTF8 - iterates the supplied UTF-8 buffer and checks for validity
 *                     replacing invalid bytes if necessary and returning a
 *                    verified UTF8 buffer, leaving the original input
 *                    unchanged.
 *
 * ENT:    buf - a utf8 buffer
 *
 * RET:    input buffer validated and any problems fixed by substituting a
 *         replacement character for bytes not valid.
 */
std::string assureValidUTF8(const char *buf);

/****
 * This can be called to convert a UTF8 stream to a wstring.
 */
std::wstring utf8ToWChar(const char *buf);

/****
 * This can be called to convert a wchar_t[] to a UTF-8 string.
 *
 */
std::string wcharToUTF8(const wchar_t *buf);



} /* namespace swordxx */
#endif
