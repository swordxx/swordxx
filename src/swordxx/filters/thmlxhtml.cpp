/******************************************************************************
 *
 *  thmlxhtml.cpp -    ThML to classed XHTML
 *
 * $Id$
 *
 * Copyright 2011-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "thmlxhtml.h"

#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../url.h"
#include "../utilstr.h"


namespace swordxx {


const char *ThMLXHTML::getHeader() const {
    return "\
    ";
}


ThMLXHTML::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    if (module) {
        version = module->getName();
        BiblicalText = (module->getType() == "Biblical Texts");
        SecHead = false;
    }
}


ThMLXHTML::ThMLXHTML() {
    setTokenStart("<");
    setTokenEnd(">");

    setEscapeStart("&");
    setEscapeEnd(";");

    setEscapeStringCaseSensitive(true);
    setPassThruNumericEscapeString(true);

    addAllowedEscapeString("quot");
    addAllowedEscapeString("amp");
    addAllowedEscapeString("lt");
    addAllowedEscapeString("gt");

    addAllowedEscapeString("nbsp");
    addAllowedEscapeString("brvbar"); // "Š"
    addAllowedEscapeString("sect");   // "§"
    addAllowedEscapeString("copy");   // "©"
    addAllowedEscapeString("laquo");  // "«"
    addAllowedEscapeString("reg");    // "®"
    addAllowedEscapeString("acute");  // "Ž"
    addAllowedEscapeString("para");   // "¶"
    addAllowedEscapeString("raquo");  // "»"

    addAllowedEscapeString("Aacute"); // "Á"
    addAllowedEscapeString("Agrave"); // "À"
    addAllowedEscapeString("Acirc");  // "Â"
    addAllowedEscapeString("Auml");   // "Ä"
    addAllowedEscapeString("Atilde"); // "Ã"
    addAllowedEscapeString("Aring");  // "Å"
    addAllowedEscapeString("aacute"); // "á"
    addAllowedEscapeString("agrave"); // "à"
    addAllowedEscapeString("acirc");  // "â"
    addAllowedEscapeString("auml");   // "ä"
    addAllowedEscapeString("atilde"); // "ã"
    addAllowedEscapeString("aring");  // "å"
    addAllowedEscapeString("Eacute"); // "É"
    addAllowedEscapeString("Egrave"); // "È"
    addAllowedEscapeString("Ecirc");  // "Ê"
    addAllowedEscapeString("Euml");   // "Ë"
    addAllowedEscapeString("eacute"); // "é"
    addAllowedEscapeString("egrave"); // "è"
    addAllowedEscapeString("ecirc");  // "ê"
    addAllowedEscapeString("euml");   // "ë"
    addAllowedEscapeString("Iacute"); // "Í"
    addAllowedEscapeString("Igrave"); // "Ì"
    addAllowedEscapeString("Icirc");  // "Î"
    addAllowedEscapeString("Iuml");   // "Ï"
    addAllowedEscapeString("iacute"); // "í"
    addAllowedEscapeString("igrave"); // "ì"
    addAllowedEscapeString("icirc");  // "î"
    addAllowedEscapeString("iuml");   // "ï"
    addAllowedEscapeString("Oacute"); // "Ó"
    addAllowedEscapeString("Ograve"); // "Ò"
    addAllowedEscapeString("Ocirc");  // "Ô"
    addAllowedEscapeString("Ouml");   // "Ö"
    addAllowedEscapeString("Otilde"); // "Õ"
    addAllowedEscapeString("oacute"); // "ó"
    addAllowedEscapeString("ograve"); // "ò"
    addAllowedEscapeString("ocirc");  // "ô"
    addAllowedEscapeString("ouml");   // "ö"
    addAllowedEscapeString("otilde"); // "õ"
    addAllowedEscapeString("Uacute"); // "Ú"
    addAllowedEscapeString("Ugrave"); // "Ù"
    addAllowedEscapeString("Ucirc");  // "Û"
    addAllowedEscapeString("Uuml");   // "Ü"
    addAllowedEscapeString("uacute"); // "ú"
    addAllowedEscapeString("ugrave"); // "ù"
    addAllowedEscapeString("ucirc");  // "û"
    addAllowedEscapeString("uuml");   // "ü"
    addAllowedEscapeString("Yacute"); // "Ý"
    addAllowedEscapeString("yacute"); // "ý"
    addAllowedEscapeString("yuml");   // "ÿ"

    addAllowedEscapeString("deg");    // "°"
    addAllowedEscapeString("plusmn"); // "±"
    addAllowedEscapeString("sup2");   // "²"
    addAllowedEscapeString("sup3");   // "³"
    addAllowedEscapeString("sup1");   // "¹"
    addAllowedEscapeString("nbsp");   // "º"
    addAllowedEscapeString("pound");  // "£"
    addAllowedEscapeString("cent");   // "¢"
    addAllowedEscapeString("frac14"); // "Œ"
    addAllowedEscapeString("frac12"); // "œ"
    addAllowedEscapeString("frac34"); // "Ÿ"
    addAllowedEscapeString("iquest"); // "¿"
    addAllowedEscapeString("iexcl");  // "¡"
    addAllowedEscapeString("ETH");    // "Ð"
    addAllowedEscapeString("eth");    // "ð"
    addAllowedEscapeString("THORN");  // "Þ"
    addAllowedEscapeString("thorn");  // "þ"
    addAllowedEscapeString("AElig");  // "Æ"
    addAllowedEscapeString("aelig");  // "æ"
    addAllowedEscapeString("Oslash"); // "Ø"
    addAllowedEscapeString("curren"); // "€"
    addAllowedEscapeString("Ccedil"); // "Ç"
    addAllowedEscapeString("ccedil"); // "ç"
    addAllowedEscapeString("szlig");  // "ß"
    addAllowedEscapeString("Ntilde"); // "Ñ"
    addAllowedEscapeString("ntilde"); // "ñ"
    addAllowedEscapeString("yen");    // "¥"
    addAllowedEscapeString("not");    // "¬"
    addAllowedEscapeString("ordf");   // "ª"
    addAllowedEscapeString("uml");    // "š"
    addAllowedEscapeString("shy");    // "­"
    addAllowedEscapeString("macr");   // "¯"

    addAllowedEscapeString("micro");  // "µ"
    addAllowedEscapeString("middot"); // "·"
    addAllowedEscapeString("cedil");  // "ž"
    addAllowedEscapeString("ordm");   // "º"
    addAllowedEscapeString("times");  // "×"
    addAllowedEscapeString("divide"); // "÷"
    addAllowedEscapeString("oslash"); // "ø"

    setTokenCaseSensitive(true);
//    addTokenSubstitute("scripture", "<i> ");
    addTokenSubstitute("/scripture", "</i> ");

    renderNoteNumbers = false;
}


bool ThMLXHTML::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
        MyUserData *u = (MyUserData *)userData;

        XMLTag tag(token);
        if ((!tag.isEndTag()) && (!tag.isEmpty()))
            u->startTag = tag;

        if (tag.name() == "sync") {
            std::string value = tag.attribute("value");
            if (tag.attribute("type") == "morph") { //&gt;
                if(value.length())
                    buf += formatted("<small><em class=\"morph\">(<a href=\"passagestudy.jsp?action=showMorph&type=Greek&value=%s\" class=\"morph\">%s</a>)</em></small>",
                        URL::encode(value.c_str()).c_str(),
                        value.c_str());
            }
            else if (tag.attribute("type") == "lemma") { //&gt;
                if(value.length())
                    // empty "type=" is deliberate.
                    buf += formatted("<small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=&value=%s\" class=\"strongs\">%s</a>&gt;</em></small>",
                        URL::encode(value.c_str()).c_str(),
                        value.c_str());
            }
            else if (tag.attribute("type") == "Strongs") {
                char const ch = *value.begin();
                value.erase(0u, 1u);
                buf += formatted("<small><em class=\"strongs\">&lt;<a href=\"passagestudy.jsp?action=showStrongs&type=%s&value=%s\" class=\"strongs\">",
                            ((ch == 'H') ? "Hebrew" : "Greek"),
                            URL::encode(value.c_str()).c_str());
                buf += (value.length()) ? value.c_str() : "";
                buf += "</a>&gt;</em></small>";
            }
            else if (tag.attribute("type") == "Dict") {
                buf += (tag.isEndTag() ? "</b>" : "<b>");
            }

        }
        // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    std::string type = tag.attribute("type");
                    std::string footnoteNumber = tag.attribute("swordFootnote");
                    std::string noteName = tag.attribute("n");
                    if (VerseKey const * const vkey =
                            dynamic_cast<VerseKey const *>(u->key))
                    {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x' : 'n';
                        buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup class=\"%c\">*%c%s</sup></small></a>",
                            ch,
                            URL::encode(footnoteNumber.c_str()).c_str(),
                            URL::encode(u->version.c_str()).c_str(),
                            URL::encode(vkey->getText()).c_str(),
                            ch,
                            ch,
                            (renderNoteNumbers ? noteName.c_str() : ""));
                    }
                    else {
                        char const ch = ((tag.attribute("type") == "crossReference") || (tag.attribute("type") == "x-cross-ref")) ? 'x' : 'n';
                        buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=%c&value=%s&module=%s&passage=%s\"><small><sup class=\"%c\">*%c%s</sup></small></a>",
                            ch,
                            URL::encode(footnoteNumber.c_str()).c_str(),
                            URL::encode(u->version.c_str()).c_str(),
                            URL::encode(u->key->getText()).c_str(),
                            ch,
                            ch,
                            (renderNoteNumbers ? noteName.c_str() : ""));
                    }
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {
                u->suspendTextPassThru = false;
            }
        }
        else if (tag.name() == "scripture") {
            buf += (tag.isEndTag() ? "</i>" : "<i>");
        }
        // <scripRef> tag
        else if (tag.name() == "scripRef") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {    //    </scripRef>
                if (!u->BiblicalText) {
                    std::string refList = u->startTag.attribute("passage");
                    if (!refList.length())
                        refList = u->lastTextNode;
                    std::string version = tag.attribute("version");

                    buf += formatted("<a href=\"passagestudy.jsp?action=showRef&type=scripRef&value=%s&module=%s\">",
                        (refList.length()) ? URL::encode(refList.c_str()).c_str() : "",
                        (version.length()) ? URL::encode(version.c_str()).c_str() : "");
                    buf += u->lastTextNode.c_str();
                    buf += "</a>";
                }
                else {
                    std::string footnoteNumber = u->startTag.attribute("swordFootnote");
                    std::string noteName = tag.attribute("n");
                    if (VerseKey const * const vkey =
                            dynamic_cast<VerseKey const *>(u->key))
                    {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        //buf.appendFormatted("<a href=\"noteID=%s.x.%s\"><small><sup>*x</sup></small></a> ", vkey->getText(), footnoteNumber.c_str());
                        buf += formatted("<a href=\"passagestudy.jsp?action=showNote&type=x&value=%s&module=%s&passage=%s\"><small><sup class=\"x\">*x%s</sup></small></a>",
                            URL::encode(footnoteNumber.c_str()).c_str(),
                            URL::encode(u->version.c_str()).c_str(),
                            URL::encode(vkey->getText()).c_str(),
                            (renderNoteNumbers ? noteName.c_str() : ""));
                    }
                }

                // let's let text resume to output again
                u->suspendTextPassThru = false;
            }
        }
        else if (tag.name() == "div") {
            if (tag.isEndTag() && u->SecHead) {
                buf += "</h";
                buf += u->SecHead;
                buf += ">";
                u->SecHead = false;
            }
            else if (!tag.attribute("class").empty()) {
                if (!stricmp(tag.attribute("class").c_str(), "sechead")) {
                    u->SecHead = '3';
                    buf += "<h3>";
                }
                else if (!stricmp(tag.attribute("class").c_str(), "title")) {
                    u->SecHead = '2';
                    buf += "<h2>";
                }
                else {
                    buf += tag.toString();
                }
            }
            else {
                buf += tag.toString();
            }
        }
        else if (((tag.name() == "img") || (tag.name() == "image"))) {
            const char *src = strstr(token, "src");
            if (!src)        // assert we have a src attribute
                return false;

            const char *c, *d;
            if (((c = strchr(src + 3u, '"')) == nullptr) ||
                ((d = strchr(++c , '"')) == nullptr))    // identify endpoints.
                return false;            // abandon hope.

            std::string imagename = "file:";
            if (*c == '/')                // as below, inside for loop.
                imagename += userData->module->getConfigEntry("AbsoluteDataPath");
            while (c != d)                // move bits into the name.
                imagename += *(c++);

            // images become clickable, if the UI supports showImage.
            buf += formatted("<a href=\"passagestudy.jsp?action=showImage&value=%s&module=%s\"><",
                        URL::encode(imagename.c_str()).c_str(),
                        URL::encode(u->version.c_str()).c_str());

            for (c = token; *c; c++) {
                if ((*c == '/') && (*(c+1) == '\0'))
                    continue;
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
               buf += " border=0 /></a>";
        }
        else {
            buf += '<';
            /*for (const char *tok = token; *tok; tok++)
                buf += *tok;*/
            buf += token;
            buf += '>';
            //return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
