/******************************************************************************
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWORDXX_SYSDATA_H
#define SWORDXX_SYSDATA_H

#include <cstdint>

#undef swswap16
#undef swswap32
#undef swswap64

#define swswap16(x) \
    ((uint16_t)( \
        (((uint16_t)(x) & (uint16_t)0x00ffU) << 8) | \
        (((uint16_t)(x) & (uint16_t)0xff00U) >> 8) ))


#define swswap32(x) \
    ((uint32_t)( \
        (((uint32_t)(x) & (uint32_t)0x000000ffUL) << 24) | \
        (((uint32_t)(x) & (uint32_t)0x0000ff00UL) <<  8) | \
        (((uint32_t)(x) & (uint32_t)0x00ff0000UL) >>  8) | \
        (((uint32_t)(x) & (uint32_t)0xff000000UL) >> 24) ))


#define swswap64(x) \
    ((uint64_t)( \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000000000ffULL) << 56) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000000000ff00ULL) << 40) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000000000ff0000ULL) << 24) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00000000ff000000ULL) <<  8) | \
           (uint64_t)(((uint64_t)(x) & (uint64_t)0x000000ff00000000ULL) >>  8) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x0000ff0000000000ULL) >> 24) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0x00ff000000000000ULL) >> 40) | \
        (uint64_t)(((uint64_t)(x) & (uint64_t)0xff00000000000000ULL) >> 56) ))




#ifndef WORDS_BIGENDIAN

#define swordtoarch16(x) (x)
#define swordtoarch32(x) (x)
#define swordtoarch64(x) (x)
#define archtosword16(x) (x)
#define archtosword32(x) (x)
#define archtosword64(x) (x)

#else

#define swordtoarch16(x) swswap16(x)
#define swordtoarch32(x) swswap32(x)
#define swordtoarch64(x) swswap64(x)
#define archtosword16(x) swswap16(x)
#define archtosword32(x) swswap32(x)
#define archtosword64(x) swswap64(x)


#endif

namespace swordxx {

constexpr int16_t swapToArch(int16_t v) noexcept { return swordtoarch16(v); }
constexpr int32_t swapToArch(int32_t v) noexcept { return swordtoarch32(v); }
constexpr int64_t swapToArch(int64_t v) noexcept { return swordtoarch64(v); }
constexpr uint16_t swapToArch(uint16_t v) noexcept { return swordtoarch16(v); }
constexpr uint32_t swapToArch(uint32_t v) noexcept { return swordtoarch32(v); }
constexpr uint64_t swapToArch(uint64_t v) noexcept { return swordtoarch64(v); }

constexpr int16_t swapFromArch(int16_t v) noexcept { return archtosword16(v); }
constexpr int32_t swapFromArch(int32_t v) noexcept { return archtosword32(v); }
constexpr int64_t swapFromArch(int64_t v) noexcept { return archtosword64(v); }
constexpr uint16_t swapFromArch(uint16_t v) noexcept { return archtosword16(v); }
constexpr uint32_t swapFromArch(uint32_t v) noexcept { return archtosword32(v); }
constexpr uint64_t swapFromArch(uint64_t v) noexcept { return archtosword64(v); }

} /* namespace swordxx */

#endif /* SWORDXX_SYSDATA_H */
