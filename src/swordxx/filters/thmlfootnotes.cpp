/******************************************************************************
 *
 *  thmlfootnotes.cpp -    SWFilter descendant to hide or show footnotes
 *            in a ThML module
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

#include "thmlfootnotes.h"

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

ThMLFootnotes::ThMLFootnotes()
    : OffOnOptionFilter(oName, oTip)
{}

ThMLFootnotes::~ThMLFootnotes() {
}


char ThMLFootnotes::processText(std::string &text, const SWKey *key, const SWModule *module) {
    std::string token;
    bool intoken    = false;
    bool hide       = false;
    std::string tagText;
    XMLTag startTag;
    std::string refs = "";
    int footnoteNum = 1;
    char buf[254];
    std::unique_ptr<VerseKey> parser;
    {
        std::unique_ptr<SWKey> p(
                    module
                    ? module->createKey()
                    : (key ? key->clone() : std::make_unique<VerseKey>()));
        if (auto * const vk = dynamic_cast<VerseKey *>(p.get())) {
            parser.reset(vk);
            p.release();
        } else {
            parser = std::make_unique<VerseKey>();
        }
    }
    *parser = key->getText();

    std::string orig = text;
    const char *from = orig.c_str();

    for (text = ""; *from; from++) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }
        if (*from == '>') {    // process tokens
            intoken = false;

            XMLTag tag(token.c_str());
            if (tag.name() == "note") {
                if (!tag.isEndTag()) {
                    if (!tag.isEmpty()) {
                        refs = "";
                        startTag = tag;
                        hide = true;
                        tagText = "";
                        continue;
                    }
                }
                if (hide && tag.isEndTag()) {
                    if (module->isProcessEntryAttributes()) {
                        std::string fc = module->getEntryAttributes()["Footnote"]["count"]["value"];
                        footnoteNum = (fc.length()) ? atoi(fc.c_str()) : 0;
                        sprintf(buf, "%i", ++footnoteNum);
                        module->getEntryAttributes()["Footnote"]["count"]["value"] = buf;
                        for (auto const & attr : startTag.attributeNames())
                            module->getEntryAttributes()["Footnote"][buf][attr.c_str()] =
                                    startTag.attribute(attr.c_str());
                        module->getEntryAttributes()["Footnote"][buf]["body"] = tagText;
                        startTag.setAttribute("swordFootnote", buf);
                        if ((!startTag.attribute("type").empty()) && (!strcmp(startTag.attribute("type").c_str(), "crossReference"))) {
                            if (!refs.length())
                                refs = parser->parseVerseList(tagText.c_str(), parser->getText(), true).getRangeText();
                            module->getEntryAttributes()["Footnote"][buf]["refList"] = refs.c_str();
                        }
                    }
                    hide = false;
                    if ((option) || ((!startTag.attribute("type").empty() && (!strcmp(startTag.attribute("type").c_str(), "crossReference"))))) {    // we want the tag in the text; crossReferences are handled by another filter
                        text += startTag.toString();
                        text.append(tagText);
                    }
                    else    continue;
                }
            }

            // if not a note token, keep token in text
            if ((tag.name() == "scripRef") && !tag.isEndTag()) {
                std::string osisRef = tag.attribute("passage");
                if (refs.length())
                    refs += "; ";
                refs += osisRef;
            }
            if (!hide) {
                text += '<';
                text.append(token);
                text += '>';
            }
            else {
                tagText += '<';
                tagText.append(token);
                tagText += '>';
            }
            continue;
        }
        if (intoken) { //copy token
            token += *from;
        }
        else if (!hide) { //copy text which is not inside a token
            text += *from;
        }
        else tagText += *from;
    }
    return 0;
}

} /* namespace swordxx */
