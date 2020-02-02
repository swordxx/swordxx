/******************************************************************************
 *
 *  swld.cpp -    code for base class 'SWLD'.  SWLD is the basis for all
 *        types of Lexicon and Dictionary modules (hence the 'LD').
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swld.h"

#include <cstdio>
#include <cstring>
#include "keys/strkey.h"
#include "swkey.h"
#include "utilstr.h"


namespace swordxx {


/******************************************************************************
 * SWLD Constructor - Initializes data for instance of SWLD
 *
 * ENT:    imodname - Internal name for module
 *    imoddesc - Name to display to user for module
 */

SWLD::SWLD(const char * imodname,
           const char * imoddesc,
           TextEncoding enc,
           SWTextDirection dir,
           SWTextMarkup mark,
           const char * ilang,
           bool strongsPadding)
    : SWModule(createKey(),
               imodname,
               imoddesc,
               "Lexicons / Dictionaries",
               enc,
               dir,
               mark,
               ilang)
    , m_strongsPadding(strongsPadding)
{}

std::shared_ptr<SWKey> SWLD::createKey() const
{ return std::make_shared<StrKey>(); }


/******************************************************************************
 * SWLD::KeyText - Sets/gets module KeyText, getting from saved text if key is
 *                persistent
 *
 * ENT:    ikeytext - value which to set keytext
 *        [0] - only get
 *
 * RET:    pointer to keytext
 */

std::string SWLD::getKeyText() const {
    getRawEntry();    // force module key to snap to entry
    return m_entkeytxt;
}

void SWLD::positionToTop() {
    if (!getKey()->isTraversable()) {
        getKey()->setText("");
    } else {
        getKey()->positionToTop();
    }
    getRawEntry();
}

void SWLD::positionToBottom() {
    if (!getKey()->isTraversable()) {
        getKey()->setText("zzzzzzzzz");
    } else {
        getKey()->positionToBottom();
    }
    getRawEntry();
}

bool SWLD::hasEntry(SWKey const & key_) const {
    auto const keyText = key_.getText();
    if (m_strongsPadding) {
        auto const buf(strongsPadBuf(keyText.c_str()));
        return buf.get() == getKeyForEntry(getEntryForKey(buf.get()));
    } else {
        return keyText == getKeyForEntry(getEntryForKey(keyText.c_str()));
    }
}


/******************************************************************************
 * SWLD::strongsPad    - Pads a key if (it-1) is 100% digits to 5 places
 *                        allows for final to be alpha, e.g. '123B'
 *
 * ENT: buf -    buffer to check and pad
 */
std::unique_ptr<char[]> SWLD::strongsPadBuf(char const * inText) {
    std::size_t len = std::strlen(inText);
    auto r(std::make_unique<char[]>(len + 6u));
    std::strcpy(r.get(), inText);
    char * buf = r.get();
    if ((len <= 0) || (len >= 9))
        return r;

    // Handle initial G or H
    bool prefix;
    switch (*buf) {
    case 'G': case 'H': case 'g': case 'h':
        ++buf;
        --len;
        prefix = true;
        break;
    default:
        prefix = false;
        break;
    }

    static auto const isDigit = [](char const c) {
        switch (c) {
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            return true;
        default:
            return false;
        }
    };
    std::size_t size = 0u;
    char * check = buf;
    for (; isDigit(*check); ++check)
        ++size;

    if (size && ((size == len) || (size == len - 1) || (size == (len - 2)))) {
        bool bang;
        if (*check == '!') {
            bang = true;
            ++check;
        } else {
            bang = false;
        }
        char subLet = 0;
        if (charIsAlpha(*check)) {
            subLet = std::toupper(*check);
            *(check - (bang ? 1 : 0)) = '\0';
        }
        std::sprintf(buf, (prefix ? "%.4d" : "%.5d"), std::atoi(buf));
        if (subLet) {
            check = buf + std::strlen(buf);
            if (bang)
                *check++ = '!';
            *check++ = subLet;
            *check = '\0';
        }
    }
    return r;
}

} /* namespace swordxx */

