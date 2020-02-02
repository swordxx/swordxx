/******************************************************************************
 *
 *  utf8utf16.cpp -    SWFilter descendant to convert UTF-8 to UTF-16
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

#include "utf8utf16.h"

#include <cassert>
#include <cstdint>
#include <string>
#include "../unicode.h"


namespace swordxx {


UTF8UTF16::UTF8UTF16() {
}


char UTF8UTF16::processText(std::string &text, const SWKey * /*key */, const SWModule * /* module */) {
    std::string result;
    std::string_view sv(text);
    while (!sv.empty()) {
        auto r(codepointFromUtf8(sv));
        if (r.second) {
            sv.remove_prefix(r.second);
            if (r.first < 0x10000) {
                std::uint16_t const ch = static_cast<std::uint16_t>(r.first);
                result.append(reinterpret_cast<char const *>(&ch), sizeof(ch));
            } else {
                r.first -= 0x10000;
                struct __attribute__((packed)) Surrogates {
                    std::uint16_t high;
                    std::uint16_t low;
                } ss;
                static_assert(sizeof(ss) == 2 * sizeof(std::uint16_t), "");
                ss.high = static_cast<std::uint16_t>((r.first >> 10) + 0xd800);
                ss.low = (r.first & 0b11'1111'1111) + 0xdc00;
                result.append(reinterpret_cast<char const *>(&ss), sizeof(ss));
            }
        } else {
            assert(!r.first);
            sv.remove_prefix(1u);
            static std::uint16_t const replacementChar = 0xfffd;
            result.append(reinterpret_cast<char const *>(&replacementChar),
                          sizeof(replacementChar));
        }
    }
    text = std::move(result);
    return 0;
}

} /* namespace swordxx */
