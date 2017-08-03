/******************************************************************************
 *
 *  sapphire.h -    the Saphire II stream cipher class
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

/******************************************************************************
 *
 * Original license notice & credits:
 * Dedicated to the Public Domain the author and inventor
 * (Michael Paul Johnson).  This code comes with no warranty.
 * Use it at your own risk.
 * Ported from the Pascal implementation of the Sapphire Stream
 * Cipher 9 December 1994.
 * Added hash-specific functions 27 December 1994.
 * Made index variable initialization key-dependent,
 * made the output function more resistant to cryptanalysis,
 * and renamed to Sapphire II Stream Cipher 2 January 1995.
 */

#include <array>
#include <cstddef>
#include <cstdint>
#include "../../defs.h"


namespace swordxx {

class Sapphire {

public: /* Methods: */

    /** Calls initialize(9 if a real key is provided. Otherwise, initialize() is
        called before encrypt() or decrypt(). */
    Sapphire(std::uint8_t * key = nullptr, std::size_t keysize = 0);
    ~Sapphire();

    void initialize(std::uint8_t * key, std::size_t keysize);

    /// Encrypts a byte or returns a random byte.
    std::uint8_t encrypt(std::uint8_t b = 0);

    /// Decrypts a byte.
    std::uint8_t decrypt(std::uint8_t b);

private: /* Methods: */

    // This function is used by initialize(), which is called by the
    // constructor.
    std::uint8_t keyrand(unsigned limit,
                         std::uint8_t * user_key,
                         std::size_t keysize,
                         std::uint8_t * rsum,
                         unsigned * keypos);

private: /* Fields: */

    // These variables comprise the state of the state machine.
    std::array<std::uint8_t, 256u> cards;    // A permutation of 0-255.
    std::uint8_t rotor; // Index that rotates smoothly
    std::uint8_t ratchet; // Index that moves erratically
    std::uint8_t avalanche; // Index heavily data dependent
    std::uint8_t last_plain; // Last plain text byte
    std::uint8_t last_cipher; // Last cipher text byte

};

} /* namespace swordxx */
