/******************************************************************************
 *
 *  sapphire.cpp -    the Saphire II stream cipher class
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
 * Dedicated to the Public Domain the author and inventor:
 * (Michael Paul Johnson).  This code comes with no warranty.
 * Use it at your own risk.
 * Ported from the Pascal implementation of the Sapphire Stream
 * Cipher 9 December 1994.
 * Added hash pre- and post-processing 27 December 1994.
 * Modified initialization to make index variables key dependent,
 * made the output function more resistant to cryptanalysis,
 * and renamed to Sapphire II 2 January 1995
 *
 */

#include <cstring>

#include "sapphire.h"

namespace swordxx {

unsigned char sapphire::keyrand(unsigned limit,
                                unsigned char *user_key,
                                unsigned char keysize,
                                unsigned char *rsum,
                                unsigned *keypos)
    {
    unsigned u,             // Value from 0 to limit to return.
        retry_limiter,      // No infinite loops allowed.
        mask;               // Select just enough bits.

    if (!limit) return 0;   // Avoid divide by zero error.
    retry_limiter = 0;
    mask = 1;               // Fill mask with enough bits to cover
    while (mask < limit)    // the desired range.
        mask = (mask << 1) + 1;
    do
        {
        *rsum = cards[*rsum] + user_key[(*keypos)++];
        if (*keypos >= keysize)
            {
            *keypos = 0;            // Recycle the user key.
            *rsum += keysize;   // key "aaaa" != key "aaaaaaaa"
            }
        u = mask & *rsum;
        if (++retry_limiter > 11)
            u %= limit;     // Prevent very rare long loops.
        }
    while (u > limit);
    return static_cast<unsigned char>(u);
    }

void sapphire::initialize(unsigned char *key, unsigned char keysize)
    {
    // Key size may be up to 256 bytes.
    // Pass phrases may be used directly, with longer length
    // compensating for the low entropy expected in such keys.
    // Alternatively, shorter keys hashed from a pass phrase or
    // generated randomly may be used. For random keys, lengths
    // of from 4 to 16 bytes are recommended, depending on how
    // secure you want this to be.

    unsigned char toswap, swaptemp, rsum;
    unsigned keypos;

    // If we have been given no key, assume the default hash setup.

    if (keysize < 1)
        {
        hash_init();
        return;
        }

    // Start with cards all in order, one of each.

    for (unsigned i=0;i<256;i++)
        cards[i] = static_cast<unsigned char>(i);

    // Swap the card at each position with some other card.

    toswap = 0;
    keypos = 0;         // Start with first byte of user key.
    rsum = 0;
    for (unsigned i = 255u;; --i) {
        toswap = keyrand(i, key, keysize, &rsum, &keypos);
        swaptemp = cards[i];
        cards[i] = cards[toswap];
        cards[toswap] = swaptemp;
        if (i == 0u)
            break;
    }

    // Initialize the indices and data dependencies.
    // Indices are set to different values instead of all 0
    // to reduce what is known about the state of the cards
    // when the first byte is emitted.

    rotor = cards[1];
    ratchet = cards[3];
    avalanche = cards[5];
    last_plain = cards[7];
    last_cipher = cards[rsum];

    toswap = swaptemp = rsum = 0;
    keypos = 0;
    }

void sapphire::hash_init(void)
    {
    // This function is used to initialize non-keyed hash
    // computation.

    // Initialize the indices and data dependencies.

    rotor = 1;
    ratchet = 3;
    avalanche = 5;
    last_plain = 7;
    last_cipher = 11;

    // Start with cards all in inverse order.
    for (unsigned i = 0u; i < 256u; ++i)
        cards[i] = static_cast<unsigned char>(255u - i);
    }

sapphire::sapphire(unsigned char *key, unsigned char keysize)
    {
    if (key && keysize)
        initialize(key, keysize);
    }

void sapphire::burn(void)
    {
    // Destroy the key and state information in RAM.
    memset(cards, 0, 256);
    rotor = ratchet = avalanche = last_plain = last_cipher = 0;
    }

sapphire::~sapphire()
    {
    burn();
    }

unsigned char sapphire::encrypt(unsigned char b)
    {
    // Picture a single enigma rotor with 256 positions, rewired
    // on the fly by card-shuffling.

    // This cipher is a variant of one invented and written
    // by Michael Paul Johnson in November, 1993.

    unsigned char swaptemp;

    // Shuffle the deck a little more.

    ratchet += cards[rotor++];
    swaptemp = cards[last_cipher];
    cards[last_cipher] = cards[ratchet];
    cards[ratchet] = cards[last_plain];
    cards[last_plain] = cards[rotor];
    cards[rotor] = swaptemp;
    avalanche += cards[swaptemp];

    // Output one byte from the state in such a way as to make it
    // very hard to figure out which one you are looking at.

    last_cipher = b^cards[(cards[ratchet] + cards[rotor]) & 0xFF] ^
                  cards[cards[(cards[last_plain] +
                               cards[last_cipher] +
                               cards[avalanche])&0xFF]];
    last_plain = b;
    return last_cipher;
    }

unsigned char sapphire::decrypt(unsigned char b)
    {
    unsigned char swaptemp;

    // Shuffle the deck a little more.

    ratchet += cards[rotor++];
    swaptemp = cards[last_cipher];
    cards[last_cipher] = cards[ratchet];
    cards[ratchet] = cards[last_plain];
    cards[last_plain] = cards[rotor];
    cards[rotor] = swaptemp;
    avalanche += cards[swaptemp];

    // Output one byte from the state in such a way as to make it
    // very hard to figure out which one you are looking at.

    last_plain = b^cards[(cards[ratchet] + cards[rotor]) & 0xFF] ^
                   cards[cards[(cards[last_plain] +
                                cards[last_cipher] +
                                cards[avalanche])&0xFF]];
    last_cipher = b;
    return last_plain;
    }

void sapphire::hash_final(unsigned char *hash,      // Destination
                          unsigned char hashlength) // Size of hash.
    {
    for (int i = 255; i >= 0; --i)
        encrypt(static_cast<unsigned char>(i));
    for (int i = 0; i < hashlength; ++i)
        hash[i] = encrypt(0);
    }

} /* namespace swordxx */
