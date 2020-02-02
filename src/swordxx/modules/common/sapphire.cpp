/******************************************************************************
 *
 *  sapphire.cpp -    the Saphire II stream cipher class
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

#include "sapphire.h"

#include <array>
#include <cassert>
#include <cerrno>
#include <cstring>
#include <sys/mman.h>
#include <type_traits>
#include <utility>


namespace swordxx {
namespace {

std::array<std::uint8_t, 256u> const inorder{{
    0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u, 12u, 13u, 14u, 15u,
    16u, 17u, 18u, 19u, 20u, 21u, 22u, 23u, 24u, 25u, 26u, 27u, 28u, 29u,
    30u, 31u, 32u, 33u, 34u, 35u, 36u, 37u, 38u, 39u, 40u, 41u, 42u, 43u,
    44u, 45u, 46u, 47u, 48u, 49u, 50u, 51u, 52u, 53u, 54u, 55u, 56u, 57u,
    58u, 59u, 60u, 61u, 62u, 63u, 64u, 65u, 66u, 67u, 68u, 69u, 70u, 71u,
    72u, 73u, 74u, 75u, 76u, 77u, 78u, 79u, 80u, 81u, 82u, 83u, 84u, 85u,
    86u, 87u, 88u, 89u, 90u, 91u, 92u, 93u, 94u, 95u, 96u, 97u, 98u, 99u,
    100u, 101u, 102u, 103u, 104u, 105u, 106u, 107u, 108u, 109u, 110u, 111u,
    112u, 113u, 114u, 115u, 116u, 117u, 118u, 119u, 120u, 121u, 122u, 123u,
    124u, 125u, 126u, 127u, 128u, 129u, 130u, 131u, 132u, 133u, 134u, 135u,
    136u, 137u, 138u, 139u, 140u, 141u, 142u, 143u, 144u, 145u, 146u, 147u,
    148u, 149u, 150u, 151u, 152u, 153u, 154u, 155u, 156u, 157u, 158u, 159u,
    160u, 161u, 162u, 163u, 164u, 165u, 166u, 167u, 168u, 169u, 170u, 171u,
    172u, 173u, 174u, 175u, 176u, 177u, 178u, 179u, 180u, 181u, 182u, 183u,
    184u, 185u, 186u, 187u, 188u, 189u, 190u, 191u, 192u, 193u, 194u, 195u,
    196u, 197u, 198u, 199u, 200u, 201u, 202u, 203u, 204u, 205u, 206u, 207u,
    208u, 209u, 210u, 211u, 212u, 213u, 214u, 215u, 216u, 217u, 218u, 219u,
    220u, 221u, 222u, 223u, 224u, 225u, 226u, 227u, 228u, 229u, 230u, 231u,
    232u, 233u, 234u, 235u, 236u, 237u, 238u, 239u, 240u, 241u, 242u, 243u,
    244u, 245u, 246u, 247u, 248u, 249u, 250u, 251u, 252u, 253u, 254u, 255u
}};

std::array<std::uint8_t, 256u> const inverse{{
    255u, 254u, 253u, 252u, 251u, 250u, 249u, 248u, 247u, 246u, 245u, 244u,
    243u, 242u, 241u, 240u, 239u, 238u, 237u, 236u, 235u, 234u, 233u, 232u,
    231u, 230u, 229u, 228u, 227u, 226u, 225u, 224u, 223u, 222u, 221u, 220u,
    219u, 218u, 217u, 216u, 215u, 214u, 213u, 212u, 211u, 210u, 209u, 208u,
    207u, 206u, 205u, 204u, 203u, 202u, 201u, 200u, 199u, 198u, 197u, 196u,
    195u, 194u, 193u, 192u, 191u, 190u, 189u, 188u, 187u, 186u, 185u, 184u,
    183u, 182u, 181u, 180u, 179u, 178u, 177u, 176u, 175u, 174u, 173u, 172u,
    171u, 170u, 169u, 168u, 167u, 166u, 165u, 164u, 163u, 162u, 161u, 160u,
    159u, 158u, 157u, 156u, 155u, 154u, 153u, 152u, 151u, 150u, 149u, 148u,
    147u, 146u, 145u, 144u, 143u, 142u, 141u, 140u, 139u, 138u, 137u, 136u,
    135u, 134u, 133u, 132u, 131u, 130u, 129u, 128u, 127u, 126u, 125u, 124u,
    123u, 122u, 121u, 120u, 119u, 118u, 117u, 116u, 115u, 114u, 113u, 112u,
    111u, 110u, 109u, 108u, 107u, 106u, 105u, 104u, 103u, 102u, 101u, 100u,
    99u, 98u, 97u, 96u, 95u, 94u, 93u, 92u, 91u, 90u, 89u, 88u, 87u, 86u,
    85u, 84u, 83u, 82u, 81u, 80u, 79u, 78u, 77u, 76u, 75u, 74u, 73u, 72u,
    71u, 70u, 69u, 68u, 67u, 66u, 65u, 64u, 63u, 62u, 61u, 60u, 59u, 58u,
    57u, 56u, 55u, 54u, 53u, 52u, 51u, 50u, 49u, 48u, 47u, 46u, 45u, 44u,
    43u, 42u, 41u, 40u, 39u, 38u, 37u, 36u, 35u, 34u, 33u, 32u, 31u, 30u,
    29u, 28u, 27u, 26u, 25u, 24u, 23u, 22u, 21u, 20u, 19u, 18u, 17u, 16u,
    15u, 14u, 13u, 12u, 11u, 10u, 9u, 8u, 7u, 6u, 5u, 4u, 3u, 2u, 1u, 0u
}};

} // anonymous namespace

struct __attribute__ ((visibility("internal"))) Sapphire::State {

/* Methods: */

    State(NoInitialization const);
    State();
    State(State &&) = delete;
    State(State const &);
    State(std::uint8_t * key, std::size_t keysize);

    ~State() noexcept;

    State & operator=(State &&) = delete;
    State & operator=(State const &) noexcept;

    void defaultInitialize();
    void initialize(std::uint8_t * key, std::size_t keysize);
    std::uint8_t keyrand(unsigned limit,
                         std::uint8_t * user_key,
                         std::size_t keysize,
                         std::uint8_t * rsum,
                         unsigned * keypos);
    std::uint8_t encrypt(std::uint8_t b = 0);
    std::uint8_t decrypt(std::uint8_t b);

