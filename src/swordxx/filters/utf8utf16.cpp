/******************************************************************************
 *
 *  utf8utf16.cpp -    SWFilter descendant to convert UTF-8 to UTF-16
 *
 * $Id$
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

#include <cstdint>
#include <string>
#include "../utilstr.h"


namespace swordxx {


UTF8UTF16::UTF8UTF16() {
}


char UTF8UTF16::processText(std::string &text, const SWKey * /*key */, const SWModule * /* module */) {
    const unsigned char *from;
    std::string orig = text;

    from = (const unsigned char *)orig.c_str();

    // -------------------------------
    text = "";
    while (*from) {

        uint32_t ch = getUniCharFromUTF8(&from);

        if (!ch) continue;    // invalid char

        if (ch < 0x10000) {
            auto const oldSize = text.size();
            text.resize(oldSize + 2);
            *((uint16_t *) &text[oldSize]) = (uint16_t)ch;
        }
        else {
            uint16_t utf16;
            utf16 = (int16_t)((ch - 0x10000) / 0x400 + 0xD800);
            auto const oldSize = text.size();
            text.resize(oldSize + 4);
            *((uint16_t *) &text[oldSize]) = utf16;
            utf16 = (int16_t)((ch - 0x10000) % 0x400 + 0xDC00);
            *((uint16_t *) &text[oldSize + 2u]) = utf16;
        }
    }
    return 0;
}

} /* namespace swordxx */
