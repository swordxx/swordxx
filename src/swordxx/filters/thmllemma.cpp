/******************************************************************************
 *
 *  thmllemma.cpp -    SWFilter descendant to hide or show lemmas
 *            in a ThML module
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

#include "thmllemma.h"

#include <cstdlib>
#include <cstring>
#include "../utilstr.h"


namespace swordxx {

namespace {

    static const char oName[] = "Lemmas";
    static const char oTip[]  = "Toggles Lemmas On and Off if they exist";

}

ThMLLemma::ThMLLemma()
    : OffOnOptionFilter(oName, oTip)
{}

ThMLLemma::~ThMLLemma() {
}


char ThMLLemma::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    (void) key;
    (void) module;
    if (isOptionOff()) {    // if we don't want lemmas
        bool intoken = false;

        std::string token;
        std::string out;
        for (auto const * from = text.c_str(); *from; from++) {
            if (*from == '<') {
                intoken = true;
                token = "";
                continue;
            }
            if (*from == '>') {    // process tokens
                intoken = false;
                if (startsWith(token, "sync "sv) && std::strstr(token.c_str(), "type=\"lemma\"")) {    // Lemma
                  continue;
                }

                // if not a lemma token, keep token in text
                out += '<';
                out += token;
                out += '>';
                continue;
            }

            if (intoken) {
                token += *from;
            }
            else    {
                out += *from;
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
