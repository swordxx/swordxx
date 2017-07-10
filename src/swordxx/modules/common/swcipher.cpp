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
    : buf(nullptr)
{ SWCipher::setCipherKey(key); }


/******************************************************************************
 * SWCipher Destructor - Cleans up instance of SWCipher
 */

SWCipher::~SWCipher()
{
    if (buf)
        free(buf);
}


char *SWCipher::Buf(const char *ibuf, unsigned long ilen)
{
    if (ibuf) {

        if (buf)
            free(buf);

        if (!ilen) {
                len = strlen(buf);
            ilen = len + 1;
        }
        else len = ilen;

        static_assert(std::numeric_limits<unsigned long>::max()
                      <= std::numeric_limits<std::size_t>::max(), "");
        buf = static_cast<char *>(malloc(ilen));
        if (!buf)
            throw std::bad_alloc();
        memcpy(buf, ibuf, ilen);
        cipher = false;
    }

    Decode();

    return buf;
}


char *SWCipher::cipherBuf(unsigned long *ilen, const char *ibuf)
{
    if (ibuf) {

        if (buf)
            free(buf);

        static_assert(std::numeric_limits<unsigned long>::max()
                      <= std::numeric_limits<std::size_t>::max(), "");
        std::size_t const bufSize = std::size_t(*ilen) + 1u;
        if (!bufSize)
            throw std::bad_alloc();
        buf = static_cast<char *>(malloc(bufSize));
        if (!buf)
            throw std::bad_alloc();
        memcpy(buf, ibuf, *ilen);
        len = *ilen;
        cipher = true;
    }

    Encode();

    *ilen = len;
    return buf;
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
    if (!cipher) {
        work = master;
        for (unsigned long i = 0; i < len; i++) {
            std::uint8_t c;
            static_assert(sizeof(char) == sizeof(std::uint8_t), "");
            std::memcpy(&c, buf + i, 1u);
            c = work.encrypt(c);
            std::memcpy(buf + i, &c, 1u);
        }
        cipher = true;
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
    if (cipher) {
        work = master;
        unsigned long i;
        for (i = 0; i < len; i++) {
            std::uint8_t c;
            static_assert(sizeof(char) == sizeof(std::uint8_t), "");
            std::memcpy(&c, buf + i, 1u);
            c = work.decrypt(c);
            std::memcpy(buf + i, &c, 1u);
        }
        buf[i] = 0;
        cipher = false;
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
    master.initialize(key, static_cast<unsigned char>(keySize));
}

} /* namespace swordxx */
