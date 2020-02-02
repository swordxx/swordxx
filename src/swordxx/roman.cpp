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

#include "roman.h"

#include <array>
#include <cassert>
#include "max_v.h"


namespace swordxx {
namespace {

inline unsigned romanDigitValue(char const c) noexcept {
    assert(isRomanDigit(c));
    switch (c) {
    case 'I': case 'i': return 1u;
    case 'V': case 'v': return 5u;
    case 'X': case 'x': return 10u;
    case 'L': case 'l': return 50u;
    case 'C': case 'c': return 100u;
    case 'D': case 'd': return 500u;
    case 'M': case 'm': return 1000u;
    default: return unsigned();
    }
}

} // anonymous namespace

bool isRomanDigit(char const c) noexcept {
    switch (c) {
    case 'M': case 'm':
    case 'D': case 'd':
    case 'C': case 'c':
    case 'L': case 'l':
    case 'X': case 'x':
    case 'V': case 'v':
    case 'I': case 'i': return true;
    default: return false;
    }
}

bool isRoman(std::string_view sv) noexcept {
    for (auto const c : sv)
        if (!isRomanDigit(c))
            return false;
    return true;
}

std::optional<std::uintmax_t> parseRomanNumeral(std::string_view sv) noexcept {
    if (sv.empty())
        return 0u;

    static constexpr auto const maxValue = max_v<std::uintmax_t>;
    static_assert(max_v<std::string_view::size_type> <= maxValue, "");

    using DigitValue = unsigned;

    // Signifies a span of identical Roman numeral digits:
    struct Span {
        std::uintmax_t spanValue; // Number of such Roman digits in the span
        DigitValue digitValue; // Numeric value of the digit
    };

    /* Returns the next span from sv, chopping its prefix on each run. On error,
       sv is left empty, and {0u, 0u} is returned. On end of input, sv is left
       empty and {0u, 0u} is returned. */
    auto const nextSpan =
            [&sv]() -> Span {
                if (sv.empty())
                    return {0u, 0u};
                auto const digit(sv.front());
                if (!isRomanDigit(digit))
                    return {0u, 0u};
                sv.remove_prefix(1u);
                auto const digitValue(romanDigitValue(digit));
                auto spanValueLeft = maxValue - digitValue;
                if (!sv.empty()) {
                    if (sv.front() == digit) {
                        // Multiples of D/L/V are not allowed:
                        switch (digit) {
                        case 'D': case 'L': case 'V':
                        case 'd': case 'l': case 'v':
                            return {0u, 0u};
                        default:
                            break;
                        }
                        do {
                            if (spanValueLeft < digitValue)
                                return {0u, 0u}; // Overflow
                            spanValueLeft -= digitValue;
                            sv.remove_prefix(1u);
                        } while (!sv.empty() && (sv.front() == digit));
                    }
                }
                assert(digitValue < max_v<DigitValue>);
                return {maxValue - spanValueLeft,
                        static_cast<DigitValue>(digitValue)};
            };

    // There can only be a maximum of 7 spans:
    std::array<Span, 7u> allSpans;
    std::size_t numSpans = 0u;
    for (auto & span : allSpans) {
        span = nextSpan();
        if (!span.digitValue) {
            assert(!span.spanValue);
            if (sv.empty())
                break;
            return {}; // Invalid character or spans of D/L/V with size > 1
        }
        assert(span.spanValue);
        ++numSpans;
    }
    if (!numSpans || !sv.empty())
        return {}; // Error on more than 7 spans

    if (numSpans == 1u)
        return allSpans.front().spanValue;

    static constexpr auto const canSubtractDigitValue =
            [](auto const digitValue, auto const fromValue) {
                switch (digitValue) {
                case 5u: case 50u: case 500u:
                    return digitValue * 2u < fromValue;
                default: return true;
                }
            };
    std::uintmax_t r = 0u;
    #define CHECK_OVERFLOW_AND_ADD_TO_RESULT(...) \
        do { \
            auto const toAdd_ = (__VA_ARGS__); \
            if (maxValue - r < toAdd_) \
                return {}; \
            r += toAdd_; \
        } while (false)

    DigitValue maxDigitValue = 1000;
    DigitValue subtractedDigitValue = 0u;
    for (unsigned i = 1u; i < numSpans; ++i) {
        auto const & s1 = allSpans[i - 1u];
        auto const & s2 = allSpans[i];
        if (s1.digitValue < s2.digitValue) {
            if (subtractedDigitValue)
                return {};
            if (s2.digitValue > maxDigitValue)
                return {};
            if (!canSubtractDigitValue(s1.digitValue, s2.digitValue))
                return {};
            if (s1.spanValue >= s2.spanValue)
                return {};
            CHECK_OVERFLOW_AND_ADD_TO_RESULT(s2.spanValue - s1.spanValue);
            subtractedDigitValue = s1.digitValue;
            maxDigitValue = s2.digitValue;
        } else {
            assert(s1.digitValue > s2.digitValue);
            if (subtractedDigitValue) {
                if (s2.digitValue >= subtractedDigitValue)
                    return {};
                if (s2.digitValue > maxDigitValue)
                    return {};
                subtractedDigitValue = 0u;
            } else {
                CHECK_OVERFLOW_AND_ADD_TO_RESULT(s1.spanValue);
                maxDigitValue = s1.digitValue;
            }
        }
    }
    if (!subtractedDigitValue)
        CHECK_OVERFLOW_AND_ADD_TO_RESULT(allSpans[numSpans -1u].spanValue);

    #undef CHECK_OVERFLOW_AND_ADD_TO_RESULT
    return r;
}

std::string toRomanNumeral(std::uintmax_t value) {
    std::size_t stringSize = 0u;
    {
        auto countSpace =
                [v = value, &stringSize](auto const digitValue, auto const size)
                        mutable
                {
                    while (v >= digitValue) {
                        v -= digitValue;
                        stringSize += size;
                    }
                };
        countSpace(1000u, 1u);
        countSpace( 900u, 2u);
        countSpace( 500u, 1u);
        countSpace( 400u, 2u);
        countSpace( 100u, 1u);
        countSpace(  90u, 2u);
        countSpace(  50u, 1u);
        countSpace(  40u, 2u);
        countSpace(  10u, 1u);
        countSpace(   9u, 2u);
        countSpace(   5u, 1u);
        countSpace(   4u, 2u);
        countSpace(   1u, 1u);
    }

    std::string r(stringSize, char());
    {
        auto construct =
                [&value, writePtr = r.data()](auto const digitValue,
                                              auto const ... digitChars) mutable
                {
                    while (value >= digitValue) {
                        value -= digitValue;
                        (((*writePtr++) = digitChars), ...);
                    }
                };
        construct(1000u, 'M');
        construct( 900u, 'C', 'M');
        construct( 500u, 'D');
        construct( 400u, 'C', 'D');
        construct( 100u, 'C');
        construct(  90u, 'X', 'C');
        construct(  50u, 'L');
        construct(  40u, 'X', 'L');
        construct(  10u, 'X');
        construct(   9u, 'I', 'X');
        construct(   5u, 'V');
        construct(   4u, 'I', 'V');
        construct(   1u, 'I');
    }
    return r;
}

} /* namespace swordxx */

