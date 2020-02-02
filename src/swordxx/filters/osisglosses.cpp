/******************************************************************************
 *
 *  osisglosses.cpp -    SWFilter descendant to hide or show glosses
 *            in an OSIS module
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

#include "osisglosses.h"

#include <cstdlib>
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Glosses";
    static const char oTip[]  = "Toggles Glosses On and Off if they exist";

}

OSISGlosses::OSISGlosses()
    : OffOnOptionFilter(oName, oTip)
{}

OSISGlosses::~OSISGlosses() {
}


char OSISGlosses::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    std::string token;
    bool intoken = false;


    using namespace std::literals::string_view_literals;

    if (isOptionOff()) {
        std::string out;
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
                    if (!wtag.attribute("gloss").empty()) {
                        wtag.attributes().erase("gloss");
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
                token.push_back(*from);
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
