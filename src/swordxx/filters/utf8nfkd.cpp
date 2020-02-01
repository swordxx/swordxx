/******************************************************************************
 *
 *  utf8nfkd.cpp -    SWFilter descendant to perform NFKD (compatability
 *            decomposition normalization) on UTF-8 text
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

#include "utf8nfkd.h"

#include <memory>
#include <string>
#include <unicode/utypes.h>
#include <unicode/ucnv.h>
#include <unicode/uchar.h>
#include <unicode/ustring.h>
#include <unicode/unorm2.h>


namespace swordxx {

struct UTF8NFKDPrivate {
    UNormalizer2 const * conv;
};

UTF8NFKD::UTF8NFKD() {
    UErrorCode err = U_ZERO_ERROR;
    p = new struct UTF8NFKDPrivate;
    p->conv = unorm2_getNFKDInstance(&err);
}


UTF8NFKD::~UTF8NFKD() {
    delete p;
}


char UTF8NFKD::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) module;

    UErrorCode err = U_ZERO_ERROR;

    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

    int32_t len =  5 + text.length() * 5;
        auto const source(std::make_unique<UChar[]>(len + 1)); //each char could become a surrogate pair

    // Convert UTF-8 string to UTF-16 (UChars)
        int32_t ulen;
        u_strFromUTF8(source.get(), len, &ulen, text.c_str(), (int32_t)text.size(), &err);

        auto const target(std::make_unique<UChar[]>(len + 1));

        //compatability decomposition
        ulen = unorm2_normalize(p->conv, source.get(), ulen, target.get(), len, &err);

       text.resize(len, '\0');
       u_strToUTF8(text.data(), len, &len, target.get(), ulen, &err);
       text.resize(len, '\0');

    return 0;
}

} /* namespace swordxx */
