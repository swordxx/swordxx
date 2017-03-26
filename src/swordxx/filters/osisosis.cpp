/***************************************************************************
 *
 *  osisosis.cpp -    internal OSIS to public OSIS filter
 *
 * $Id$
 *
 * Copyright 2004-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osisosis.h"

#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"


namespace swordxx {


OSISOSIS::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));
}


OSISOSIS::OSISOSIS() {
    setTokenStart("<");
    setTokenEnd(">");

    setEscapeStart("&");
    setEscapeEnd(";");

    setEscapeStringCaseSensitive(true);
    setPassThruNumericEscapeString(true);

    addAllowedEscapeString("quot");
    addAllowedEscapeString("apos");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    setTokenCaseSensitive(true);
}


char OSISOSIS::processText(std::string &text, const SWKey *key, const SWModule *module) {
    char status = SWBasicFilter::processText(text, key, module);
    if (VerseKey const * const vkey = dynamic_cast<VerseKey const *>(key)) {
        if (vkey->getVerse()) {
            VerseKey *tmp = (VerseKey *)vkey->clone();
            *tmp = *vkey;
            tmp->setAutoNormalize(false);
            tmp->setIntros(true);

            *tmp = Position::MaxVerse;
            if (*vkey == *tmp) {
                tmp->setVerse(0);
//                sprintf(ref, "\t</div>");
//                pushString(&to, ref);
                *tmp = Position::MaxChapter;
                *tmp = Position::MaxVerse;
                if (*vkey == *tmp) {
                    tmp->setChapter(0);
                    tmp->setVerse(0);
//                    sprintf(ref, "\t</div>");
//                    pushString(&to, ref);
                }
            }
                        delete tmp;
        }

//
//            else if (vkey->Chapter()) {
//                sprintf(ref, "\t<div type=\"chapter\" osisID=\"%s\">", vkey->getOSISRef());
//            }
//            else sprintf(ref, "\t<div type=\"book\" osisID=\"%s\">", vkey->getOSISRef());
//

    }
    return status;
}

bool OSISOSIS::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        MyUserData *u = (MyUserData *)userData;
        XMLTag tag(token);

        if (!tag.isEmpty() && (!tag.isEndTag()))
            u->startTag = tag;

        // <w> tag
        if (tag.getName() == "w") {

            // start <w> tag
            if ((!tag.isEmpty()) && (!tag.isEndTag())) {
                std::string attr = tag.attribute("lemma");
                if (attr.length()) {
                    if (!strncmp(attr.c_str(), "x-Strongs:", 10)) {
                        std::memcpy(&attr[3u], "strong", 6);
                        attr.erase(0u, 3u);
                        tag.setAttribute("lemma", attr.c_str());
                    }
                }
                attr = tag.attribute("morph");
                if (attr.length()) {
                    if (!strncmp(attr.c_str(), "x-StrongsMorph:", 15)) {
                        memcpy(&attr[3u], "strong", 6);
                        attr.erase(0u, 3u);
                        tag.setAttribute("lemma", attr.c_str());
                    }
                    if (!strncmp(attr.c_str(), "x-Robinson:", 11)) {
                        attr[2] = 'r';
                        attr.erase(0u, 2u);
                        tag.setAttribute("lemma", attr.c_str());
                    }
                }
                tag.setAttribute("wn", nullptr);
                tag.setAttribute("savlm", nullptr);
                tag.setAttribute("splitID", nullptr);
            }
            buf += tag.toString();
        }

        // <note> tag
        else if (tag.getName() == "note") {
            if (!tag.isEndTag()) {
                std::string type = tag.attribute("type");

                bool strongsMarkup = (type == "x-strongsMarkup" || type == "strongsMarkup");    // the latter is deprecated
                if (strongsMarkup) {
                    tag.setEmpty(false);    // handle bug in KJV2003 module where some note open tags were <note ... />
                }

                if (!tag.isEmpty()) {
                    tag.setAttribute("swordFootnote", nullptr);

                    if (!strongsMarkup) {
                        buf += tag.toString();
                    }
                    else u->suspendTextPassThru = true;
                }

                if (u->module) {
                                        XMLTag tag = token;
                                        std::string swordFootnote = tag.attribute("swordFootnote");
                                        std::string footnoteBody = u->module->getEntryAttributes()["Footnote"][swordFootnote]["body"];
                                        buf.append(u->module->renderText(footnoteBody.c_str()));
                                }
            }
            if (tag.isEndTag()) {
                if (u->suspendTextPassThru == false)
                    buf += tag.toString();
                else u->suspendTextPassThru = false;
            }
        }

        else {
             return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
