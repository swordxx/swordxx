/******************************************************************************
 *
 *  gbfheadings.cpp -    SWFilter descendant to hide or show headings
 *            in a GBF module
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
    if (!option) {    // if we don't want headings
        char token[2048]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        bool hide = false;

    const char *from;
    std::string orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++) {
            if (*from == '<') {
                intoken = true;
                tokpos = 0;
//				memset(token, 0, 2048);
                token[0] = 0;
                token[1] = 0;
                token[2] = 0;
                continue;
            }
            if (*from == '>') {	// process tokens
                intoken = false;
                switch (*token) {
                case 'T':				// Reference
                    switch(token[1]) {
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
                    text += '<';
                    for (char *tok = token; *tok; tok++)
                        text += *tok;
                    text += '>';
                }
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
                if (!hide) {
                    text += *from;
                }
            }
        }
    }
    return 0;
}

} /* namespace swordxx */
