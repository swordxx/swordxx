/******************************************************************************
 *
 *  gbfmorph.cpp -    SWFilter descendant to hide or show morph tags
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

#include "gbfmorph.h"

#include <cstdlib>


namespace swordxx {

namespace {

    static const char oName[] = "Morphological Tags";
    static const char oTip[]  = "Toggles Morphological Tags On and Off if they exist";

}

GBFMorph::GBFMorph()
    : OffOnOptionFilter(oName, oTip)
{}

GBFMorph::~GBFMorph() {
}


char GBFMorph::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    if (isOptionOff()) {    // if we don't want morph tags
        char token[2048]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        bool lastspace = false;

        std::string out;

        for (auto const * from = text.c_str(); *from; from++) {
            if (*from == '<') {
                intoken = true;
                tokpos = 0;
                token[0] = 0;
                token[1] = 0;
                token[2] = 0;
                continue;
            }
            if (*from == '>') {	// process tokens
                intoken = false;
                if (*token == 'W' && token[1] == 'T') {	// Morph
                  if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
                    if (lastspace)
                      out.pop_back();
                  }
                  continue;
                }
                // if not a morph tag token, keep token in text
                out += '<';
                out += token;
                out += '>';
                continue;
            }
            if (intoken) {
                if (tokpos < 2045) {
                    token[tokpos++] = *from;
                    // TODO: why is this + 2 ?
                    token[tokpos+2] = 0;
                }
            }
            else {
                out += *from;
                lastspace = (*from == ' ');
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
