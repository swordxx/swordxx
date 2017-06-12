/******************************************************************************
 *
 *  osisscripref.cpp -    SWFilter descendant to hide or show scripture
 *            references in an OSIS module
 *
 * $Id$
 *
 * Copyright 2003-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osisscripref.h"

#include <cstdlib>
#include "../swmodule.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Cross-references";
    static const char oTip[]  = "Toggles Scripture Cross-references On and Off if they exist";

}

OSISScripref::OSISScripref()
    : OffOnOptionFilter(oName, oTip)
{}

OSISScripref::~OSISScripref() {
}


char OSISScripref::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    std::string token;
    bool intoken    = false;
    bool hide       = false;
    std::string tagText;
    XMLTag startTag;

    std::string orig = text;
    const char *from = orig.c_str();

    XMLTag tag;

    for (text = ""; *from; ++from) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }
        if (*from == '>') {    // process tokens
            intoken = false;

            tag = token.c_str();

            if (!strncmp(token.c_str(), "note", 4) || !strncmp(token.c_str(), "/note", 5)) {
                if (!tag.isEndTag() && !tag.isEmpty()) {
                    startTag = tag;
                    if (tag.attribute("type") == "crossReference") {
                        hide = true;
                        tagText = "";
                        if (option) {    // we want the tag in the text
                            text.push_back('<');
                            text.append(token);
                            text.push_back('>');
                        }
                        continue;
                    }
                }
                if (hide && tag.isEndTag()) {
                    hide = false;
                    if (option) {    // we want the tag in the text
                        text.append(tagText);  // end tag gets added further down
                    }
                    else    continue;    // don't let the end tag get added to the text
                }
            }

            // if not a heading token, keep token in text
            if (!hide) {
                text.push_back('<');
                text.append(token);
                text.push_back('>');
            }
            else {
                tagText.push_back('<');
                tagText.append(token);
                tagText.push_back('>');
            }
            continue;
        }
        if (intoken) { //copy token
            token.push_back(*from);
        }
        else if (!hide) { //copy text which is not inside a token
            text.push_back(*from);
        }
        else tagText.push_back(*from);
    }
    return 0;
}

} /* namespace swordxx */
