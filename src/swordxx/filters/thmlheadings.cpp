/******************************************************************************
 *
 *  thmlheadings.cpp -    SWFilter descendant to hide or show headings
 *            in a ThML module
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

#include "thmlheadings.h"

#include <cstdio>
#include <cstdlib>
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Headings";
    static const char oTip[]  = "Toggles Headings On and Off if they exist";

}

ThMLHeadings::ThMLHeadings()
    : OffOnOptionFilter(oName, oTip)
{}

ThMLHeadings::~ThMLHeadings() {
}


char ThMLHeadings::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    std::string token;
    bool intoken    = false;
    bool isheader   = false;
    bool hide       = false;
    bool preverse   = false;
    bool withinDiv  = false;
    std::string header;
    int headerNum   = 0;
    int pvHeaderNum = 0;
    char buf[254];
    XMLTag startTag;

    std::string out;

    XMLTag tag;

    using namespace std::literals::string_view_literals;

    for (auto const * from = text.c_str(); *from; ++from) {
        if (*from == '<') {
            intoken = true;
            token = "";

            continue;
        }
        if (*from == '>') {    // process tokens
            intoken = false;

            if (!strnicmp(token.c_str(), "div", 3) || !strnicmp(token.c_str(), "/div", 4)) {
                withinDiv =  (!strnicmp(token.c_str(), "div", 3));
                tag = token.c_str();
                if (hide && tag.isEndTag()) {
                    if (module->isProcessEntryAttributes() && (isOptionOn() || (!preverse))) {
                        std::string heading;
                        if (auto cls = startTag.attribute("class");
                            !startsWith(cls, "fromEntryAttributes"sv))
                            startTag.attributes()["class"] =
                                    "fromEntryAttributes " + cls;
                        heading += startTag.toString();
                        heading += header;
                        heading += tag.toString();
                        if (preverse) {
                            sprintf(buf, "%i", pvHeaderNum++);
                            module->getEntryAttributes()["Heading"]["Preverse"][buf] = heading;
                        }
                        else {
                            sprintf(buf, "%i", headerNum++);
                            module->getEntryAttributes()["Heading"]["Interverse"][buf] = heading;
                            if (isOptionOn()) {    // we want the tag in the text
                                out.append(header);
                            }
                        }

                        for (auto const & [name, value] : startTag.attributes())
                            module->getEntryAttributes()["Heading"][buf][name] =
                                    value;
                    }

                    hide = false;
                    if (isOptionOff() || preverse) {    // we don't want the tag in the text anymore
                        preverse = false;
                        continue;
                    }
                    preverse = false;
                }
                if (!tag.attribute("class").empty() && ((caseInsensitiveEquals(tag.attribute("class"), "sechead"))
                                         ||  (caseInsensitiveEquals(tag.attribute("class"), "title")))) {

                    isheader = true;

                    if (!tag.isEndTag()) { //start tag
                        if (!tag.isEmpty()) {
                            startTag = tag;

/* how do we tell a ThML preverse title from one that should be in the text?  probably if any text is before the title...  just assuming all are preverse for now
                    }
                    if (!tag.attribute("subtype").empty() && caseInsensitiveEquals(tag.attribute("subtype"), "x-preverse")) {
*/
                        hide = true;
                        preverse = true;
                        header = "";
                        continue;
                        }    // move back up under startTag = tag
                    }
/* this is where non-preverse will go eventually
                    if (!tag.isEndTag()) { //start tag
                        hide = true;
                        header = "";
                        if (option) {    // we want the tag in the text
                            out.append('<');
                            out.append(token);
                            out.append('>');
                        }
                        continue;
                    }
*/
                }
                else {
                    isheader = false;
                    if (auto cls = tag.attribute("class");
                        startsWith(cls, "fromEntryAttributes "sv))
                    {
                        cls.erase(0u, "fromEntryAttributes "sv.size());
                        tag.attributes()["class"] = std::move(cls);
                        token = tag.toString();
                        token.erase(0u, 1u);
                        token.pop_back();
                    }
                }
            }

            if (withinDiv && isheader) {
                header.push_back('<');
                header.append(token);
                header.push_back('>');
            } else {
                // if not a heading token, keep token in text
                if (!hide) {
                    out.push_back('<');
                    out.append(token);
                    out.push_back('>');
                }
            }
            continue;
        }
        if (intoken) { //copy token
            token.push_back(*from);
        }
        else if (!hide) { //copy text which is not inside a token
            out.push_back(*from);
        }
        else header.push_back(*from);
    }
    text = std::move(out);
    return 0;
}


} /* namespace swordxx */
