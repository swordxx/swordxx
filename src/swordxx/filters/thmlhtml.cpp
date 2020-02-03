/***************************************************************************
 *
 *  thmlhtml.cpp -    ThML to HTML filter
 *
 * Copyright 1999-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "thmlhtml.h"

#include <cstdlib>
#include <cstring>
#include "../swmodule.h"
#include "../utilxml.h"


namespace swordxx {

ThMLHTML::MyUserData::~MyUserData() noexcept = default;

ThMLHTML::ThMLHTML() {
    setEscapeStringCaseSensitive(true);
    setPassThruNumericEscapeString(true);

    addAllowedEscapeString("quot");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    addAllowedEscapeString("nbsp");
    addAllowedEscapeString("brvbar"); // "�"
    addAllowedEscapeString("sect");   // "�"
    addAllowedEscapeString("copy");   // "�"
    addAllowedEscapeString("laquo");  // "�"
    addAllowedEscapeString("reg");    // "�"
    addAllowedEscapeString("acute");  // "�"
    addAllowedEscapeString("para");   // "�"
    addAllowedEscapeString("raquo");  // "�"

    addAllowedEscapeString("Aacute"); // "�"
    addAllowedEscapeString("Agrave"); // "�"
    addAllowedEscapeString("Acirc");  // "�"
    addAllowedEscapeString("Auml");   // "�"
    addAllowedEscapeString("Atilde"); // "�"
    addAllowedEscapeString("Aring");  // "�"
    addAllowedEscapeString("aacute"); // "�"
    addAllowedEscapeString("agrave"); // "�"
    addAllowedEscapeString("acirc");  // "�"
    addAllowedEscapeString("auml");   // "�"
    addAllowedEscapeString("atilde"); // "�"
    addAllowedEscapeString("aring");  // "�"
    addAllowedEscapeString("Eacute"); // "�"
    addAllowedEscapeString("Egrave"); // "�"
    addAllowedEscapeString("Ecirc");  // "�"
    addAllowedEscapeString("Euml");   // "�"
    addAllowedEscapeString("eacute"); // "�"
    addAllowedEscapeString("egrave"); // "�"
    addAllowedEscapeString("ecirc");  // "�"
    addAllowedEscapeString("euml");   // "�"
    addAllowedEscapeString("Iacute"); // "�"
    addAllowedEscapeString("Igrave"); // "�"
    addAllowedEscapeString("Icirc");  // "�"
    addAllowedEscapeString("Iuml");   // "�"
    addAllowedEscapeString("iacute"); // "�"
    addAllowedEscapeString("igrave"); // "�"
    addAllowedEscapeString("icirc");  // "�"
    addAllowedEscapeString("iuml");   // "�"
    addAllowedEscapeString("Oacute"); // "�"
    addAllowedEscapeString("Ograve"); // "�"
    addAllowedEscapeString("Ocirc");  // "�"
    addAllowedEscapeString("Ouml");   // "�"
    addAllowedEscapeString("Otilde"); // "�"
    addAllowedEscapeString("oacute"); // "�"
    addAllowedEscapeString("ograve"); // "�"
    addAllowedEscapeString("ocirc");  // "�"
    addAllowedEscapeString("ouml");   // "�"
    addAllowedEscapeString("otilde"); // "�"
    addAllowedEscapeString("Uacute"); // "�"
    addAllowedEscapeString("Ugrave"); // "�"
    addAllowedEscapeString("Ucirc");  // "�"
    addAllowedEscapeString("Uuml");   // "�"
    addAllowedEscapeString("uacute"); // "�"
    addAllowedEscapeString("ugrave"); // "�"
    addAllowedEscapeString("ucirc");  // "�"
    addAllowedEscapeString("uuml");   // "�"
    addAllowedEscapeString("Yacute"); // "�"
    addAllowedEscapeString("yacute"); // "�"
    addAllowedEscapeString("yuml");   // "�"

    addAllowedEscapeString("deg");    // "�"
    addAllowedEscapeString("plusmn"); // "�"
    addAllowedEscapeString("sup2");   // "�"
    addAllowedEscapeString("sup3");   // "�"
    addAllowedEscapeString("sup1");   // "�"
    addAllowedEscapeString("nbsp");   // "�"
    addAllowedEscapeString("pound");  // "�"
    addAllowedEscapeString("cent");   // "�"
    addAllowedEscapeString("frac14"); // "�"
    addAllowedEscapeString("frac12"); // "�"
    addAllowedEscapeString("frac34"); // "�"
    addAllowedEscapeString("iquest"); // "�"
    addAllowedEscapeString("iexcl");  // "�"
    addAllowedEscapeString("ETH");    // "�"
    addAllowedEscapeString("eth");    // "�"
    addAllowedEscapeString("THORN");  // "�"
    addAllowedEscapeString("thorn");  // "�"
    addAllowedEscapeString("AElig");  // "�"
    addAllowedEscapeString("aelig");  // "�"
    addAllowedEscapeString("Oslash"); // "�"
    addAllowedEscapeString("curren"); // "�"
    addAllowedEscapeString("Ccedil"); // "�"
    addAllowedEscapeString("ccedil"); // "�"
    addAllowedEscapeString("szlig");  // "�"
    addAllowedEscapeString("Ntilde"); // "�"
    addAllowedEscapeString("ntilde"); // "�"
    addAllowedEscapeString("yen");    // "�"
    addAllowedEscapeString("not");    // "�"
    addAllowedEscapeString("ordf");   // "�"
    addAllowedEscapeString("uml");    // "�"
    addAllowedEscapeString("shy");    // "�"
    addAllowedEscapeString("macr");   // "�"

    addAllowedEscapeString("micro");  // "�"
    addAllowedEscapeString("middot"); // "�"
    addAllowedEscapeString("cedil");  // "�"
    addAllowedEscapeString("ordm");   // "�"
    addAllowedEscapeString("times");  // "�"
    addAllowedEscapeString("divide"); // "�"
    addAllowedEscapeString("oslash"); // "�"

    setTokenCaseSensitive(true);

    addTokenSubstitute("note", " <font color=\"#800000\"><small>(");
    addTokenSubstitute("/note", ")</small></font> ");
}

std::unique_ptr<BasicFilterUserData> ThMLHTML::createUserData(
        SWModule const * module,
        SWKey const * key)
{ return std::make_unique<MyUserData>(module, key); }

bool ThMLHTML::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
        MyUserData * u = static_cast<MyUserData *>(userData);
        XMLTag tag(token);
        if (tag.name() == "sync") {
            if (!tag.attribute("value").empty() && (tag.attribute("type") == "Strongs")) {
                auto value(tag.attribute("value"));
                if (!value.empty()) {
                    auto const firstChar(*(value.c_str()));
                    if (firstChar == 'H' || firstChar == 'G' || firstChar == 'A') {
                        value.erase(0u, 1u);
                        buf += "<small><em>";
                        buf += value;
                        buf += "</em></small>";
                    }
                    else if (firstChar == 'T') {
                        value.erase(0u, 2u);

                        buf += "<small><i>";
                        buf += value;
                        buf += "</i></small>";
                    }
                }
            }
            else if (!tag.attribute("value").empty() && (tag.attribute("type") == "morph")) {
                buf += "<small><em>";
                buf += tag.attribute("value");
                buf += "</em></small>";
            }
            else if (!tag.attribute("value").empty() && (tag.attribute("type") == "lemma")) {
                buf += "<small><em>(";
                buf += tag.attribute("value");
                buf += ")</em></small>";
            }
        }
        else if (tag.name() == "div") {
            if (tag.isEndTag() && (u->isSecHead)) {
                buf += "</i></b><br />";
                u->isSecHead = false;
            } else if (tag.attribute("class") == "sechead") {
                u->isSecHead = true;
                buf += "<br /><b><i>";
            } else if (tag.attribute("class") == "title") {
                u->isSecHead = true;
                buf += "<br /><b><i>";
            }
        }
        else if (tag.name() == "img") {
            const char *src = std::strstr(token, "src");
            if (!src)        // assert we have a src attribute
                return false;

            buf += '<';
            for (const char *c = token; *c; c++) {
                if (c == src) {
                    for (;((*c) && (*c != '"')); c++)
                        buf += *c;

                    if (!*c) { c--; continue; }

                    buf += '"';
                    if (*(c+1) == '/') {
                        buf += "file:";
                        buf += userData->module->getConfigEntry("AbsoluteDataPath");
                        if (buf[buf.length()-2] == '/')
                            c++;        // skip '/'
                    }
                    continue;
                }
                buf += *c;
            }
            buf += '>';
        }
        else if (tag.name() == "scripRef") { //do nothing with scrip refs, we leave them out

        }
        else {
            buf += '<';
            buf += token;
            buf += '>';

//            return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
