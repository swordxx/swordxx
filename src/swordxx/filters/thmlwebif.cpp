/***************************************************************************
 *
 *  thmlwebif.cpp -    ThML to HTML filter with hrefs
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

#include "thmlwebif.h"

#include "../url.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

#define PASSAGE_STUDY_URL "passagestudy.jsp"

ThMLWEBIF::ThMLWEBIF() {}

bool ThMLWEBIF::handleToken(std::string & buf,
                            char const * token,
                            BasicFilterUserData * userData)
{
    // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        XMLTag const tag(token);
        if (tag.name() == "sync") {
            auto value(tag.attribute("value"));

            auto const url(
                        URL::encode(
                            std::string(
                                ((value.size() > 1u
                                  && (value[0u] == 'G' || value[0u] == 'H')
                                  && charIsDigit(value[1u]))
                                ? ++value.begin()
                                : value.end()),
                                value.end())));

            if (tag.attribute("type") == "morph") {
                buf += "<small><em> (<a href=\"" PASSAGE_STUDY_URL
                       "?showMorph=";
                buf += url;
                buf += "#cv\">";
                buf += value;
                buf += "</a>) </em></small>";
            } else {
                if (!value.empty()) {
                    value.erase(0u, 1u); //skip leading G, H or T
                    //url = value;
                }

                buf += "<small><em> &lt;<a href=\"" PASSAGE_STUDY_URL
                       "?showStrong=";
                buf += url;
                buf += "#cv\">";
                buf += value;
                buf += "</a>&gt; </em></small>";
            }
        } else if (tag.name() == "scripRef") {
            auto * const u = static_cast<MyUserData *>(userData);
            if (tag.isEndTag()) {
                if (u->inscriptRef) {
                    // like "<scripRef passage="John 3:16">John 3:16</scripRef>"
                    u->inscriptRef = false;
                    buf += "</a>";
                } else {
                    // end of scripRef like "<scripRef>John 3:16</scripRef>"
                    buf += "<a href=\"" PASSAGE_STUDY_URL "?key=";
                    buf += URL::encode(u->lastTextNode);
                    buf += "#cv\">";
                    buf += u->lastTextNode;
                    buf += "</a>";

                    // let's let text resume to output again
                    u->suspendTextPassThru = false;
                }
            } else if (!tag.attribute("passage").empty()) { //passage given
                u->inscriptRef = true;

                buf += "<a href=\"" PASSAGE_STUDY_URL "?key=";
                buf += URL::encode(tag.attribute("passage"));
                buf += "#cv\">";
            } else { //no passage given
                u->inscriptRef = false;
                // let's stop text from going to output
                u->suspendTextPassThru = true;
            }
        } else {
            return ThMLXHTML::handleToken(buf, token, userData);
        }
    }
    return true;
}

} /* namespace swordxx */
