/******************************************************************************
 *
 *  gbfheadings.cpp -    SWFilter descendant to hide or show headings
 *            in a GBF module
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

#include "gbfheadings.h"

#include <cstdlib>


namespace swordxx {

namespace {

    static const char oName[] = "Headings";
    static const char oTip[]  = "Toggles Headings On and Off if they exist";

}

GBFHeadings::GBFHeadings()
    : OffOnOptionFilter(oName, oTip)
{}

GBFHeadings::~GBFHeadings() {
}


char GBFHeadings::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    if (isOptionOff()) {    // if we don't want headings
        std::string lastToken;
        bool intoken = false;
        bool hide = false;

        std::string out;
        for (const char *from = text.c_str(); *from; from++) {
            if (*from == '<') {
                intoken = true;
                lastToken.clear();
                continue;
            }
            if (*from == '>') {	// process tokens
                intoken = false;
                if (lastToken.c_str()[0u] == 'T') {
                    switch(lastToken.c_str()[1u]) {
                    case 'S':               // Begin heading
                        hide = true;
                        break;
                    case 's':               // end heading
                        hide = false;
                        break;
                    }
                    continue;	// skip token
                }
                // if not a heading token, keep token in text
                if (!hide) {
                    out += '<';
                    out += lastToken;
                    out += '>';
                }
                continue;
            }
            if (intoken) {
                lastToken += *from;
            }
            else {
                if (!hide) {
                    out += *from;
                }
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
