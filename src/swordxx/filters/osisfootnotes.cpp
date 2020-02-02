/******************************************************************************
 *
 *  osisfootnotes.cpp -    SWFilter descendant to hide or show footnotes
 *            in an OSIS module
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

#include "osisfootnotes.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
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

OSISFootnotes::OSISFootnotes()
    : OffOnOptionFilter(oName, oTip)
{}

OSISFootnotes::~OSISFootnotes() {
}


char OSISFootnotes::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    std::string token;
    bool intoken    = false;
    bool hide       = false;
    std::string tagText;
    XMLTag startTag;
    std::string refs = "";
    int footnoteNum = 1;
    char buf[254];
    std::shared_ptr<VerseKey> parser;
    {
        std::shared_ptr<SWKey> p(module
                                 ? module->createKey()
                                 : (key
                                    ? key->clone()
                                    : std::make_shared<VerseKey>()));
        if (auto vk = std::dynamic_pointer_cast<VerseKey>(p)) {
            parser = std::move(vk);
        } else {
            parser = std::make_shared<VerseKey>();
        }
    }
    parser->setText(key->getText());
    std::string out;

    XMLTag tag;
    bool strongsMarkup = false;


    for (auto const * from = text.c_str(); *from; ++from) {

        // remove all newlines temporarily to fix kjv2003 module
        if ((*from == 10) || (*from == 13)) {
            if ((out.size()>1) && (out[out.size()-2] != ' ') && (*(from+1) != ' '))
                out.push_back(' ');
            continue;
        }


        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }



        if (*from == '>') {    // process tokens
            intoken = false;
            if (startsWith(token, "note"sv) || startsWith(token, "/note"sv)) {
                tag = token.c_str();

                if (!tag.isEndTag()) {
                    if ((tag.attribute("type") == "x-strongsMarkup")
                        || (tag.attribute("type") == "strongsMarkup"))    // deprecated
                    {
                        tag.setEmpty(false);  // handle bug in KJV2003 module where some note open tags were <note ... />
                        strongsMarkup = true;
                    }

                    if (!tag.isEmpty()) {
//                    if ((!tag.isEmpty()) || (std::string("strongsMarkup") == tag.getAttribute("type"))) {
                        refs = "";
                        startTag = tag;
                        hide = true;
                        tagText = "";
                        continue;
                    }
                }
                if (hide && tag.isEndTag()) {
                    if (module->isProcessEntryAttributes() && !strongsMarkup) { //don`t parse strongsMarkup to EntryAttributes as Footnote
                        sprintf(buf, "%i", footnoteNum++);
                        for (auto const & [name, value] : startTag.attributes())
                            module->getEntryAttributes()["Footnote"][buf][name] =
                                    value;
                        module->getEntryAttributes()["Footnote"][buf]["body"] = tagText;
                        startTag.attributes()["swordFootnote"] = buf;
                        if (startTag.attribute("type") == "crossReference") {
                            if (!refs.length())
                                refs = parser->parseVerseList(tagText.c_str(), parser->getText().c_str(), true).getRangeText();
                            module->getEntryAttributes()["Footnote"][buf]["refList"] = refs.c_str();
                        }
                    }
                    hide = false;
                    if (isOptionOn() || (startTag.attribute("type") == "crossReference")) {    // we want the tag in the text; crossReferences are handled by another filter
                        out.append(startTag.toString());
//                        out.append(tagText);    // we don't put the body back in because it is retrievable from EntryAttributes["Footnotes"][]["body"].
                    }
                    else    continue;
                }
                strongsMarkup = false;
            }

            // if not a heading token, keep token in text
            //if ((!std::strcmp(tag.getName(), "reference")) && (!tag.isEndTag())) {
            //    std::string osisRef = tag.getAttribute("osisRef");
            if (startsWith(token, "reference")) {
                if (refs.length()) {
                    refs.append("; ");
                }

                const char* attr = std::strstr(token.c_str() + 9, "osisRef=\"");
                char const * end  = attr ? std::strchr(attr+9, '"') : nullptr;

                if (attr && end) {
                    refs.append(attr+9, end-(attr+9));
                }
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

