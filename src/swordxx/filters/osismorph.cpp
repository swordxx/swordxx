/******************************************************************************
 *
 *  osismorph.cpp -    SWFilter descendant to hide or show morph tags
 *            in a OSIS module
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osismorph.h"

#include <cstdlib>
#include <cstring>


namespace swordxx {

namespace {

    static const char oName[] = "Morphological Tags";
    static const char oTip[]  = "Toggles Morphological Tags On and Off if they exist";

}

OSISMorph::OSISMorph()
    : OffOnOptionFilter(oName, oTip)
{}

OSISMorph::~OSISMorph() {
}


char OSISMorph::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    if (isOptionOff()) {	// if we don't want morph tags
        char token[2048]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        std::string out;

        //taken out of the loop for speed
        char const * start = nullptr;
        char const * end = nullptr;

        for (auto const * from = text.c_str(); *from; ++from) {
            if (*from == '<') {
                intoken = true;
                tokpos = 0;
                token[0] = 0;
                continue;
            }
            if (*from == '>') {	// process tokens
                intoken = false;

                if ((*token == 'w') && (token[1] == ' ')) {
                    start = std::strstr(token+2, "morph=\""); //we leave out "w " at the start
                    end = start ? std::strchr(start + 7, '"') : nullptr; //search the end of the morph value

                    if (start && end) { //start and end of the morph tag found
                        out.push_back('<');
                        out.append(token, start-token); //the text before the morph attr
                        out.append(end+1); //text after the morph attr
                        out.push_back('>');

                        continue; //next loop
                    }
                }

                out.push_back('<');
                out.append(token);
                out.push_back('>');

                continue;
            }
            if (intoken) {
                if (tokpos < 2045) {
                    token[tokpos++] = *from;
                    token[tokpos] = 0;
                }
            }
            else {
                out.push_back(*from);
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
