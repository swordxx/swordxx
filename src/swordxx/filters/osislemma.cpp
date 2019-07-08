/******************************************************************************
 *
 *  osislemma.cpp -    SWFilter descendant to hide or show lemmata
 *                   in a OSIS module
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

#include "osislemma.h"

#include <cstdlib>
#include <cstring>
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Lemmas";
    static const char oTip[]  = "Toggles Lemmas On and Off if they exist";

}

OSISLemma::OSISLemma()
    : OffOnOptionFilter(oName, oTip)
{}

OSISLemma::~OSISLemma() {
}


char OSISLemma::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    (void) module;
    std::string token;
    bool intoken = false;

    const std::string orig = text;
    const char * from = orig.c_str();

    using namespace std::literals::string_view_literals;

    if (!option) {
        for (text = ""; *from; ++from) {
            if (*from == '<') {
                intoken = true;
                token = "";
                continue;
            }
            if (*from == '>') {    // process tokens
                intoken = false;
                if (startsWith(token, "w "sv)) {    // Word
                    XMLTag wtag(token.c_str());

                    // Always save off lemma if we haven't yet:
                    if (!wtag.attribute("savlm").empty()) {
                        auto const lemma(wtag.attribute("lemma"));
                        if (!lemma.empty())
                            wtag.setAttribute("savlm", lemma.c_str());
                    }

                    int count = wtag.attributePartCount("lemma", ' ');
                    for (int i = 0; i < count; i++) {
                        std::string a = wtag.attribute("lemma", i, ' ');
                        auto const prefixPos(a.find(':'));
                        if (prefixPos == std::string::npos
                            || (prefixPos >= sizeof("lemma.") - 1u
                                && std::strncmp(a.c_str(),
                                                "lemma.",
                                                sizeof("lemma.") - 1u)))
                        {
                            // remove attribute part
                            wtag.setAttribute("lemma", nullptr, i, ' ');
                            i--;
                            count--;
                        }
                    }

                    token = wtag.toString();
                    trimString(token);
                    // drop <>
                    token.erase(0u, 1u);
                    token.pop_back();
                }

                // keep token in text
                text.push_back('<');
                text.append(token);
                text.push_back('>');

                continue;
            }
            if (intoken) {
                token += *from;
            }
            else    {
                text.push_back(*from);
            }
        }
    }
    return 0;
}

} /* namespace swordxx */
