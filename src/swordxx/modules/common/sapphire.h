/******************************************************************************
 *
 *  sapphire.h -    the Saphire II stream cipher class
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

#ifndef SWORDXX_SAPPHIRE_H
#define SWORDXX_SAPPHIRE_H

#include <cstddef>
#include <cstdint>
#include "../../defs.h"


namespace swordxx {

class Sapphire {

public: /* Types: */

    enum NoInitialization { DontInitialize };

private: /* Types: */

    struct State;

public: /* Methods: */

    Sapphire(NoInitialization const);
    Sapphire();
    Sapphire(Sapphire &&) noexcept;
    Sapphire(Sapphire const &);
    Sapphire(std::uint8_t * key, std::size_t keysize);

    virtual ~Sapphire() noexcept;

    Sapphire & operator=(Sapphire &&) noexcept;
    Sapphire & operator=(Sapphire const &) noexcept;

    /** Default-initializes or re-default-initializes the cipher state. */
    void defaultInitialize();

    /** Initializes or re-initializes the cipher state. */
    void initialize(std::uint8_t * key, std::size_t keysize);

    /// Encrypts a byte or returns a random byte.
    std::uint8_t encrypt(std::uint8_t b = 0);

    /// Decrypts a byte.
    std::uint8_t decrypt(std::uint8_t b);

private: /* Fields: */

    State * m_state;

};

} /* namespace swordxx */

#endif /* SWORDXX_SAPPHIRE_H */
