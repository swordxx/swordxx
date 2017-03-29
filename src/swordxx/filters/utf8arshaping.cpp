/******************************************************************************
 *
 *  utf8arshaping.cpp -    SWFilter descendant to perform Arabic shaping on
 *            UTF-8 text
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

#if SWORDXX_HAS_ICU

#include "utf8arshaping.h"

#include <cstdlib>
#include <memory>
#include <string>
#include "../utilstr.h"


namespace swordxx {

UTF8arShaping::UTF8arShaping() : err(U_ZERO_ERROR) {
    conv = ucnv_open("UTF-8", &err);
}

UTF8arShaping::~UTF8arShaping() {
        ucnv_close(conv);
}

char UTF8arShaping::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) module;
     if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

        auto const textSize(text.size());
        auto const ustr(std::make_unique<UChar[]>(textSize));
        auto const ustr2(std::make_unique<UChar[]>(textSize));

    // Convert UTF-8 string to UTF-16 (UChars)
        int32_t len = textSize;
        len = ucnv_toUChars(conv, ustr.get(), len, text.c_str(), -1, &err);

        len = u_shapeArabic(ustr.get(), len, ustr2.get(), len, U_SHAPE_LETTERS_SHAPE | U_SHAPE_DIGITS_EN2AN, &err);

       text.resize(textSize * 2u, '\0');
       len = ucnv_fromUChars(conv, &text[0u], text.size(), ustr2.get(), len, &err);
       text.resize(len, '\0');
    return 0;
}

} /* namespace swordxx */
#endif
