/******************************************************************************
 *
 *  gbfredletterwords.cpp -    SWFilter descendant to toggle red coloring of
 *                words of Christ in a GBF module
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

#include "gbfredletterwords.h"

#include <cstdlib>
#include "../swmodule.h"


namespace swordxx {

namespace {

    static const char oName[] = "Words of Christ in Red";
    static const char oTip[]  = "Toggles Red Coloring for Words of Christ On and Off if they are marked";

}

GBFRedLetterWords::GBFRedLetterWords()
    : OffOnOptionFilter(oName, oTip)
{}

GBFRedLetterWords::~GBFRedLetterWords() {
}


char GBFRedLetterWords::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
/** This function removes the red letter words in Bible like the WEB
* The words are marked by <FR> as start and <Fr> as end tag.
*/
    if (isOptionOff()) {	// if we don't want footnotes
        char token[4096]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        bool hide = false;

        std::string out;
        for (auto const * from = text.c_str(); *from; from++) {
            if (*from == '<') {
                intoken = true;
                tokpos = 0;
//				std::memset(token, 0, 4096);
                token[0] = 0;
                token[1] = 0;
                token[2] = 0;
                continue;
            }
            if (*from == '>') {	// process tokens
                intoken = false;
                /*switch (*token) {
                case 'F':			// Font attribute
                    switch(token[1]) {
                    case 'R':               // Begin red letter words
                        hide = true;
                        break;
                    case 'r':               // end red letter words
                        hide = false;
                        break;
                    }
                    continue;	// skip token
                }*/

                //hide the token if either FR or Fr was detected
                hide = (token[0] == 'F' && ( (token[1] == 'R') || (token[1] == 'r') ));

                // if not a red letter word token, keep token in text
                if (!hide) {
                    out += '<';
                    for (char *tok = token; *tok; tok++)
                        out += *tok;
                    out += '>';
                }
                continue;
            }
            if (intoken) {
                if (tokpos < 4090) {
                    token[tokpos++] = *from;
                    // TODO: why is this + 2 ?  The below comment still doesn't help me understand.  The switch statment
                    // is commented out in this filter
                    token[tokpos+2] = 0;	// +2 cuz we init token with 2 extra '0' because of switch statement
                }
            }
            else {
                out += *from;
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
