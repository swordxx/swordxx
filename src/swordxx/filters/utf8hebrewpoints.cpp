/******************************************************************************
 *
 *  utf8hebrewpoints.cpp -    SWFilter descendant to remove UTF-8 Hebrew
 *                vowel points
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

#include "utf8hebrewpoints.h"

#include <cstdio>
#include <cstdlib>


namespace swordxx {

namespace {

    static const char oName[] = "Hebrew Vowel Points";
    static const char oTip[]  = "Toggles Hebrew Vowel Points";

}

UTF8HebrewPoints::UTF8HebrewPoints()
    : OnOffOptionFilter(oName, oTip)
{}

UTF8HebrewPoints::~UTF8HebrewPoints(){};


char UTF8HebrewPoints::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    if (!option) {
        //The UTF-8 range 0xD6 0xB0 to 0xD6 0xBF excluding 0xD6 0xBE consist of Hebrew cantillation marks so block those out.
        std::string orig = text;
        const unsigned char* from = (unsigned char*)orig.c_str();
        for (text = ""; *from; from++) {
            if ((*from == 0xD6) && (*(from + 1) >= 0xB0 && *(from + 1) <= 0xBF) && (*(from + 1) != 0xBE)) {
                from++;
            }
            else {
                         text += *from;
                        }
        }
         }
    return 0;
}


} /* namespace swordxx */