/* Fields: */

    std::array<std::uint8_t, 256u> cards; // A permutation of 0-255.
    std::uint8_t rotor; // Index that rotates smoothly
    std::uint8_t ratchet; // Index that moves erratically
    std::uint8_t avalanche; // Index heavily data dependent
    std::uint8_t last_plain; // Last plain text byte
    std::uint8_t last_cipher; // Last cipher text byte

}; /* class Sapphire::State */

Sapphire::State::State(NoInitialization const) {
    // Prevent paging of cryptographic state to swap:
    while (::mlock(this, sizeof(State)) != 0) {
        if (errno != EINTR)
            throw std::bad_alloc();
    }
}

Sapphire::State::State()
    : State(DontInitialize)
{ defaultInitialize(); }

Sapphire::State::State(State const & copy)
    : State(DontInitialize)
{ (*this) = copy; }

Sapphire::State::State(std::uint8_t * key, std::size_t keysize)
    : State(DontInitialize)
{ initialize(key, keysize); }

Sapphire::State::~State() noexcept { std::memset(this, 0, sizeof(*this)); }

Sapphire::State & Sapphire::State::operator=(State const &) noexcept = default;

void Sapphire::State::defaultInitialize() {
    // Start with cards all in inverse order.
    cards = inverse;

    // Initialize the indices and data dependencies.
    rotor = 1u;
    ratchet = 3u;
    avalanche = 5u;
    last_plain = 7u;
    last_cipher = 11u;
}

