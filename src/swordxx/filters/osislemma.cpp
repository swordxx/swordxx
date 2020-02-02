/******************************************************************************
 *
 *  osislemma.cpp -    SWFilter descendant to hide or show lemmata
 *                   in a OSIS module
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

    std::string out;

    using namespace std::literals::string_view_literals;

    if (isOptionOff()) {
        for (auto const * from = text.c_str(); *from; ++from) {
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
                    if (!wtag.attribute("savlm").empty())
                        if (auto lemma = wtag.attribute("lemma"); !lemma.empty())
                            wtag.attributes()["savlm"] = std::move(lemma);

                    std::string newLemmaAttr;
                    {
                        auto const lemma(wtag.attribute("lemma"));
                        std::size_t separatorPos;
                        for (std::string_view tail = lemma;;
                             tail.remove_prefix(separatorPos + 1u))
                        {
                            separatorPos = tail.find(' ');
                            auto const attrToken(tail.substr(0u, separatorPos));

                            auto const prefixPos(attrToken.find(':'));
                            // Only keep lemmas:
                            if (prefixPos != std::string::npos
                                && (prefixPos < sizeof("lemma.") - 1u
                                    || startsWith(attrToken, "lemma."sv)))
                            {
                                if (!newLemmaAttr.empty())
                                    newLemmaAttr.push_back(' ');
                                newLemmaAttr.append(attrToken);
                            }

                            if (separatorPos == std::string_view::npos)
                                break;
                        }
                    }
                    wtag.attributes()["lemma"] = newLemmaAttr;

                    token = wtag.toString();
                    trimString(token);
                    // drop <>
                    token.erase(0u, 1u);
                    token.pop_back();
                }

                // keep token in text
                out.push_back('<');
                out.append(token);
                out.push_back('>');

                continue;
            }
            if (intoken) {
                token += *from;
            }
            else    {
                out.push_back(*from);
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
