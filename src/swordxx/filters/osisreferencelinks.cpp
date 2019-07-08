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
#include "../utilxml.h"


namespace swordxx {

OSISReferenceLinks::OSISReferenceLinks(const char *optionName, const char *optionTip, const char *type, const char *subType, const char *defaultValue)
        : m_optionName(optionName)
        , m_optionTip(optionTip)
        , m_type(type)
        , m_subType(subType)
{
    optName   = m_optionName.c_str();
    optTip    = m_optionTip.c_str();
    setOptionValue(defaultValue);
}


OSISReferenceLinks::~OSISReferenceLinks() {
}


char OSISReferenceLinks::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    (void) key;
    (void) module;
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
            if (!startsWith(token, "reference"sv)
                && !startsWith(token, "/reference"sv))
            {
                text.push_back('<');
                text.append(token);
                text.push_back('>');
            }
            else {
                XMLTag tag;
                tag = token.c_str();
                if (!tag.isEndTag() && m_type == tag.attribute("type") && (!m_subType.size() || m_subType == tag.attribute("subType"))) {
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


