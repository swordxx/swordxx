/***************************************************************************
 *
 *  thmlwebif.cpp -    ThML to HTML filter with hrefs
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

#include "thmlwebif.h"

#include <cctype>
#include <cstdlib>
#include "../swmodule.h"
#include "../url.h"
#include "../utilxml.h"


namespace swordxx {


ThMLWEBIF::ThMLWEBIF() : baseURL(""), passageStudyURL(baseURL + "passagestudy.jsp") {
}


bool ThMLWEBIF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {

    if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
        MyUserData *u = (MyUserData *)userData;
        XMLTag tag(token);
        std::string url;
        if (tag.name() == "sync") {
            auto value(tag.attribute("value"));
            url = value;
            if ((url.length() > 1) && strchr("GH", url[0])) {
                if (isdigit(url[1]))
                    url = url.c_str()+1;
            }

            if(!tag.attribute("type").empty() && !strcmp(tag.attribute("type").c_str(), "morph")){
                buf += "<small><em> (";
                buf += formatted("<a href=\"%s?showMorph=%s#cv\">", passageStudyURL.c_str(), URL::encode(url.c_str()).c_str() );
            }
            else {
                if (!value.empty()) {
                    value.erase(0u, 1u); //skip leading G, H or T
                    //url = value;
                }

                buf += "<small><em> &lt;";
                buf += formatted("<a href=\"%s?showStrong=%s#cv\">", passageStudyURL.c_str(), URL::encode(url.c_str()).c_str() );
            }

            buf += value;
            buf += "</a>";

            if (!tag.attribute("type").empty() && !strcmp(tag.attribute("type").c_str(), "morph")) {
                buf += ") </em></small>";
            }
            else {
                buf += "&gt; </em></small>";
            }
        }
        else if (tag.name() == "scripRef") {
            if (tag.isEndTag()) {
                if (u->inscriptRef) { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
                    u->inscriptRef = false;
                    buf += "</a>";
                }
                else { // end of scripRef like "<scripRef>John 3:16</scripRef>"
                    url = u->lastTextNode;
                    buf += formatted("<a href=\"%s?key=%s#cv\">", passageStudyURL.c_str(), URL::encode(url.c_str()).c_str());
                    buf += u->lastTextNode.c_str();
                    buf += "</a>";

                    // let's let text resume to output again
                    u->suspendTextPassThru = false;
                }
            }
            else if (!tag.attribute("passage").empty()) { //passage given
                u->inscriptRef = true;

                buf += formatted("<a href=\"%s?key=%s#cv\">", passageStudyURL.c_str(), URL::encode(tag.attribute("passage").c_str()).c_str());
            }
            else { //no passage given
                u->inscriptRef = false;
                // let's stop text from going to output
                u->suspendTextPassThru = true;
            }
        }
        else {
            return ThMLXHTML::handleToken(buf, token, userData);
        }
    }
    return true;
}


} /* namespace swordxx */
