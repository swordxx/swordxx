/******************************************************************************
 *
 *  osisxlit.cpp -    SWFilter descendant to hide or show xlit values
 *            (encoded transliterations) in an OSIS module
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

    std::string out;

    using namespace std::literals::string_view_literals;

    if (isOptionOff()) {
        for (auto const * from = text.c_str(); *from; ++from) {
            if (*from == '<') {
                intoken = true;
                token = "";
                continue;
            }
            if (*from == '>') {    // process tokens
                intoken = false;
                if (startsWith(token, "w "sv)) {    // Word
                    XMLTag wtag(token.c_str());
                    if (!wtag.attribute("xlit").empty()) {
                        wtag.attributes().erase("xlit");
                        token = wtag.toString();
                        trimString(token);
                        // drop <>
                        token.erase(0u, 1u);
                        token.pop_back();
                    }
                }

                // keep token in text
                out.push_back('<');
                out.append(token);
                out.push_back('>');

                continue;
            }
            if (intoken) {
                token += *from;
            }
            else    {
                out.push_back(*from);
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
