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

SWCompress::SWCompress()
{
    buf = zbuf = nullptr;
    level = 6;
    reset();
}

SWCompress::~SWCompress()
{
    if (zbuf)
        std::free(zbuf);

    if (buf)
        std::free(buf);
}


void SWCompress::reset()
{
        if (buf)
            std::free(buf);

        if (zbuf)
            std::free(zbuf);

        buf    = nullptr;
        zbuf   = nullptr;
        direct  = 0;
        zlen    = 0;
        slen    = 0;
        zpos    = 0;
        pos     = 0;
}


char *SWCompress::Buf(const char *ibuf, unsigned long *len) {
    // setting an uncompressed buffer
    if (ibuf) {
        reset();
        slen = (len) ? *len : std::strlen(ibuf);
        buf = static_cast<char *>(std::calloc(slen + 1, 1));
        std::memcpy(buf, ibuf, slen);
    }

    // getting an uncompressed buffer
    if (!buf) {
        // be sure we at least allocate an empty buf for return:
        buf = static_cast<char *>(std::calloc(1, 1));
        direct = 1;
        Decode();
//        slen = strlen(buf);
        if (len)
            *len = slen;
    }
    return buf;
}


char *SWCompress::zBuf(unsigned long *len, char *ibuf)
{
    // setting a compressed buffer
    if (ibuf) {
        reset();
        zbuf = static_cast<char *>(std::malloc(*len));
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


unsigned long SWCompress::GetChars(char *ibuf, unsigned long len)
{
    if (direct) {
        len = (((zlen - zpos) > (unsigned)len) ? len : zlen - zpos);
        if (len > 0) {
            std::memmove(ibuf, &zbuf[zpos], len);
            zpos += len;
        }
    }
    else {
//        slen = strlen(buf);
        len = (((slen - pos) > (unsigned)len) ? len : slen - pos);
        if (len > 0) {
            std::memmove(ibuf, &buf[pos], len);
            pos += len;
        }
    }
    return len;
}


unsigned long SWCompress::SendChars(char *ibuf, unsigned long len)
{
    /** \bug Just avoids a null dereference by throwing std::bad_alloc, which is
             not properly handled. */
    static auto const saferRealloc =
            [](void * const ptr, std::size_t const size) {
                if (auto * const r = std::realloc(ptr, size))
                    return static_cast<char *>(r);
                throw std::bad_alloc();
            };

    if (direct) {
        if (buf) {
//            slen = strlen(buf);
            if ((pos + len) > (unsigned)slen) {
                buf = saferRealloc(buf, pos + len + 1024);
                std::memset(&buf[pos], 0, len + 1024);
            }
        } else {
            buf = static_cast<char *>(std::calloc(1, len + 1024));
        }
        std::memmove(&buf[pos], ibuf, len);
        pos += len;
    }
    else {
        if (zbuf) {
            if ((zpos + len) > zlen) {
                zbuf = saferRealloc(zbuf, zpos + len + 1024);
                zlen = zpos + len + 1024;
            }
        }
        else {
            zbuf = static_cast<char *>(std::calloc(1, len + 1024));
            zlen = len + 1024;
        }
        std::memmove(&zbuf[zpos], ibuf, len);
        zpos += len;
    }
    return len;
}


/******************************************************************************
 * SWCompress::Encode    - This function "encodes" the input stream into the
 *                        output stream.
 *                        The GetChars() and SendChars() functions are
 *                        used to separate this method from the actual
 *                        i/o.
 */

void SWCompress::Encode(void)
{
    cycleStream();
}


/******************************************************************************
 * SWCompress::Decode    - This function "decodes" the input stream into the
 *                        output stream.
 *                        The GetChars() and SendChars() functions are
 *                        used to separate this method from the actual
 *                        i/o.
 */

void SWCompress::Decode(void)
{
    cycleStream();
}


void SWCompress::cycleStream() {
    char buf[1024];
    unsigned long len, totlen = 0;

    do {
        len = GetChars(buf, 1024);
        if (len)
            totlen += SendChars(buf, len);
    } while (len == 1024);

    zlen = slen = totlen;
}

} /* namespace swordxx */
