/******************************************************************************
 *
 *  utf8nfc.cpp -    SWFilter descendant to perform NFC (canonical
 *            composition normalization) on UTF-8 text
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

#include "utf8nfc.h"

#include <string>
#include <unicode/unistr.h>
#include <unicode/normlzr.h>


namespace swordxx {

UTF8NFC::UTF8NFC() {
        conv = ucnv_open("UTF-8", &err);
}

UTF8NFC::~UTF8NFC() {
         ucnv_close(conv);
}

char UTF8NFC::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) module;
    if ((unsigned long)key < 2)    // hack, we're en(1)/de(0)ciphering
        return -1;

    err = U_ZERO_ERROR;
    UnicodeString source(text.c_str(), text.size(), conv, err);
    UnicodeString target;

    err = U_ZERO_ERROR;
    Normalizer::normalize(source, UNORM_NFC, 0, target, err);

    err = U_ZERO_ERROR;
    text.resize(text.size() * 2u, '\0'); // potentially, it can grow to 2x the original size
    int32_t len = target.extract(&text[0u], text.size(), conv, err);
    text.resize(len, '\0');

    return 0;
}

} /* namespace swordxx */

#endif /* SWORDXX_HAS_ICU */
