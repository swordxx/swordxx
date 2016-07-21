/******************************************************************************
 *
 *  thmllemma.cpp -    SWFilter descendant to hide or show lemmas
 *            in a ThML module
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

#include "thmllemma.h"

#include <cstdlib>
#include <cstring>


namespace swordxx {

namespace {

    static const char oName[] = "Lemmas";
    static const char oTip[]  = "Toggles Lemmas On and Off if they exist";

    static const StringList *oValues() {
        static const std::string choices[3] = {"Off", "On", ""};
        static const StringList oVals(&choices[0], &choices[2]);
        return &oVals;
    }
}


ThMLLemma::ThMLLemma() : SWOptionFilter(oName, oTip, oValues()) {
}


ThMLLemma::~ThMLLemma() {
}


char ThMLLemma::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    if (!option) {    // if we don't want lemmas
        bool intoken = false;

        std::string token;
        std::string orig = text;
        const char *from = orig.c_str();
        for (text = ""; *from; from++) {
            if (*from == '<') {
                intoken = true;
                token = "";
                continue;
            }
            if (*from == '>') {    // process tokens
                intoken = false;
                if (!strncmp(token.c_str(), "sync ", 5) && strstr(token.c_str(), "type=\"lemma\"")) {    // Lemma
                  continue;
                }

                // if not a lemma token, keep token in text
                text += '<';
                text += token;
                text += '>';
                continue;
            }

            if (intoken) {
                token += *from;
            }
            else    {
                text += *from;
            }
        }
    }
    return 0;
}

} /* namespace swordxx */
