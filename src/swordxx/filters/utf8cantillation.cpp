/******************************************************************************
 *
 *  utf8cantillation.cpp -    SWFilter descendant to remove UTF-8 Hebrew
 *                cantillation
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

#include "utf8cantillation.h"

#include <cstdio>
#include <cstdlib>


namespace swordxx {

namespace {

    static const char oName[] = "Hebrew Cantillation";
    static const char oTip[]  = "Toggles Hebrew Cantillation Marks";

}

UTF8Cantillation::UTF8Cantillation()
    : OffOnOptionFilter(oName, oTip)
{}

UTF8Cantillation::~UTF8Cantillation(){};


char UTF8Cantillation::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    if (isOptionOff()) {
        //The UTF-8 range 0xD6 0x90 to 0xD6 0xAF and 0xD7 0x84 consist of Hebrew cantillation marks so block those out.
        std::string out;
        for (auto from = reinterpret_cast<unsigned char const *>(text.c_str()); *from; from++) {
            if (*from != 0xD6) {
                if (*from == 0xD7 && *(from + 1) == 0x84) {
                    from++;
                }
                else {
                    out += *from;
                }
            }
            else if (*(from + 1) < 0x90 || *(from + 1) > 0xAF) {
                out += *from;
                from++;
                out += *from;
            }
            else {
                from++;
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
