/***************************************************************************
 *
 *  teihtmlhref.cpp -    TEI to HTML with hrefs filter
 *
 * $Id$
 *
 * Copyright 2008-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "teihtmlhref.h"

#include <cctype>
#include <cstdlib>
#include <iostream>
#include "../swmodule.h"
#include "../url.h"
#include "../utilxml.h"


namespace swordxx {


TEIHTMLHREF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    BiblicalText = false;
    if (module) {
        version = module->getName();
        BiblicalText = (module->getType() == "Biblical Texts");
    }
}


TEIHTMLHREF::TEIHTMLHREF() {
    setTokenStart("<");
    setTokenEnd(">");

    setEscapeStart("&");
    setEscapeEnd(";");

    setEscapeStringCaseSensitive(true);

    addAllowedEscapeString("quot");
    addAllowedEscapeString("apos");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    setTokenCaseSensitive(true);

    renderNoteNumbers = false;
}

bool TEIHTMLHREF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        MyUserData *u = (MyUserData *)userData;
        XMLTag tag(token);

        if (tag.getName() == "p") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {    // non-empty start tag
                buf += "<!P><br />";
            }
            else if (tag.isEndTag()) {    // end tag
                buf += "<!/P><br />";
                //userData->supressAdjacentWhitespace = true;
            }
            else {                    // empty paragraph break marker
                buf += "<!P><br />";
                //userData->supressAdjacentWhitespace = true;
            }
        }

        // <hi>
        else if (tag.getName() == "hi") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                std::string rend = tag.attribute("rend");

                u->lastHi = rend;
                if (rend == "italic" || rend == "ital")
                    buf += "<i>";
                else if (rend == "bold")
                    buf += "<b>";
                else if (rend == "super" || rend == "sup")
                    buf += "<sup>";
                else if (rend == "sub")
                    buf += "<sub>";
                else if (rend == "overline")
                    buf += "<span style=\"text-decoration:overline\">";
            }
            else if (tag.isEndTag()) {
                std::string rend = u->lastHi;
                if (rend == "italic" || rend == "ital")
                    buf += "</i>";
                else if (rend == "bold")
                    buf += "</b>";
                else if (rend == "super" || rend == "sup")
                    buf += "</sup>";
                else if (rend == "sub")
                    buf += "</sub>";
                else if (rend == "overline")
                    buf += "</span>";
            }
        }

        // <entryFree>
        else if (tag.getName() == "entryFree") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                std::string n = tag.attribute("n");
                if (n != "") {
                    buf += "<b>";
                    buf += n;
                    buf += "</b>";
                }
            }
        }

        // <sense>
        else if (tag.getName() == "sense") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                std::string n = tag.attribute("n");
                if (n != "") {
                    buf += "<br /><b>";
                    buf += n;
                    buf += "</b> ";
                }
            }
        }

        // <div>
        else if (tag.getName() == "div") {

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<!P>";
            }
            else if (tag.isEndTag()) {
            }
        }

        // <lb.../>
        else if (tag.getName() == "lb") {
            buf += "<br />";
        }

        // <pos>, <gen>, <case>, <gram>, <number>, <mood>, <pron>, <def>
        else if ((tag.getName() == "pos") ||
                 (tag.getName() == "gen") ||
                 (tag.getName() == "case") ||
                 (tag.getName() == "gram") ||
                 (tag.getName() == "number") ||
                 (tag.getName() == "pron") /*||
                 (tag.getName() == "def")*/) {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<i>";
            }
            else if (tag.isEndTag()) {
                buf += "</i>";
            }
        }

        // <tr>
        else if (tag.getName() == "tr") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<i>";
            }
            else if (tag.isEndTag()) {
                buf += "</i>";
            }
        }

        // orth
        else if (tag.getName() == "orth") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<b>";
            }
            else if (tag.isEndTag()) {
                buf += "</b>";
            }
        }

        // <etym>, <usg>
        else if ((tag.getName() == "etym") ||
                 (tag.getName() == "usg")) {
            // do nothing here
        }
        else if (tag.getName() == "ref") {
            if (!tag.isEndTag()) {
                u->suspendTextPassThru = true;
                std::string target;
                std::string work;
                std::string ref;

                int was_osisref = false;
                if(!tag.attribute("osisRef").empty())
                {
                    target += tag.attribute("osisRef");
                    was_osisref=true;
                }
                else if(!tag.attribute("target").empty())
                    target += tag.attribute("target");

                if(target.size())
                {
                    const char* the_ref = std::strchr(target.c_str(), ':');

                    if(!the_ref) {
                        // No work
                        ref = target;
                    }
                    else {
                        // Compensate for starting :
                        ref = the_ref + 1;

                        int size = target.size() - ref.size() - 1;
                        work.resize(size, '\0');
                        std::strncpy(&work[0u], target.c_str(), size);
                    }

                    if(was_osisref)
                    {
                        buf += formatted("<a href=\"passagestudy.jsp?action=showRef&type=scripRef&value=%s&module=%s\">",
                            (!ref.empty()) ? URL::encode(ref.c_str()).c_str() : "",
                            (!work.empty()) ? URL::encode(work.c_str()).c_str() : "");
                    }
                    else
                    {
                        // Dictionary link, or something
                        buf += formatted("<a href=\"sword://%s/%s\">",
                            (!work.empty()) ? URL::encode(work.c_str()).c_str() : u->version.c_str(),
                            (!ref.empty()) ? URL::encode(ref.c_str()).c_str() : ""
                            );
                    }
                }
                else
                {
                    //std::cout << "TARGET WASN'T\n";
                }

            }
            else {
                buf += u->lastTextNode.c_str();
                buf += "</a>";

                u->suspendTextPassThru = false;
            }
        }

           // <note> tag
        else if (tag.getName() == "note") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {
                std::string footnoteNumber = tag.attribute("swordFootnote");
                std::string noteName = tag.attribute("n");

                buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=n&value=%s&module=%s&passage=%s\"><small><sup class=\"n\">*n%s</sup></small></a>",
                    URL::encode(footnoteNumber.c_str()).c_str(),
                    URL::encode(u->version.c_str()).c_str(),
                    URL::encode(u->key->getText()).c_str(),
                    (renderNoteNumbers ? URL::encode(noteName.c_str()).c_str() : ""));

                u->suspendTextPassThru = false;
            }
        }
        // <graphic> image tag
        else if (tag.getName() == "graphic") {
            auto url(tag.attribute("url"));
            if (!url.empty()) {        // assert we have a url attribute
                std::string filepath;
                if (userData->module) {
                    filepath = userData->module->getConfigEntry("AbsoluteDataPath");
                    if ((filepath.size()) && (filepath[filepath.size()-1] != '/') && (url[0] != '/'))
                        filepath += '/';
                }
                filepath += url;
                // images become clickable, if the UI supports showImage.
                buf += formatted("<a href=\"passagestudy.jsp?action=showImage&value=%s&module=%s\"><img src=\"file:%s\" border=\"0\" /></a>",
                            URL::encode(filepath.c_str()).c_str(),
                            URL::encode(u->version.c_str()).c_str(),
                            filepath.c_str());
                u->suspendTextPassThru = true;
            }
        }
        // <table> <row> <cell>
        else if (tag.getName() == "table") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<table><tbody>\n";
            }
            else if (tag.isEndTag()) {
                buf += "</tbody></table>\n";
                u->supressAdjacentWhitespace = true;
            }

        }
        else if (tag.getName() == "row") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "\t<tr>";
            }
            else if (tag.isEndTag()) {
                buf += "</tr>\n";
            }
        }
        else if (tag.getName() == "cell") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "<td>";
            }
            else if (tag.isEndTag()) {
                buf += "</td>";
            }
        }

        else {
            return false;  // we still didn't handle token
        }

    }
    return true;
}


} /* namespace swordxx */

