/******************************************************************************
 *
 *  swcomprs.cpp -     a driver class that provides compression utilities
 *
 * $Id$
 *
 * Copyright 1996-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swcomprs.h"

#include <cstdlib>
#include <cstring>
#include <new>


namespace swordxx {
namespace {

char * saferMemoryOperationWrap(void * const ptr) {
    if (ptr)
        return static_cast<char *>(ptr);
    throw std::bad_alloc();
};

char * saferMalloc(std::size_t const size)
{ return saferMemoryOperationWrap(std::malloc(size)); };

char * saferRealloc(char * const ptr, std::size_t const size)
{ return saferMemoryOperationWrap(std::realloc(ptr, size)); };

char * saferCalloc(std::size_t const nmemb, std::size_t const size)
{ return saferMemoryOperationWrap(std::calloc(nmemb, size)); };

}

SWCompress::SWCompress() {}

SWCompress::~SWCompress() {
    std::free(zbuf);
    std::free(buf);
}

void SWCompress::reset() {
    std::free(buf);
    buf = nullptr;
    std::free(zbuf);
    zbuf = nullptr;
    direct = 0;
    zlen = 0;
    zpos = 0;
    pos = 0;
    slen = 0;
}

char * SWCompress::Buf(const char * ibuf, unsigned long * len) {
    // setting an uncompressed buffer
    if (ibuf) {
        auto const newBuffer = saferCalloc(slen + 1, 1);
        reset();
        slen = (len) ? *len : std::strlen(ibuf);
        buf = newBuffer;
        std::memcpy(buf, ibuf, slen);
    }

    // getting an uncompressed buffer
    if (!buf) {
        // be sure we at least allocate an empty buf for return:
        buf = saferCalloc(1, 1);
        direct = 1;
        Decode();
//        slen = strlen(buf);
        if (len)
            *len = slen;
    }
    return buf;
}

char * SWCompress::zBuf(unsigned long * len, char * ibuf) {
    // setting a compressed buffer
    if (ibuf) {
        auto const newBuffer = saferMalloc(*len);
        reset();
        zbuf = newBuffer;
        std::memcpy(zbuf, ibuf, *len);
        zlen = *len;
    }

    // getting a compressed buffer
    if (!zbuf) {
        direct = 0;
        Encode();
    }

    *len = zlen;
    return zbuf;
}

/// \note Override for other than buffer compression
unsigned long SWCompress::GetChars(char * ibuf, unsigned long len) {
    if (direct) {
        len = ((zlen - zpos) > len) ? len : zlen - zpos;
        if (len > 0) {
            std::memmove(ibuf, &zbuf[zpos], len);
            zpos += len;
        }
    } else {
//        slen = strlen(buf);
        len = ((slen - pos) > len) ? len : slen - pos;
        if (len > 0) {
            std::memmove(ibuf, &buf[pos], len);
            pos += len;
        }
    }
    return len;
}

/// \note override for other than buffer compression
unsigned long SWCompress::SendChars(char *ibuf, unsigned long len) {
    if (direct) {
        if (buf) {
//            slen = strlen(buf);
            if ((pos + len) > slen) {
                buf = saferRealloc(buf, pos + len + 1024);
                std::memset(&buf[pos], 0, len + 1024);
            }
        } else {
            buf = saferCalloc(1, len + 1024);
        }
        std::memmove(&buf[pos], ibuf, len);
        pos += len;
    } else {
        if (zbuf) {
            if ((zpos + len) > zlen) {
                zbuf = saferRealloc(zbuf, zpos + len + 1024);
                zlen = zpos + len + 1024;
            }
        } else {
            zbuf = saferCalloc(1, len + 1024);
            zlen = len + 1024;
        }
        std::memmove(&zbuf[zpos], ibuf, len);
        zpos += len;
    }
    return len;
}

/**
    \brief "Encodes" the input stream into the output stream.

    The GetChars() and SendChars() functions are used to separate this method
    from the actual I/O.

    \note Override to provide compression algorithm.
*/
void SWCompress::Encode() { cycleStream(); }

/**
    \brief "Decodes" the input stream into the output stream.

    The GetChars() and SendChars() functions are used to separate this method
    from the actual I/O.

    \note Override to provide compression algorithm.
*/
void SWCompress::Decode() { cycleStream(); }

void SWCompress::cycleStream() {
    char buf[1024];
    unsigned long len;
    unsigned long totlen = 0;

    do {
        len = GetChars(buf, 1024);
        if (len)
            totlen += SendChars(buf, len);
    } while (len == 1024);

    zlen = slen = totlen;
}

} /* namespace swordxx */
