/******************************************************************************
 *
 *  osisxlit.cpp -    SWFilter descendant to hide or show xlit values
 *            (encoded transliterations) in an OSIS module
 *
 * $Id$
 *
 * Copyright 2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osisxlit.h"

#include <cstdlib>
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Transliterated Forms";
    static const char oTip[]  = "Toggles transliterated forms On and Off if they exist";

}

OSISXlit::OSISXlit()
    : OffOnOptionFilter(oName, oTip)
{}

OSISXlit::~OSISXlit() {
}


char OSISXlit::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    std::string token;
    bool intoken = false;

    const std::string orig = text;
    const char * from = orig.c_str();

    if (!option) {
        for (text = ""; *from; ++from) {
            if (*from == '<') {
                intoken = true;
                token = "";
                continue;
            }
            if (*from == '>') {    // process tokens
                intoken = false;
                if (hasPrefix(token, "w ")) {    // Word
                    XMLTag wtag(token.c_str());
                    if (!wtag.getAttribute("xlit").empty()) {
                        wtag.setAttribute("xlit", nullptr);
                        token = wtag;
                        trimString(token);
                        // drop <>
                        token.erase(0u, 1u);
                        token.pop_back();
                    }
                }

                // keep token in text
                text.push_back('<');
                text.append(token);
                text.push_back('>');

                continue;
            }
            if (intoken) {
                token += *from;
            }
            else    {
                text.push_back(*from);
            }
        }
    }
    return 0;
}

} /* namespace swordxx */
