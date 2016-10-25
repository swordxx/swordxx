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

/** stdstr - clone a string
*/
SWDLLEXPORT char *stdstr (char **iistr, const char *istr, unsigned int memPadFactor = 1);
SWDLLEXPORT char *strstrip (char *istr);
SWDLLEXPORT const char *stristr (const char *s1, const char *s2);
SWDLLEXPORT int strnicmp(const char *s1, const char *s2, int len);
SWDLLEXPORT int stricmp(const char *s1, const char *s2);

/******************************************************************************
 * SW_toupper - array of uppercase values for any given Latin-1 value
 *
 * use this instead of toupper() for fast lookups on accented characters
 */
extern const unsigned char SW_toupper_array[256];
#define SW_toupper(c) SW_toupper_array[(unsigned char)c]

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
