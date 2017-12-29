/******************************************************************************
 *
 *  osisredletterwords.cpp -    SWFilter descendant to toggle red coloring for
 *                words of Christ in an OSIS module
 *
 * $Id$
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

#include "osisredletterwords.h"

#include <cstdlib>
#include "../swmodule.h"


namespace swordxx {

namespace {

    static const char oName[] = "Words of Christ in Red";
    static const char oTip[]  = "Toggles Red Coloring for Words of Christ On and Off if they are marked";

}


OSISRedLetterWords::OSISRedLetterWords()
    : OnOffOptionFilter(oName, oTip)
{}


OSISRedLetterWords::~OSISRedLetterWords() {
}


char OSISRedLetterWords::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    if (option) //leave in the red lettered words
        return 0;

    std::string token;
    bool intoken    = false;

    std::string orig = text;
    const char *from = orig.c_str();

    //taken out of the loop
    char const * start = nullptr;
    char const * end = nullptr;

    for (text = ""; *from; from++) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }
        else if (*from == '>') {    // process tokens
            intoken = false;

            if ((token[0] == 'q') && (token[1] == ' ')) { //q tag
                start = std::strstr(token.c_str(), " who=\"Jesus\"");
                if (start && (std::strlen(start) >= 12)) { //we found a quote of Jesus Christ
                    end = start+12; //marks the end of the who attribute value

                    text.push_back('<');
                    text.append(token.c_str(), start - (token.c_str())); //the text before the who attr
                    text.append(end, token.c_str() + token.length() - end);  //text after the who attr
                    text.push_back('>');

                    continue;
                }
            }

            //token not processed, append it. We don't want to alter the text
            text.push_back('<');
            text.append(token);
            text.push_back('>');
            continue;
        }

        if (intoken) { //copy token
            token.push_back(*from);
        }
        else { //copy text which is not inside a token
            text.push_back(*from);
        }
    }
    return 0;
}

} /* namespace swordxx */

