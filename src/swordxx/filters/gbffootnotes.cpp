/******************************************************************************
 *
 *  gbffootnotes.cpp -    SWFilter descendant to hide or show footnotes
 *            in a GBF module
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "gbffootnotes.h"

#include <cstdio>
#include <cstdlib>
#include <string>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Footnotes";
    static const char oTip[]  = "Toggles Footnotes On and Off if they exist";

}

GBFFootnotes::GBFFootnotes()
    : OffOnOptionFilter(oName, oTip)
{}

GBFFootnotes::~GBFFootnotes() {
}


char GBFFootnotes::processText (std::string &text, const SWKey *key, const SWModule *module)
{
    (void) key;
    std::string token;
    bool intoken    = false;
    bool hide       = false;
    std::string tagText;
    XMLTag startTag;
    std::string refs = "";
    int footnoteNum = 1;

    std::string out;

    //XMLTag tag;

    using namespace std::literals::string_view_literals;

    for (const char *from = text.c_str(); *from; from++) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }
        if (*from == '>') {    // process tokens
            intoken = false;

            //XMLTag tag(token);
            if (startsWith(token, "RF"sv)) {
//                 tag = token;

                refs = "";
                startTag = token.c_str();
                hide = true;
                tagText = "";
                continue;
            }
            else if (startsWith(token, "Rf"sv)) {
                if (module->isProcessEntryAttributes()) {
                    //tag = token;

                    if((tagText.length() == 1) || (module->getName() == "IGNT")) {
                        if (isOptionOn()) { // for ASV marks text in verse then put explanation at end of verse
                            out.append(" <FS>[");
                            out.append(tagText);
                            out.append("]<Fs>");
                            hide = false;
                            continue;
                        }
                    }
                    std::string fc = module->getEntryAttributes()["Footnote"]["count"]["value"];
                    footnoteNum = (fc.length()) ? std::atoi(fc.c_str()) : 0;
                    auto footnoteNumStr(std::to_string(++footnoteNum));
                    module->getEntryAttributes()["Footnote"]["count"]["value"] =
                            footnoteNumStr;
                    for (auto const & [name, value] : startTag.attributes())
                        module->getEntryAttributes()["Footnote"][footnoteNumStr][name] =
                                value;
                    module->getEntryAttributes()["Footnote"][footnoteNumStr]["body"] = tagText;
                    startTag.attributes()["swordFootnote"] =
                            std::move(footnoteNumStr);
                }
                hide = false;
                if (isOptionOn()) {
                    out.append(startTag.toString());
                    out.append(tagText);
                }
                else    continue;
            }
            if (!hide) {
                out.push_back('<');
                out.append(token);
                out.push_back('>');
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
            out.push_back(*from);
        }
        else tagText.push_back(*from);
    }
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */
