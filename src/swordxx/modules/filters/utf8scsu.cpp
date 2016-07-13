/******************************************************************************
 *
 *  utf8scsu.cpp -    SWFilter descendant to convert UTF-8 to SCSU
 *
 * $Id$
 *
 * Copyright 2001-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#ifdef _ICU_

#include "utf8scsu.h"

#include <string>

namespace swordxx {


UTF8SCSU::UTF8SCSU() {
    // initialize SCSU converter
    scsuConv = ucnv_open("SCSU", &err);

    // initialize UTF-8 converter
    utf8Conv = ucnv_open("UTF-8", &err);
}

UTF8SCSU::~UTF8SCSU() {
         ucnv_close(scsuConv);
         ucnv_close(utf8Conv);
}

char UTF8SCSU::processText(std::string &text, const SWKey *key, const SWModule *module) {
    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

    err = U_ZERO_ERROR;
    UnicodeString utf16Text(text.c_str(), text.length(), utf8Conv, err);
    err = U_ZERO_ERROR;
    int32_t len = utf16Text.extract(&text[0u], text.size(), scsuConv, err);
    if (len > (int32_t)text.size()+1) {
        text.resize(len + 1, '\0');
        utf16Text.extract(&text[0u], text.size(), scsuConv, err);
    }

    return 0;
}

} /* namespace swordxx */

#endif
