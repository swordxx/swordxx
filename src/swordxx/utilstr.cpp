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

unsigned char const SW_toupper_array[256] = {
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
    0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87,
    0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f,
    0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97,
    0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f,
    0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
    0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf,
    0xb0, 0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7,
    0xb8, 0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf,
    0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7,
    0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf,
    0xd0, 0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7,
    0xd8, 0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf,
    0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
    0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};


/******************************************************************************
 * stdstr - clones a string
 *
 * ENT:    ipstr    - pointer to a string pointer to set if necessary
 *    istr    - string to set to *ipstr
 *            0 - only get
 *
 * RET:    *ipstr
 */

char *stdstr(char **ipstr, const char *istr, unsigned int memPadFactor) {
    delete[] *ipstr;
    if (istr) {
        int len = strlen(istr) + 1;
        *ipstr = new char [ len * memPadFactor ];
        memcpy(*ipstr, istr, len);
    }
    else *ipstr = nullptr;
    return *ipstr;
}


/******************************************************************************
 * strstrip - Removes leading and trailing spaces from a string
 *
 * ENT:    istr    - string pointer to strip
 *
 * RET:    *istr
 */

char *strstrip(char *istr) {
    char *tmp = istr;
    char *rtmp;

    int len = strlen(istr);
    if (len < 1)
        return istr;
    rtmp = istr + (len - 1);

    while ((rtmp > istr)&&((*rtmp == ' ')||(*rtmp == '\t')||(*rtmp == 10)||(*rtmp == 13))) *(rtmp--) = 0;
    while ((*tmp == ' ')||(*tmp == '\t')||(*tmp == 10)||(*tmp == 13)) tmp++;
    memmove(istr, tmp, (rtmp - tmp) + 1);
    istr[(rtmp - tmp) + 1] = 0;

    return istr;
}


/******************************************************************************
 * stristr - Scans a string for the occurrence of a given substring, no case
 *
 * ENT:    scans s1 for the first occurrence of the substring s2, ingnoring case
 *
 * RET:    a pointer to the element in s1, where s2 begins (points to s2 in s1).
 *            If s2 does not occur in s1, returns null.
 */

const char *stristr(const char *s1, const char *s2) {
    int tLen = strlen(s2);
    int cLen = strlen(s1);
    char *target = new char [ tLen + 1 ];
    int i, j;
    char const * retVal = nullptr;

    strcpy(target, s2);
    for (i = 0; i < tLen; i++)
        target[i] = SW_toupper(target[i]);

    for (i = 0; i < (cLen - tLen)+1; i++) {
        if (SW_toupper(s1[i]) == (unsigned char)*target) {
            for (j = 1; j < tLen; j++) {
                if (SW_toupper(s1[i+j]) != (unsigned char)target[j])
                    break;
            }
            if (j == tLen) {
                retVal = s1+i;
                break;
            }
        }
    }
    delete [] target;
    return retVal;
}

/******************************************************************************
 * strnicmp - compares the first n bytes of 2 strings ignoring case
 *
 * ENT:    compares s1 to s2 comparing the first n byte ingnoring case
 *
 * RET:    same as strcmp
 */

int strnicmp(const char *s1, const char *s2, int len) {
    int tLen = strlen(s2);
    int cLen = strlen(s1);
    char diff;
    int i;
    for (i = 0; ((i < len) && (i < tLen) && (i < cLen)); i++) {
        if ((diff = SW_toupper(*s1) - SW_toupper(*s2)))
            return diff;
    s1++;
    s2++;
    }
    return (i < len) ? cLen - tLen : 0;
}

int stricmp(const char *s1, const char *s2) {
#if defined(__GNUC__)
    return ::strcasecmp(s1, s2);
#else
 #if defined(_WIN32_WCE)
    return ::_stricmp(s1, s2);
 #else
    return ::stricmp(s1, s2);
 #endif
#endif
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
