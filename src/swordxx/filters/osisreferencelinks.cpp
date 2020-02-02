/******************************************************************************
 *
 *  osisreferencelinks.cpp -    SWFilter descendant to toggle OSIS reference
 *                links with specified type/subType
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
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

OSISReferenceLinks::OSISReferenceLinks(std::string optionName,
                                       std::string optionTip,
                                       std::string type,
                                       std::string subType,
                                       std::string_view defaultValue)
    : OnOffOptionFilter(std::move(optionName), std::move(optionTip))
    , m_type(std::move(type))
    , m_subType(std::move(subType))
{ setOptionValue(std::move(defaultValue)); }


OSISReferenceLinks::~OSISReferenceLinks() {
}


char OSISReferenceLinks::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    (void) key;
    (void) module;
    if (isOptionOn())
        return 0;

    std::string token;
    bool intoken        = false;
    bool stripThisToken = false;

    std::string out;

    for (auto const * from = text.c_str(); *from; ++from) {
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
                out.push_back('<');
                out.append(token);
                out.push_back('>');
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
                out.push_back('<');
                out.append(token);
                out.push_back('>');
            }
            continue;
        }

        if (intoken) { //copy token
            token.push_back(*from);
        }
        else { //copy text which is not inside a token
            out.push_back(*from);
        }
    }
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */


