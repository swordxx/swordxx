/******************************************************************************
 *
 *  osisvariants.cpp -    SWFilter descendant to hide or show textual variants
 *            in an OSIS module
 *
 * $Id$
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osisvariants.h"

#include <cstdlib>
#include <list>
#include <string>
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Textual Variants";
    static const char oTip[]  = "Switch between Textual Variants modes";
    static const char *choices[4] = { "Primary Reading", "Secondary Reading", "All Readings", "" };

    static const std::list<std::string> *oValues() {
        static const std::list<std::string> oVals(&choices[0], &choices[3]);
        return &oVals;
    }
}


OSISVariants::OSISVariants() : SWOptionFilter(oName, oTip, oValues()) {
}


OSISVariants::~OSISVariants() {
}


char OSISVariants::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    using namespace std::literals::string_view_literals;

    (void) key;
    (void) module;

    int option_ = 0;
    if      (optionValue == choices[0]) option_ = 0;
    else if (optionValue == choices[1]) option_ = 1;
    else                                option_ = 2;

    if (option_ == 0 || option_ == 1) { //we want primary or variant only
        bool intoken = false;
        bool hide = false;
        bool invar = false;

        std::string token;
        std::string out;
        XMLTag tag;

        //we use a fixed comparision string to make sure the loop is as fast as the original two blocks with almost the same code
        const char* variantChoice = (option_ == 0) ? "x-2" : "x-1";

        for (auto const * from = text.c_str(); *from; from++) {
            if (*from == '<') {
                intoken = true;
                token = "";
                continue;
            }
            else if (*from == '>') {    // process tokens
                intoken = false;

                if (startsWith(token, "seg"sv)) {
                    tag = token.c_str();

                    if ((tag.attribute("type") == "x-variant") && (tag.attribute("subType") == variantChoice)) {
                        invar = true;
                        hide = true;
                        continue;
                    }
                }
                if (startsWith(token, "/seg"sv)) {
                    hide = false;
                    if (invar) {
                        invar = false;
                        continue;
                    }
                }
                if (!hide) {
                    out += '<';
                    out.append(token);
                    out += '>';
                }

                continue;
            }
            if (intoken) {
                token += *from;
            }
            else if (!hide) {
                out += *from;
            }
        }
        text = std::move(out);
    }

    return 0;
}


} /* namespace swordxx */