void Sapphire::State::initialize(std::uint8_t * key, std::size_t keysize) {
    // Key size may be up to 256 bytes.
    // Pass phrases may be used directly, with longer length
    // compensating for the low entropy expected in such keys.
    // Alternatively, shorter keys hashed from a pass phrase or
    // generated randomly may be used. For random keys, lengths
    // of from 4 to 16 bytes are recommended, depending on how
    // secure you want this to be.

    assert(keysize < 256u);

    // If we have been given no key, assume the default hash setup.
    if (!key || (keysize == 0u)) {
        defaultInitialize();
        return;
    }

    // Start with cards all in order, one of each.
    cards = inorder;

    // Swap the card at each position with some other card.
    std::uint8_t toswap = 0u;
    unsigned keypos = 0u; // Start with first byte of user key.
    std::uint8_t rsum = 0u;
    for (unsigned i = 255u;; --i) {
        toswap = keyrand(i, key, keysize, &rsum, &keypos);
        auto const swaptemp = cards[i];
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
}

std::uint8_t Sapphire::State::keyrand(unsigned limit,
                                      std::uint8_t * user_key,
                                      std::size_t keysize,
                                      std::uint8_t * rsum,
                                      unsigned * keypos)
{
    assert(user_key);
    assert(keysize < 256u);
    assert(rsum);
    unsigned u; // Value from 0 to limit to return.
    unsigned retry_limiter; // No infinite loops allowed.
    unsigned mask; // Select just enough bits.

    if (!limit)
        return 0;   // Avoid divide by zero error.
    retry_limiter = 0;
    mask = 1;               // Fill mask with enough bits to cover
    while (mask < limit)    // the desired range.
        mask = (mask << 1) + 1;
    do {
        *rsum = cards[*rsum] + user_key[(*keypos)++];
        if (*keypos >= keysize) {
            *keypos = 0;            // Recycle the user key.
            *rsum += keysize;   // key "aaaa" != key "aaaaaaaa"
        }
        u = mask & *rsum;
        if (++retry_limiter > 11)
            u %= limit;     // Prevent very rare long loops.
    } while (u > limit);
    return static_cast<std::uint8_t>(u);
}

std::uint8_t Sapphire::State::encrypt(std::uint8_t b) {
    // Picture a single enigma rotor with 256 positions, rewired
    // on the fly by card-shuffling.

    // This cipher is a variant of one invented and written
    // by Michael Paul Johnson in November, 1993.

    // Shuffle the deck a little more.
    ratchet += cards[rotor++];
    auto const swaptemp = cards[last_cipher];
    cards[last_cipher] = cards[ratchet];
    cards[ratchet] = cards[last_plain];
    cards[last_plain] = cards[rotor];
    cards[rotor] = swaptemp;
    avalanche += cards[swaptemp];

    // Output one byte from the state in such a way as to make it
    // very hard to figure out which one you are looking at.
    last_cipher = b ^ cards[(cards[ratchet] + cards[rotor]) & 0xFF]
                    ^ cards[cards[(cards[last_plain]
                                   + cards[last_cipher]
                                   + cards[avalanche]) & 0xFF]];
    last_plain = b;
    return last_cipher;
}

std::uint8_t Sapphire::State::decrypt(std::uint8_t b) {
    // Shuffle the deck a little more.
    ratchet += cards[rotor++];
    auto const swaptemp = cards[last_cipher];
    cards[last_cipher] = cards[ratchet];
    cards[ratchet] = cards[last_plain];
    cards[last_plain] = cards[rotor];
    cards[rotor] = swaptemp;
    avalanche += cards[swaptemp];

    // Output one byte from the state in such a way as to make it
    // very hard to figure out which one you are looking at.
    last_plain = b ^ cards[(cards[ratchet] + cards[rotor]) & 0xFF] ^
                     cards[cards[(cards[last_plain]
                                  + cards[last_cipher]
                                  + cards[avalanche]) & 0xFF]];
    last_cipher = b;
    return last_plain;
}

Sapphire::Sapphire(NoInitialization const dontInitialize)
    : m_state(new State(dontInitialize))
{}

Sapphire::Sapphire()
    : m_state(new State())
{}

Sapphire::Sapphire(Sapphire && move) noexcept
    : m_state(move.m_state)
{ move.m_state = nullptr; }

Sapphire::Sapphire(Sapphire const & copy)
    : m_state(new State(*copy.m_state))
{}

Sapphire::Sapphire(std::uint8_t * key, std::size_t keysize)
    : m_state(new State(key, keysize))
{}

Sapphire::~Sapphire() noexcept { delete m_state; }

Sapphire & Sapphire::operator=(Sapphire && move) noexcept {
    delete m_state;
    m_state = move.m_state;
    move.m_state = nullptr;
    return *this;
}

Sapphire & Sapphire::operator=(Sapphire const & copy) noexcept {
    assert(m_state);
    assert(copy.m_state);
    (*m_state) = *(copy.m_state);
    return *this;
}

void Sapphire::defaultInitialize() {
    assert(m_state);
    m_state->defaultInitialize();
}

void Sapphire::initialize(std::uint8_t * key, std::size_t keysize) {
    assert(m_state);
    m_state->initialize(key, keysize);
}

std::uint8_t Sapphire::encrypt(std::uint8_t b) {
    assert(m_state);
    return m_state->encrypt(b);
}

std::uint8_t Sapphire::decrypt(std::uint8_t b) {
    assert(m_state);
    return m_state->decrypt(b);
}

} /* namespace swordxx */
