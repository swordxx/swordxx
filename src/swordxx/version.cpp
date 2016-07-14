/******************************************************************************
 *
 *  url.cpp -    code for an URL parser utility class
 *
 * $Id$
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

#include <version.h>

#include <array>
#include <cstdlib>
#include <ios>
#include <sstream>


namespace swordxx {

std::mutex detail::VersionStaticInit::mutex;
namespace {
std::mutex runtimeVersionMutex;
std::mutex swordCompatVersionMutex;
constexpr auto const versionDelimiter = '.';
}

std::string versionToString(Version v) {
    std::ostringstream oss;
    Version const verPatch(v % 65536u);
    v = v / 65536u;
    Version const verMinor(v % 65536u);
    Version const verMajor(v / 65536u);
    oss << std::hex << verMajor;
    if (verMinor || verPatch)
        oss << versionDelimiter << std::hex << verMinor;
    if (verPatch)
        oss << versionDelimiter << std::hex << verPatch;
    return oss.str();
}

Version runtimeVersion() noexcept { return SWORDXX_VERSION; }

std::string runtimeVersionStr() noexcept {
    std::lock_guard<std::mutex> const guard(runtimeVersionMutex);
    static std::string const staticStr(SWORDXX_VERSION_STR);
    return staticStr;
}

Version swordCompatVersion() noexcept { return SWORDXX_SWORD_COMPAT_VERSION; }

std::string swordCompatVersionStr() noexcept {
    std::lock_guard<std::mutex> const guard(swordCompatVersionMutex);
    static std::string const staticStr(SWORDXX_SWORD_COMPAT_VERSION_STR);
    return staticStr;
}

std::pair<bool, Version> parseVersion(char const * c) noexcept {
    using R = std::pair<bool, Version>;
    constexpr static auto const failure = R(false, Version());
    assert(c);
    enum {
        FIRSTNUM,
        NUM_DELIM_OR_END,
        DELIM_OR_END
    } state = FIRSTNUM;
    std::array<unsigned, 3u> components{ 0u, 0u, 0u };
    auto component(components.begin());
    unsigned digits = 0u;
    auto const success =
            [&components]() noexcept {
                Version v(components[0u]);
                v = (v * 256u) + components[1u];
                v = (v * 256u) + components[2u];
                return R(true, v);
            };
    static auto const charToDigit =
            [](char const d) noexcept {
                switch (d) {
                    case '1': case '2': case '3': case '4': case '5':
                    case '6': case '7': case '8': case '9':
                        return static_cast<unsigned>(d - '0');
                    case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                        return static_cast<unsigned>(10 + (d - 'a'));
                    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                        return static_cast<unsigned>(10 + (d - 'A'));
                }
                std::abort();
            };
    for (;; ++c) {
        switch (*c) {
            case '\0':
                if (state == FIRSTNUM)
                    return failure;
                return success();
            case '0':
                switch (state) {
                    case FIRSTNUM:
                        assert(*component == 0u);
                        state = DELIM_OR_END;
                        continue;
                    case DELIM_OR_END:
                        return failure;
                    case NUM_DELIM_OR_END:
                        if (digits == 2u)
                            return failure;
                        ++digits;
                        (*component) *= 16u;
                        continue;
                }
            case '1': case '2': case '3': case '4': case '5':
            case '6': case '7': case '8': case '9':
            case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                switch (state) {
                    case FIRSTNUM:
                        assert(*component == 0u);
                        state = NUM_DELIM_OR_END;
                        (*component) = charToDigit(*c);
                        ++digits;
                        continue;
                    case DELIM_OR_END:
                        return failure;
                    case NUM_DELIM_OR_END:
                        if (digits == 2u)
                            return failure;
                        ++digits;
                        (*component) = ((*component) * 16u) + charToDigit(*c);
                        continue;
                }
            case versionDelimiter:
                if (state == FIRSTNUM)
                    return failure;
                digits = 0u;
                if (++component == components.end())
                    return failure;
                assert(*component == 0u);
                state = FIRSTNUM;
                continue;
            default:
                return failure;
        }
    }
}

/// \todo Refactor these tests:
#if 0
#define testFailure(str) \
    do { \
        auto r(parseVersion((str))); \
        assert(!r.first && (str)); \
    } while (false)
#define testSuccess(str, value) \
    do { \
        auto r(parseVersion((str))); \
        assert(r.first && (str)); \
        assert((r.second == (value)) && (str)); \
    } while (false)

void test() {
    testFailure("");
    testFailure("x");
    testFailure("xy");
    testFailure(".");
    testFailure("0.");
    testFailure("1.");
    testFailure(".0");
    testFailure(".1");
    testFailure("00");
    testFailure("01");
    testFailure("1.00");
    testFailure("00.1");
    testFailure("00.1.2");
    testFailure("01.2");
    testFailure("1.02");
    testFailure("1.00.2");
    testFailure("1.2.00");
    testFailure("1..1");
    testFailure("10000");
    testFailure("1.00");
    testFailure("00.1");
    testFailure("00.00");
    testFailure(".1.1.1");
    testFailure("1.1.1.");
    testFailure("1..1.1");
    testFailure("1.1..1");
    testFailure("0.0.0.0");
    testFailure("1.1.1.1");
    testFailure("111.11.11");
    testFailure("11.111.11");
    testFailure("11.11.111");
    testSuccess("0", 0x0u);
    testSuccess("0.0.0", 0x0u);
    testSuccess("1", 0x00010000u);
    testSuccess("1.1", 0x00010100u);
    testSuccess("1.1.1", 0x00010101u);
    testSuccess("0.1.1", 0x00000101u);
    testSuccess("0.0.1", 0x00000001u);
    testSuccess("1.1.fF", 0x000101ffu);
    testSuccess("1.fF.1", 0x0001ff01u);
    testSuccess("fF.1.1", 0x00ff0101u);
    testSuccess("12.34.56", 0x00123456u);
    testSuccess("78.9", 0x00780900u);
    testSuccess("ab.cd.ef", 0x00abcdefu);
    testSuccess("AB.CD.EF", 0x00abcdefu);
}

int x = (test(), 42);
#endif

} /* namespace swordxx */
