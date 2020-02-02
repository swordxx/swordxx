/******************************************************************************
 *
 * Copyright 2004-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include SWORDXX_VERSION_H_FILE

#include <cassert>
#include "utilstr.h"


namespace swordxx {
namespace {

std::string const staticSwordCompatVersion(SWORDXX_SWORD_COMPAT_VERSION);
std::string const staticRuntimeVersion(SWORDXX_VERSION_STR);

} // anonymous namespace

Version runtimeVersion() noexcept { return SWORDXX_VERSION; }

std::string const & runtimeVersionStr() noexcept
{ return staticRuntimeVersion; }

bool isValidSwordVersion(char const * s) noexcept {
    for (;; ++s) {
        // Expect digit:
        auto const v = charToDecimal(*s);
        if (v < 0)
            return false;
        ++s;
        // if (v != 0) // this check would not allow leading zeroes like in 1.01
            // Skip numbers:
            while (charToDecimal(*s) >= 0)
                ++s;
        // Expect separator:
        if (!*s)
            return true;
        if (*s != '.')
            return false;
    }
}

int compareVersions(char const * a, char const * b) noexcept {
    assert(isValidSwordVersion(a));
    assert(isValidSwordVersion(b));
    static auto const getComponentLength =
            [](char const * str) noexcept {
                assert(str);
                assert(*str);
                assert(charToDecimal(*str) >= 0);
                std::size_t r = 1u;
                while (charToDecimal(*++str) >= 0)
                    ++r;
                return r;
            };
    static auto const isOnlyZeroesVersion =
            [](char const * str, int const falseReturnValue) noexcept {
                for (;; ++str) {
                    if (*str != '0')
                        return falseReturnValue;
                    if (!*++str)
                        return 0;
                }
            };
    for (;; ++b) { // for each version component
        // skip leading zeroes:
        while (*a == '0')
            ++a;
        if (!charIsDigit(*a))
            --a;
        while (*b == '0')
            ++b;
        if (!charIsDigit(*a))
            --a;

        auto aCompSize = getComponentLength(a);
        auto const bCompSize = getComponentLength(b);
        if (aCompSize != bCompSize)
            return aCompSize < bCompSize ? -1 : 1;
        do { // for each digit in version component
            auto const aValue = charToDecimal(*a);
            auto const bValue = charToDecimal(*b);
            if (aValue != bValue)
                return aValue < bValue ? -1 : 1;
            ++b;
            ++a;
        } while (--aCompSize);
        if (!*a) {
            if (!*b)
                return 0;
            return isOnlyZeroesVersion(++b, -1);
        }
        ++a;
        if (!*b)
            return isOnlyZeroesVersion(a, 1);
    }
}
/// \pre isValidSwordVersion(a) && isValidSwordVersion(b)
bool versionEqual(char const * a, char const * b) noexcept
{ return compareVersions(a, b) == 0; }

/// \pre isValidSwordVersion(a) && isValidSwordVersion(b)
bool versionNotEqual(char const * a, char const * b) noexcept
{ return compareVersions(a, b) != 0; }

/// \pre isValidSwordVersion(a) && isValidSwordVersion(b)
bool versionLess(char const * a, char const * b) noexcept
{ return compareVersions(a, b) < 0; }

/// \pre isValidSwordVersion(a) && isValidSwordVersion(b)
bool versionLessEqual(char const * a, char const * b) noexcept
{ return compareVersions(a, b) <= 0; }

/// \pre isValidSwordVersion(a) && isValidSwordVersion(b)
bool versionGreater(char const * a, char const * b) noexcept
{ return compareVersions(a, b) > 0; }

/// \pre isValidSwordVersion(a) && isValidSwordVersion(b)
bool versionGreaterEqual(char const * a, char const * b) noexcept
{ return compareVersions(a, b) >= 0; }

bool isCompatibleWithSwordVersion(char const * in) noexcept {
    if (!isValidSwordVersion(in))
        return false;
    auto const myVersion = staticSwordCompatVersion.c_str();
    assert(isValidSwordVersion(myVersion));
    return versionLessEqual(in, myVersion);
}

} /* namespace swordxx */
