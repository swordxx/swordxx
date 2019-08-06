/***************************************************************************
 *
 *  osiswebif.cpp -    OSIS to HTML filter with hrefs for strongs and
 *            morph tags
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

#include "osiswebif.h"

#include <cstdlib>
#include <cstring>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../url.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {


OSISWEBIF::OSISWEBIF() : baseURL(""), passageStudyURL(baseURL + "passagestudy.jsp"), javascript(false) {
}

std::unique_ptr<BasicFilterUserData> OSISWEBIF::createUserData(
        SWModule const * module,
        SWKey const * key)
{
    std::unique_ptr<MyUserData> u(
                static_cast<MyUserData *>(
                    OSISXHTML::createUserData(module, key).release()));
    u->interModuleLinkStart = "<a href=\"#\" onclick=\"return im('%s', '%s');\">";
    u->interModuleLinkEnd = "</a>";
    if (module) u->fn = module->getConfigEntry("EmbeddedFootnoteMarkers");
    return u;
}

bool OSISWEBIF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    auto & u = *static_cast<MyUserData *>(userData);
    std::string scratch;
    bool sub = (u.suspendTextPassThru) ? substituteToken(scratch, token) : substituteToken(buf, token);
    if (!sub) {

        // manually process if it wasn't a simple substitution
        XMLTag tag(token);

        // <w> tag
        if (tag.name() == "w") {

            // start <w> tag
            if ((!tag.isEmpty()) && (!tag.isEndTag())) {
                u.w = token;
            }

            // end or empty <w> tag
            else {
                bool endTag = tag.isEndTag();
                std::string lastText;
                bool show = true;    // to handle unplaced article in kjv2003-- temporary till combined

                if (endTag) {
                    tag = u.w.c_str();
                    lastText = u.lastTextNode.c_str();
                }
                else lastText = "stuff";

                std::string attrib;
                const char *val;
                if (!(attrib = tag.attribute("xlit")).empty()) {
                    val = std::strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
//                    buf += formatted(" %s", val);
                }
                if (!(attrib = tag.attribute("gloss")).empty()) {
                    val = std::strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
//                    buf += formatted(" %s", val);
                }
                if (!(attrib = tag.attribute("lemma")).empty()) {
                    int count = tag.attributePartCount("lemma", ' ');
                    int i = 0;
                    do {
                        attrib = tag.attribute("lemma", i, ' ');
                        val = std::strchr(attrib.c_str(), ':');
                        val = (val) ? (val + 1) : attrib.c_str();
                        const char *val2 = val;
                        if ((std::strchr("GH", *val)) && (charIsDigit(val[1])))
                            val2++;
                        if ((!std::strcmp(val2, "3588")) && (lastText.length() < 1))
                            show = false;
                        else
                            buf += formatted(" <small><em>&lt;<a href=\"%s?showStrong=%s#cv\">%s</a>&gt;</em></small> ", passageStudyURL, URL::encode(val2), val2);
                    } while (++i < count);
                }
                if (!(attrib = tag.attribute("morph")).empty() && (show)) {
                    std::string savelemma = tag.attribute("savlm");
                    if ((std::strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
                        show = false;
                    if (show) {
                        int count = tag.attributePartCount("morph", ' ');
                        int i = 0;
                        do {
                            attrib = tag.attribute("morph", i, ' ');
                            val = std::strchr(attrib.c_str(), ':');
                            val = (val) ? (val + 1) : attrib.c_str();
                            const char *val2 = val;
                            if ((*val == 'T') && (std::strchr("GH", val[1])) && (charIsDigit(val[2])))
                                val2+=2;
                            buf += formatted(" <small><em>(<a href=\"%s?showMorph=%s#cv\">%s</a>)</em></small> ", passageStudyURL, URL::encode(val2), val2);
                        } while (++i < count);
                    }
                }
                if (!(attrib = tag.attribute("POS")).empty()) {
                    val = std::strchr(attrib.c_str(), ':');
                    val = (val) ? (val + 1) : attrib.c_str();
                    buf += formatted(" %s", val);
                }

                /*if (endTag)
                    buf += "}";*/
            }
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
                    if (!strongsMarkup) {    // leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
                        auto const footnoteNumber(tag.attribute("swordFootnote"));
                        auto const n(tag.attribute("n"));
                        auto const modName((u.module) ? u.module->getName() : "");
                        if (auto const * const vkey = u.verseKey) {
                            char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x':'n';
//                            buf += formatted("<a href=\"noteID=%s.%c.%s\"><small><sup>*%c</sup></small></a> ", vkey->getText(), ch, footnoteNumber.c_str(), ch);
                            buf.append("<span class=\"fn\" onclick=\"f(\'")
                               .append(modName)
                               .append("\',\'")
                               .append(vkey->getText())
                               .append("\',\'")
                               .append(footnoteNumber)
                               .append("\');\"");
                            if (!n.empty()) {
                                buf.append(" data-n=\"").append(n).append("\">");
                            } else if (u.fn != "true") {
                                buf.append(" data-n=\"").append(1u, ch).append("\">");
                            }
                            buf.append(1u, ch)
                               .append("</span>");
                        }
                    }
                    u.suspendTextPassThru = (++u.suspendLevel);
                }
            }
            if (tag.isEndTag()) {
                u.suspendTextPassThru = (--u.suspendLevel);

            }
        }


        // handled appropriately in base class
        else {
            return OSISXHTML::handleToken(buf, token, userData);
        }
    }
    return true;
}


} /* namespace swordxx */

