/******************************************************************************
 *
 *  osisenum.cpp -    SWFilter descendant to hide or show word enumations
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

#include "osisenum.h"

#include <cstdlib>
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Enumerations";
    static const char oTip[]  = "Toggles Enumerations On and Off if they exist";

}

OSISEnum::OSISEnum()
    : OffOnOptionFilter(oName, oTip)
{}

OSISEnum::~OSISEnum() {
}


char OSISEnum::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;

    using namespace std::literals::string_view_literals;

    if (isOptionOff()) {
        std::string result;
        std::string token;
        bool inToken = false;
        for (auto const & c : text) {
            if (c == '<') {
                inToken = true;
            } else if (c == '>') { // Process token:
                inToken = false;
                if (startsWith(token, "w "sv)) {    // Word
                    XMLTag wtag(token.c_str());
                    if (!wtag.attribute("n").empty()) {
                        wtag.attributes().erase("n");
                        token = wtag.toString();
                        trimString(token);
                        result.append(token);
                    } else { // Keep original token in text:
                        result.push_back('<');
                        result.append(token);
                        result.push_back('>');
                    }
                } else { // Keep original token in text:
                    result.push_back('<');
                    result.append(token);
                    result.push_back('>');
                }
                token.clear();
            } else {
                (inToken ? token : result).push_back(c);
            }
        }
        text = std::move(result);
    }
    return 0;
}

} /* namespace swordxx */
