/******************************************************************************
 *
 *  osisreferencelinks.cpp -    SWFilter descendant to toggle OSIS reference
 *                links with specified type/subType
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

#include "osisreferencelinks.h"

#include <cstdlib>
#include "../swmodule.h"
#include "../utilfuns/utilxml.h"


namespace swordxx {


namespace {

    static const StringList *oValues() {
        static const std::string choices[3] = {"On", "Off", ""};
        static const StringList oVals(&choices[0], &choices[2]);
        return &oVals;
    }
}


OSISReferenceLinks::OSISReferenceLinks(const char *optionName, const char *optionTip, const char *type, const char *subType, const char *defaultValue)
        : SWOptionFilter(),
          optionName(optionName),
          optionTip(optionTip),
          type(type),
          subType(subType) {

    optName   = this->optionName.c_str();
    optTip    = this->optionTip.c_str();
    optValues = oValues();
    setOptionValue(defaultValue);
}


OSISReferenceLinks::~OSISReferenceLinks() {
}


char OSISReferenceLinks::processText(std::string &text, const SWKey *key, const SWModule *module) {
    if (option) return 0;

    std::string token;
    bool intoken        = false;
    bool stripThisToken = false;

    std::string orig = text;
    const char *from = orig.c_str();

    for (text = ""; *from; ++from) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }
        else if (*from == '>') {    // process tokens
            intoken = false;
            if (std::strncmp(token.c_str(), "reference", 9)
                && std::strncmp(token.c_str(), "/reference", 10)) {
                text.push_back('<');
                text.append(token);
                text.push_back('>');
            }
            else {
                XMLTag tag;
                tag = token.c_str();
                if (!tag.isEndTag() && type == tag.getAttribute("type") && (!subType.size() || subType == tag.getAttribute("subType"))) {
                    stripThisToken = true;
                    continue;
                }
                else if (tag.isEndTag() && stripThisToken) {
                    stripThisToken = false;
                    continue;
                }
                text.push_back('<');
                text.append(token);
                text.push_back('>');
            }
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


