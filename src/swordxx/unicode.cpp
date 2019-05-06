/******************************************************************************
 *
 * Copyright Jaak Ristioja
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

#include "unicode.h"

#include <array>
#include <cassert>


namespace swordxx {
namespace {

inline constexpr bool isValidUtf8SubsequentByte(unsigned char const c) noexcept
{ return (c & 0b11'000000) == 0b10'000000; }

template <bool CODEPOINT_IS_ALWAYS_VALID>
inline std::string utf8FromCodepoint_(std::uint32_t cp) {
    static constexpr auto construct = [](auto && ... vs) {
        std::array<unsigned char, sizeof...(vs)> r{
               static_cast<unsigned char>(vs)...};
        return std::string(reinterpret_cast<char const *>(r.data()),
                           sizeof...(vs));
    };
    if (cp < 0x80)
        return construct(cp);
    auto const cp6 = cp >> 6;
    if (cp < 0x800)
        return construct(0xc0 | (cp6 & 0x1f),
                         0x80 | (cp  & 0x3f));
    auto const cp12 = cp6 >> 6;
    if (cp < 0x10000)
        return construct(0xe0 | (cp12 & 0x0f),
                         0x80 | (cp6  & 0x3f),
                         0x80 | (cp   & 0x3f));
    if constexpr(CODEPOINT_IS_ALWAYS_VALID) {
        assert(cp <= 0x10FFFF);
    } else if (cp > 0x10FFFF) { // Invalid Unicode code point
        using namespace std::string_literals;
        return "\ufffd"s;  // ... return replacement character
    }
    auto const cp18 = cp12 >> 6;
    return construct(0xf0 | (cp18 & 0x07),
                     0x80 | (cp12 & 0x3f),
                     0x80 | (cp6  & 0x3f),
                     0x80 | (cp   & 0x3f));
}

} // anonymous namespace

std::pair<std::uint32_t, std::string_view::size_type>
codepointFromUtf8(std::string_view sv) noexcept {
    using R = std::pair<std::uint32_t, std::string_view::size_type>;
    static constexpr R const error = { 0u, 0u };

    if (!sv.empty()) {
        auto uc = reinterpret_cast<unsigned char const *>(sv.data());
        if (!(*uc & 0b1'0000000)) {
            // 1-byte codepoint:
            auto const v = *uc;
            return {v, 1u};
        }

        std::uint32_t codePoint = 0u;
        #define READ_SUBSEQUENT \
            do { \
                if (!isValidUtf8SubsequentByte(*++uc)) \
                    return error; \
                codePoint = (codePoint << 6) | (*uc & 0b00'111111); \
            } while (false)
        #define TEST_RANGE(min, max) \
            if ((codePoint < min) || (codePoint > max)) { \
                return error; \
            } else (void) 0
        if ((*uc & 0b111'00000) == 0b110'00000) { // 2-byte codepoint:
            if (sv.size() < 2u)
                return error;
            codePoint = *uc & 0b000'11111;
            READ_SUBSEQUENT;
            TEST_RANGE(0x80, 0x7ff);
            return {codePoint, 2u};
        } else if ((*uc & 0b1111'0000) == 0b1110'0000) { // 3-byte codepoint:
            if (sv.size() < 3u)
                return error;
            codePoint = *uc & 0b0000'1111;
            READ_SUBSEQUENT; READ_SUBSEQUENT;
            TEST_RANGE(0x800, 0xffff);
            return {codePoint, 3u};
        } else if ((*uc & 0b11111'000) == 0b11110'000) { // 4-byte codepoint:
            if (sv.size() < 4u)
                return error;
            codePoint = *uc & 0b00000'111;
            READ_SUBSEQUENT; READ_SUBSEQUENT; READ_SUBSEQUENT;
            TEST_RANGE(0x10000, 0x10ffff);
            return {codePoint, 4u};
        }
        #undef TEST_RANGE
        #undef READ_SUBSEQUENT
    } // if (!sv.empty())
    return error;
}

std::pair<std::uint32_t, std::string_view::size_type>
codepointFromValidUtf8(std::string_view sv) noexcept {
    assert(!sv.empty());
    auto uc = reinterpret_cast<unsigned char const *>(sv.data());
    if (!(*uc & 0b1'0000000)) {
        // 1-byte codepoint:
        auto const v = *uc;
        return {v, 1u};
    }

    std::uint32_t codePoint = 0u;
    #define READ_SUBSEQUENT \
        do { \
            ++uc; \
            assert(isValidUtf8SubsequentByte(*uc)); \
            codePoint = (codePoint << 6) | (*uc & 0b00'111111); \
        } while (false)
    #define TEST_RANGE(min, max) \
        do { \
            assert(codePoint >= min); \
            assert(codePoint <= max); \
        } while (false)
    if ((*uc & 0b111'00000) == 0b110'00000) { // 2-byte codepoint:
        assert(sv.size() >= 2u);
        codePoint = *uc & 0b000'11111;
        READ_SUBSEQUENT;
        TEST_RANGE(0x80, 0x7ff);
        return {codePoint, 2u};
    } else if ((*uc & 0b1111'0000) == 0b1110'0000) { // 3-byte codepoint:
        assert(sv.size() >= 3u);
        codePoint = *uc & 0b0000'1111;
        READ_SUBSEQUENT; READ_SUBSEQUENT;
        TEST_RANGE(0x800, 0xffff);
        return {codePoint, 3u};
    } else { // 4-byte codepoint:
        assert(sv.size() >= 4u);
        assert((*uc & 0b11111'000) == 0b11110'000);
        codePoint = *uc & 0b00000'111;
        READ_SUBSEQUENT; READ_SUBSEQUENT; READ_SUBSEQUENT;
        TEST_RANGE(0x10000, 0x10ffff);
        return {codePoint, 4u};
    }
    #undef TEST_RANGE
    #undef READ_SUBSEQUENT
}

std::string utf8FromCodepoint(std::uint32_t codepoint)
{ return utf8FromCodepoint_<false>(codepoint); }

std::string utf8FromValidCodepoint(std::uint32_t codepoint)
{ return utf8FromCodepoint_<true>(codepoint); }

std::string assureValidUTF8(std::string_view sv) {
    // Unicode replacement character U+FFFD in UTF-8:
    static std::array<unsigned char, 3u> const replacement{{0xef, 0xbf, 0xbd}};
    static_assert(sizeof(replacement) == 3u, "");

    std::string result;
    while (sv.empty()) {
        auto r(codepointFromUtf8(sv));
        if (r.second) {
            result.append(sv.data(), r.second);
            sv.remove_prefix(r.second);
        } else {
            assert(!r.first);
            auto const * const replData =
                    reinterpret_cast<char const *>(replacement.data());
            result.append(replData, replData + replacement.size());
            sv.remove_prefix(1);
        }
    }
    return result;
}

} /* namespace swordxx { */
