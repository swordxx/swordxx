/******************************************************************************
 *
 *  utf8bidireorder.cpp -    SWFilter descendant to perform reordering of
 *                UTF-8 text to visual order according to the
 *                Unicode Bidirectional Algorithm (UBA)
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

#include "utf8bidireorder.h"

#include <cstdlib>
#include <string>
#include "../utilstr.h"


namespace swordxx {


UTF8BiDiReorder::UTF8BiDiReorder() : err(U_ZERO_ERROR) {

        conv = ucnv_open("UTF-8", &err);

}


UTF8BiDiReorder::~UTF8BiDiReorder() {
        ucnv_close(conv);
}


char UTF8BiDiReorder::processText(std::string &text, const SWKey *key, const SWModule *module)
{
        UChar *ustr, *ustr2;
    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

        int32_t len = text.length();
        ustr = new UChar[len]; //each char could become a surrogate pair

    // Convert UTF-8 string to UTF-16 (UChars)
        len = ucnv_toUChars(conv, ustr, len, text.c_str(), -1, &err);
        ustr2 = new UChar[len];

        UBiDi* bidi = ubidi_openSized(len + 1, 0, &err);
        ubidi_setPara(bidi, ustr, len, UBIDI_DEFAULT_RTL, NULL, &err);
        len = ubidi_writeReordered(bidi, ustr2, len,
                UBIDI_DO_MIRRORING | UBIDI_REMOVE_BIDI_CONTROLS, &err);
        ubidi_close(bidi);

//        len = ubidi_writeReverse(ustr, len, ustr2, len,
//                UBIDI_DO_MIRRORING | UBIDI_REMOVE_BIDI_CONTROLS, &err);

       text.resize(text.size() * 2u, '\0');
       len = ucnv_fromUChars(conv, &text[0u], text.size(), ustr2, len, &err);
       text.resize(len, '\0');

        delete [] ustr2;
        delete [] ustr;
    return 0;
}

} /* namespace swordxx */

#endif /* SWORDXX_HAS_ICU */
