/***************************************************************************
 *
 *  teirtf.cpp -    TEI to RTF filter
 *
 * $Id$
 *
 * Copyright 2006-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "teirtf.h"

#include <cctype>
#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilxml.h"


namespace swordxx {


TEIRTF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    BiblicalText = false;
    inOsisRef = false;
    if (module) {
        version = module->getName();
        BiblicalText = (module->getType() == "Biblical Texts");
    }
}


TEIRTF::TEIRTF() {
    setTokenStart("<");
    setTokenEnd(">");

    setEscapeStart("&");
    setEscapeEnd(";");

    setEscapeStringCaseSensitive(true);

    addEscapeStringSubstitute("amp", "&");
    addEscapeStringSubstitute("apos", "'");
    addEscapeStringSubstitute("lt", "<");
    addEscapeStringSubstitute("gt", ">");
    addEscapeStringSubstitute("quot", "\"");

    setTokenCaseSensitive(true);
}


bool TEIRTF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
    if (!substituteToken(buf, token)) {
        MyUserData *u = (MyUserData *)userData;
        XMLTag tag(token);

        // <p> paragraph tag
        if (tag.getName() == "p") {
            if (!tag.isEndTag()) {    // non-empty start tag
                buf += "{\\sb100\\fi200\\par}";
            }
        }

        // <hi>
        else if ((tag.getName() == "hi") || (tag.getName() == "emph")) {
            std::string rend = tag.attribute("rend");
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                if (rend == "italic" || rend == "ital")
                    buf += "{\\i1 ";
                else if (rend == "bold")
                    buf += "{\\b1 ";
                else if (rend == "super" || rend == "sup")
                        buf += "{\\super ";
                else if (rend == "sub")
                    buf += "{\\sub ";
            }
            else if (tag.isEndTag()) {
                buf += "}";
            }
        }

        // <entryFree>
        else if (tag.getName() == "entryFree") {
            std::string n = tag.attribute("n");
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                    if (n != "") {
                                    buf += "{\\b1 ";
                    buf += n;
                    buf += ". }";                }
            }
        }

        // <sense>
        else if (tag.getName() == "sense") {
            std::string n = tag.attribute("n");
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                    if (n != "") {
                                    buf += "{\\sb100\\par\\b1 ";
                    buf += n;
                    buf += ". }";
                }
            }
        }

         // <orth>
         else if (tag.getName() == "orth") {
             if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                 buf += "{\\b1 ";
             }
             else if (tag.isEndTag()) {
                     buf += "}";
             }
         }

        // <div>
        else if (tag.getName() == "div") {

            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf.append("{\\pard\\sa300}");
            }
            else if (tag.isEndTag()) {
            }
        }

        // <pos>, <gen>, <case>, <gram>, <number>, <mood>
        else if ((tag.getName() == "pos")
                 || (tag.getName() == "gen")
                 || (tag.getName() == "case")
                 || (tag.getName() == "gram")
                 || (tag.getName() == "number")
                 || (tag.getName() == "mood"))
        {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "{\\i1 ";
            }
            else if (tag.isEndTag()) {
                    buf += "}";
            }
        }

        // <tr>
        else if (tag.getName() == "tr") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "{\\i1 ";
            }
            else if (tag.isEndTag()) {
                    buf += "}";
            }
        }

        // <etym>
        else if (tag.getName() == "etym") {
            if ((!tag.isEndTag()) && (!tag.isEmpty())) {
                buf += "[";
            }
            else if (tag.isEndTag()) {
                    buf += "]";
            }
        }

               // <note> tag
        else if (tag.getName() == "note") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    std::string type = tag.attribute("type");

                    std::string footnoteNumber = tag.attribute("swordFootnote");
                    if (dynamic_cast<VerseKey const *>(u->key)) {
                        buf += formatted("{\\super <a href=\"\">*%s</a>} ", footnoteNumber.c_str());
                    }
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {
                u->suspendTextPassThru = false;
            }
        }

        // <lb/> tag
        else if (tag.getName() == "lb") {
            buf += "{\\par}";
            userData->supressAdjacentWhitespace = true;
        }

        // <ref> tag
        else if (tag.getName() == "ref") {
            if (!tag.isEndTag() && !tag.attribute("osisRef").empty()) {
                buf += "{<a href=\"\">";
                u->inOsisRef = true;
            }
            else if (tag.isEndTag() && u->inOsisRef) {
                buf += "</a>}";
                u->inOsisRef = false;
            }
        }

        else if (tag.getName() == "graphic") {
            auto src(tag.attribute("url"));
            if (src.empty())        // assert we have a src attribute
                return false;

            char* filepath = new char[strlen(u->module->getConfigEntry("AbsoluteDataPath")) + strlen(token)];
            *filepath = 0;
            strcpy(filepath, userData->module->getConfigEntry("AbsoluteDataPath"));
            strcat(filepath, src.c_str());

// we do this because BibleCS looks for this EXACT format for an image tag
            buf += "<img src=\"";
            buf += filepath;
            buf += "\" />";
            delete [] filepath;
        }

        else {
            return false;  // we still didn't handle token
        }

    }
    return true;
}


} /* namespace swordxx */

