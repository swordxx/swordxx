/******************************************************************************
 *
 *  utf8cantillation.cpp -    SWFilter descendant to remove UTF-8 Hebrew
 *                cantillation
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
    if (!option) {
        //The UTF-8 range 0xD6 0x90 to 0xD6 0xAF and 0xD7 0x84 consist of Hebrew cantillation marks so block those out.
        std::string orig = text;
        const unsigned char* from = (unsigned char*)orig.c_str();
        for (text = ""; *from; from++) {
            if (*from != 0xD6) {
                if (*from == 0xD7 && *(from + 1) == 0x84) {
                    from++;
                }
                else {
                    text += *from;
                }
            }
            else if (*(from + 1) < 0x90 || *(from + 1) > 0xAF) {
                text += *from;
                from++;
                text += *from;
            }
            else {
                from++;
            }
        }
    }
    return 0;
}

} /* namespace swordxx */
