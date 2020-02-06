/***************************************************************************
 *
 *  osisosis.cpp -    internal OSIS to public OSIS filter
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
#include <cstring>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilstr.h"


namespace swordxx {


OSISOSIS::MyUserData::MyUserData(SWModule const * module_,
                                 SWKey const * key_)
    : BasicFilterUserData(module_, key_)
{
    osisQToTick = ((!module_->getConfigEntry("OSISqToTick")) || (std::strcmp(module_->getConfigEntry("OSISqToTick"), "false")));
}

OSISOSIS::MyUserData::~MyUserData() noexcept = default;

OSISOSIS::OSISOSIS()
    : SWBasicFilter(CaseSensitiveTokens
                    | CaseSensitiveEscapeStrings
                    | PassThroughNumericEscapeStrings)
{
    addAllowedEscapeString("quot");
    addAllowedEscapeString("apos");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");
}

std::unique_ptr<BasicFilterUserData> OSISOSIS::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

char OSISOSIS::processText(std::string &text, const SWKey *key, const SWModule *module) {
    char status = SWBasicFilter::processText(text, key, module);
    if (VerseKey const * const vkey = dynamic_cast<VerseKey const *>(key)) {
        if (vkey->getVerse()) {
            auto const tmp(std::static_pointer_cast<VerseKey>(vkey->clone()));
            *tmp = *vkey;
            tmp->setAutoNormalize(false);
            tmp->setIntros(true);

            tmp->positionToMaxVerse();
            if (*vkey == *tmp) {
                tmp->setVerse(0);
//                sprintf(ref, "\t</div>");
//                pushString(&to, ref);
                tmp->positionToMaxChapter();
                tmp->positionToMaxVerse();
                if (*vkey == *tmp) {
                    tmp->setChapter(0);
                    tmp->setVerse(0);
//                    sprintf(ref, "\t</div>");
//                    pushString(&to, ref);
                }
            }
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
    using namespace std::literals::string_view_literals;

  // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        MyUserData * u = static_cast<MyUserData *>(userData);
        XMLTag tag(token);

        if (!tag.isEmpty() && (!tag.isEndTag()))
            u->startTag = tag;

        // <w> tag
        if (tag.name() == "w") {

            // start <w> tag
            if ((!tag.isEmpty()) && (!tag.isEndTag())) {
                if (auto attr = tag.attribute("lemma"); !attr.empty()) {
                    if (startsWith(attr, "x-Strongs:"sv)) {
                        std::memcpy(&attr[3u], "strong", 6);
                        attr.erase(0u, 3u);
                        tag.attributes()["lemma"] = std::move(attr);
                    }
                }
                if (auto attr = tag.attribute("morph"); !attr.empty()) {
                    if (startsWith(attr, "x-StrongsMorph:"sv)) {
                        // s/^x-Strongs/strong/:
                        std::memcpy(&attr[3u], "strong", 6);
                        attr.erase(0u, 3u);
                        tag.attributes()["lemma"] = std::move(attr);
                    } else if (startsWith(attr, "x-Robinson:"sv)) {
                        // s/^x-R/r/:
                        attr[2] = 'r';
                        attr.erase(0u, 2u);
                        tag.attributes()["lemma"] = std::move(attr);
                    }
                }
                tag.attributes().erase("wn");
                tag.attributes().erase("savlm");
                tag.attributes().erase("splitID");
            }
            buf += tag.toString();
        }

        // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                std::string type = tag.attribute("type");

                bool strongsMarkup = (type == "x-strongsMarkup" || type == "strongsMarkup");    // the latter is deprecated
                if (strongsMarkup) {
                    tag.setEmpty(false);    // handle bug in KJV2003 module where some note open tags were <note ... />
                }

                if (!tag.isEmpty()) {
                    tag.attributes().erase("swordFootnote");

                    if (!strongsMarkup) {
                        buf += tag.toString();
                    }
                    else u->suspendTextPassThru = true;
                }

                if (u->module) {
                                        XMLTag tag2 = token;
                                        std::string swordFootnote = tag2.attribute("swordFootnote");
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
