/******************************************************************************
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

SWCipher::~SWCipher() noexcept
{ free(m_buf); }


char * SWCipher::buf(char const * ibuf, std::size_t ilen) {
    if (ibuf) {

        free(m_buf);

        if (!ilen) {
                m_len = std::strlen(m_buf);
            ilen = m_len + 1;
        }
        else m_len = ilen;

        m_buf = static_cast<char *>(malloc(ilen));
        if (!m_buf)
            throw std::bad_alloc();
        std::memcpy(m_buf, ibuf, ilen);
        m_cipher = false;
    }

    // Decode:
    if (m_cipher) {
        m_work = m_master;
        std::size_t i;
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

    return m_buf;
}


char * SWCipher::cipherBuf(std::size_t * ilen, char const * ibuf) {
    if (ibuf) {

        free(m_buf);

        std::size_t const bufSize = *ilen + 1u;
        if (!bufSize)
            throw std::bad_alloc();
        m_buf = static_cast<char *>(malloc(bufSize));
        if (!m_buf)
            throw std::bad_alloc();
        std::memcpy(m_buf, ibuf, *ilen);
        m_len = *ilen;
        m_cipher = true;
    }

    // Encode:
    if (!m_cipher) {
        m_work = m_master;
        for (std::size_t i = 0; i < m_len; ++i) {
            std::uint8_t c;
            static_assert(sizeof(char) == sizeof(std::uint8_t), "");
            std::memcpy(&c, m_buf + i, 1u);
            c = m_work.encrypt(c);
            std::memcpy(m_buf + i, &c, 1u);
        }
        m_cipher = true;
    }

    *ilen = m_len;
    return m_buf;
}


/******************************************************************************
 * SWCipher::setCipherKey    - setter for a new CipherKey
 *
 */

void SWCipher::setCipherKey(char const * ikey) {
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
