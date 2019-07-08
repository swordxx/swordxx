/******************************************************************************
 *
 *  gbffootnotes.cpp -    SWFilter descendant to hide or show footnotes
 *            in a GBF module
 *
 * $Id$
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
    char buf[254];

    std::string orig = text;
    const char *from = orig.c_str();

    //XMLTag tag;

    using namespace std::literals::string_view_literals;

    for (text = ""; *from; from++) {
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
                        if (option) { // for ASV marks text in verse then put explanation at end of verse
                            text.append(" <FS>[");
                            text.append(tagText);
                            text.append("]<Fs>");
                            hide = false;
                            continue;
                        }
                    }
                    std::string fc = module->getEntryAttributes()["Footnote"]["count"]["value"];
                    footnoteNum = (fc.length()) ? std::atoi(fc.c_str()) : 0;
                    sprintf(buf, "%i", ++footnoteNum);
                    module->getEntryAttributes()["Footnote"]["count"]["value"] = buf;
                    for (auto const & attr : startTag.attributeNames())
                        module->getEntryAttributes()["Footnote"][buf][attr.c_str()] =
                                startTag.attribute(attr.c_str());
                    module->getEntryAttributes()["Footnote"][buf]["body"] = tagText;
                    startTag.setAttribute("swordFootnote", buf);
                }
                hide = false;
                if (option) {
                    text.append(startTag.toString());
                    text.append(tagText);
                }
                else    continue;
            }
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

    /*
    if (!option) {    // if we don't want footnotes
        char token[4096]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        int len;
        bool hide = false;

        const char *from;
        std::string orig = text;
        from = orig.c_str();
        for (text = ""; *from; from++) {
            if (*from == '<') {
                intoken = true;
                tokpos = 0;
//                std::memset(token, 0, 4096);
                token[0] = 0;
                token[1] = 0;
                token[2] = 0;
                continue;
            }
            if (*from == '>') {    // process tokens
                intoken = false;
                switch (*token) {
                case 'R':                // Reference
                    switch(token[1]) {
                    case 'F':               // Begin footnote
                        hide = true;
                        break;
                    case 'f':               // end footnote
                        hide = false;
                        break;
                    }
                    continue;    // skip token
                case 'W':
                    if (token[1] == 'T') {
                        switch (token[2]) {
                        case 'P':
                        case 'S':
                        case 'A':
                            continue; // remove this token
                        default:
                            break;
                        }
                    }
                }
                // if not a footnote token, keep token in text
                if (!hide) {
                    text += '<';
                    text += token;
                    text += '>';
                }
                continue;
            }
            if (intoken) {
                if (tokpos < 4090)
                    token[tokpos++] = *from;
                    token[tokpos+2] = 0;    // +2 cuz we init token with 2 extra '0' because of switch statement
            }
            else    {
                if (!hide) {
                    text += *from;
                }
            }
        }
    }
    return 0;*/
}

} /* namespace swordxx */
