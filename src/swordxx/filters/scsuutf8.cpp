/******************************************************************************
 *
 *  scsuutf8.cpp -    SWFilter descendant to convert a SCSU character to
 *            UTF-8
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

/* This class is based on:
 * http://czyborra.com/scsu/scsu.c written by Roman Czyborra@dds.nl
 *
 * This is a deflator to UTF-8 output for input compressed in SCSU,
 * the (Reuters) Standard Compression Scheme for Unicode as described
 * in http://www.unicode.org/unicode/reports/tr6.html
 */

#include "scsuutf8.h"

#include <string>
#include <unicode/unistr.h>


namespace swordxx {


SCSUUTF8::SCSUUTF8() {
    // initialize SCSU converter
    scsuConv = ucnv_open("SCSU", &err);
    // initialize UTF-8 converter
    utf8Conv = ucnv_open("UTF-8", &err);
}

SCSUUTF8::~SCSUUTF8() {
    ucnv_close(scsuConv);
    ucnv_close(utf8Conv);
}

char SCSUUTF8::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) module;
    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

    // Try decoding with ICU if possible
    err = U_ZERO_ERROR;
    UnicodeString utf16Text(text.c_str(), text.length(), scsuConv, err);
    err = U_ZERO_ERROR;
    int32_t len = utf16Text.extract(&text[0u], text.size(), utf8Conv, err);
    if (len > (int32_t)text.size()+1) {
        text.resize(len + 1, '\0');
        utf16Text.extract(&text[0u], text.size(), utf8Conv, err);
    }
    return 0;
}


} /* namespace swordxx */
