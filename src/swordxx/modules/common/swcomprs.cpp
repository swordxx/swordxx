/******************************************************************************
 *
 *  swcomprs.cpp -     a driver class that provides compression utilities
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
    std::free(m_zbuf);
    std::free(m_buf);
}

void SWCompress::reset() noexcept {
    std::free(m_buf);
    m_buf = nullptr;
    std::free(m_zbuf);
    m_zbuf = nullptr;
    m_zlen = 0;
    m_zpos = 0;
    m_pos = 0;
    m_slen = 0;
}

char * SWCompress::Buf(const char * ibuf, unsigned long * len) {
    // setting an uncompressed buffer
    if (ibuf) {
        std::size_t const newLength = len ? *len : std::strlen(ibuf);
        auto const newBuffer = saferCalloc(newLength + 1, 1);
        reset();
        m_slen = newLength;
        m_buf = newBuffer;
        std::memcpy(m_buf, ibuf, m_slen);
    }

    // getting an uncompressed buffer
    if (!m_buf) {
        // be sure we at least allocate an empty buf for return:
        m_buf = saferCalloc(1, 1);
        Decode();
//        slen = std::strlen(buf);
        if (len)
            *len = m_slen;
    }
    return m_buf;
}

char * SWCompress::zBuf(unsigned long * len, char * ibuf) {
    // setting a compressed buffer
    if (ibuf) {
        auto const newBuffer = saferMalloc(*len);
        reset();
        m_zbuf = newBuffer;
        std::memcpy(m_zbuf, ibuf, *len);
        m_zlen = *len;
    }

    // getting a compressed buffer
    if (!m_zbuf) {
        Encode();
    }

    *len = m_zlen;
    return m_zbuf;
}

/// \note Override for other than buffer compression
unsigned long SWCompress::GetChars(char * ibuf, unsigned long len, Direction dir) {
    if (dir == DECODE) {
        len = ((m_zlen - m_zpos) > len) ? len : m_zlen - m_zpos;
        if (len > 0) {
            std::memmove(ibuf, &m_zbuf[m_zpos], len);
            m_zpos += len;
        }
    } else {
//        slen = std::strlen(buf);
        len = ((m_slen - m_pos) > len) ? len : m_slen - m_pos;
        if (len > 0) {
            std::memmove(ibuf, &m_buf[m_pos], len);
            m_pos += len;
        }
    }
    return len;
}

/// \note override for other than buffer compression
unsigned long SWCompress::SendChars(char *ibuf, unsigned long len, Direction dir) {
    if (dir == DECODE) {
        if (m_buf) {
//            slen = std::strlen(buf);
            if ((m_pos + len) > m_slen) {
                m_buf = saferRealloc(m_buf, m_pos + len + 1024);
                std::memset(&m_buf[m_pos], 0, len + 1024);
            }
        } else {
            m_buf = saferCalloc(1, len + 1024);
        }
        std::memmove(&m_buf[m_pos], ibuf, len);
        m_pos += len;
    } else {
        if (m_zbuf) {
            if ((m_zpos + len) > m_zlen) {
                m_zbuf = saferRealloc(m_zbuf, m_zpos + len + 1024);
                m_zlen = m_zpos + len + 1024;
            }
        } else {
            m_zbuf = saferCalloc(1, len + 1024);
            m_zlen = len + 1024;
        }
        std::memmove(&m_zbuf[m_zpos], ibuf, len);
        m_zpos += len;
    }
    return len;
}

/**
    \brief "Encodes" the input stream into the output stream.

    The GetChars() and SendChars() functions are used to separate this method
    from the actual I/O.

    \note Override to provide compression algorithm.
*/
void SWCompress::Encode() { cycleStream(ENCODE); }

/**
    \brief "Decodes" the input stream into the output stream.

    The GetChars() and SendChars() functions are used to separate this method
    from the actual I/O.

    \note Override to provide compression algorithm.
*/
void SWCompress::Decode() { cycleStream(DECODE); }

void SWCompress::cycleStream(Direction dir) {
    char buf_[1024];
    unsigned long len;
    unsigned long totlen = 0;

    do {
        len = GetChars(buf_, 1024, dir);
        if (len)
            totlen += SendChars(buf_, len, dir);
    } while (len == 1024);

    m_zlen = m_slen = totlen;
}

} /* namespace swordxx */
