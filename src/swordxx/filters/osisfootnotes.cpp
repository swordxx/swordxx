/******************************************************************************
 *
 *  osisfootnotes.cpp -    SWFilter descendant to hide or show footnotes
 *            in an OSIS module
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

#include "osisfootnotes.h"

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

OSISFootnotes::OSISFootnotes()
    : OffOnOptionFilter(oName, oTip)
{}

OSISFootnotes::~OSISFootnotes() {
}


char OSISFootnotes::processText(std::string &text, const SWKey *key, const SWModule *module) {
    std::string token;
    bool intoken    = false;
    bool hide       = false;
    std::string tagText;
    XMLTag startTag;
    std::string refs = "";
    int footnoteNum = 1;
    char buf[254];
    SWKey *p = (module) ? module->createKey() : (key) ? key->clone() : new VerseKey();
        VerseKey * parser = dynamic_cast<VerseKey *>(p);
        if (!parser) {
            delete p;
                parser = new VerseKey();
        }
        *parser = key->getText();

    std::string orig = text;
    const char *from = orig.c_str();

    XMLTag tag;
    bool strongsMarkup = false;


    for (text = ""; *from; ++from) {

        // remove all newlines temporarily to fix kjv2003 module
        if ((*from == 10) || (*from == 13)) {
            if ((text.length()>1) && (text[text.length()-2] != ' ') && (*(from+1) != ' '))
                text.push_back(' ');
            continue;
        }


        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }



        if (*from == '>') {    // process tokens
            intoken = false;
            if (!std::strncmp(token.c_str(), "note", 4)
                || !std::strncmp(token.c_str(), "/note", 5)) {
                tag = token.c_str();

                if (!tag.isEndTag()) {
                    if (!tag.getAttribute("type").empty() && (!strcmp("x-strongsMarkup", tag.getAttribute("type").c_str())
                                            || !strcmp("strongsMarkup", tag.getAttribute("type").c_str()))    // deprecated
                            ) {
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
                        for (auto const & attr : startTag.getAttributeNames())
                            module->getEntryAttributes()["Footnote"][buf][attr.c_str()] =
                                    startTag.getAttribute(attr.c_str());
                        module->getEntryAttributes()["Footnote"][buf]["body"] = tagText;
                        startTag.setAttribute("swordFootnote", buf);
                        if ((!startTag.getAttribute("type").empty()) && (!strcmp(startTag.getAttribute("type").c_str(), "crossReference"))) {
                            if (!refs.length())
                                refs = parser->parseVerseList(tagText.c_str(), parser->getText(), true).getRangeText();
                            module->getEntryAttributes()["Footnote"][buf]["refList"] = refs.c_str();
                        }
                    }
                    hide = false;
                    if (option || (!startTag.getAttribute("type").empty() && !strcmp(startTag.getAttribute("type").c_str(), "crossReference"))) {    // we want the tag in the text; crossReferences are handled by another filter
                        text.append(startTag.toString());
//                        text.append(tagText);    // we don't put the body back in because it is retrievable from EntryAttributes["Footnotes"][]["body"].
                    }
                    else    continue;
                }
                strongsMarkup = false;
            }

            // if not a heading token, keep token in text
            //if ((!strcmp(tag.getName(), "reference")) && (!tag.isEndTag())) {
            //    std::string osisRef = tag.getAttribute("osisRef");
            if (!std::strncmp(token.c_str(), "reference", 9)) {
                if (refs.length()) {
                    refs.append("; ");
                }

                const char* attr = strstr(token.c_str() + 9, "osisRef=\"");
                char const * end  = attr ? strchr(attr+9, '"') : nullptr;

                if (attr && end) {
                    refs.append(attr+9, end-(attr+9));
                }
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
        delete parser;
    return 0;
}

} /* namespace swordxx */

