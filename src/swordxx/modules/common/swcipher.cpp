/******************************************************************************
 *
 *  swcipher.cpp -    code for class 'SWCipher'- a driver class that
 *            provides cipher utilities
 *
 * $Id$
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swcipher.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <new>


namespace swordxx {

/******************************************************************************
 * SWCipher Constructor - Initializes data for instance of SWCipher
 *
 */

SWCipher::SWCipher(char const * key)
    : m_buf(nullptr)
{ SWCipher::setCipherKey(key); }


/******************************************************************************
 * SWCipher Destructor - Cleans up instance of SWCipher
 */

SWCipher::~SWCipher()
{
    if (m_buf)
        free(m_buf);
}


char *SWCipher::Buf(const char *ibuf, unsigned long ilen)
{
    if (ibuf) {

        if (m_buf)
            free(m_buf);

        if (!ilen) {
                m_len = std::strlen(m_buf);
            ilen = m_len + 1;
        }
        else m_len = ilen;

        static_assert(std::numeric_limits<unsigned long>::max()
                      <= std::numeric_limits<std::size_t>::max(), "");
        m_buf = static_cast<char *>(malloc(ilen));
        if (!m_buf)
            throw std::bad_alloc();
        std::memcpy(m_buf, ibuf, ilen);
        m_cipher = false;
    }

    Decode();

    return m_buf;
}


char *SWCipher::cipherBuf(unsigned long *ilen, const char *ibuf)
{
    if (ibuf) {

        if (m_buf)
            free(m_buf);

        static_assert(std::numeric_limits<unsigned long>::max()
                      <= std::numeric_limits<std::size_t>::max(), "");
        std::size_t const bufSize = std::size_t(*ilen) + 1u;
        if (!bufSize)
            throw std::bad_alloc();
        m_buf = static_cast<char *>(malloc(bufSize));
        if (!m_buf)
            throw std::bad_alloc();
        std::memcpy(m_buf, ibuf, *ilen);
        m_len = *ilen;
        m_cipher = true;
    }

    Encode();

    *ilen = m_len;
    return m_buf;
}


/******************************************************************************
 * SWCipher::Encode    - This function "encodes" the input stream into the
 *                        output stream.
 *                        The GetChars() and SendChars() functions are
 *                        used to separate this method from the actual
 *                        i/o.
 */

void SWCipher::Encode(void)
{
    if (!m_cipher) {
        m_work = m_master;
        for (unsigned long i = 0; i < m_len; i++) {
            std::uint8_t c;
            static_assert(sizeof(char) == sizeof(std::uint8_t), "");
            std::memcpy(&c, m_buf + i, 1u);
            c = m_work.encrypt(c);
            std::memcpy(m_buf + i, &c, 1u);
        }
        m_cipher = true;
    }
}


/******************************************************************************
 * SWCipher::Decode    - This function "decodes" the input stream into the
 *                        output stream.
 *                        The GetChars() and SendChars() functions are
 *                        used to separate this method from the actual
 *                        i/o.
 */

void SWCipher::Decode(void)
{
    if (m_cipher) {
        m_work = m_master;
        unsigned long i;
        for (i = 0; i < m_len; i++) {
            std::uint8_t c;
            static_assert(sizeof(char) == sizeof(std::uint8_t), "");
            std::memcpy(&c, m_buf + i, 1u);
            c = m_work.decrypt(c);
            std::memcpy(m_buf + i, &c, 1u);
        }
        m_buf[i] = 0;
        m_cipher = false;
    }
}


/******************************************************************************
 * SWCipher::setCipherKey    - setter for a new CipherKey
 *
 */

void SWCipher::setCipherKey(const char *ikey) {
    auto const keySize = std::strlen(ikey);
    assert(keySize < 256u); /// \todo Throw exception instead!?
    static_assert(sizeof(char) == sizeof(std::uint8_t), "");
    static_assert(alignof(char) == sizeof(char), "");
    static_assert(alignof(char) == alignof(std::uint8_t), "");
    std::uint8_t key[255u];
    std::memcpy(key, ikey, keySize);
    m_master.initialize(key, keySize);
}

} /* namespace swordxx */
