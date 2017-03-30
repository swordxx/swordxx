/***************************************************************************
 *
 *  thmlrtf.cpp -    ThML to RTF filter
 *
 * $Id$
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

#include "thmlrtf.h"

#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilstr.h"


namespace swordxx {

ThMLRTF::ThMLRTF() {
    setTokenStart("<");
    setTokenEnd(">");

    setEscapeStart("&");
    setEscapeEnd(";");

    setEscapeStringCaseSensitive(true);

     addEscapeStringSubstitute("nbsp", "\302\240");
    addEscapeStringSubstitute("apos", "'");
    addEscapeStringSubstitute("quot", "\"");
    addEscapeStringSubstitute("amp", "&");
    addEscapeStringSubstitute("lt", "<");
    addEscapeStringSubstitute("gt", ">");
    addEscapeStringSubstitute("brvbar", "\xA6");
    addEscapeStringSubstitute("sect", "\xA7");
    addEscapeStringSubstitute("copy", "\xA9");
    addEscapeStringSubstitute("laquo", "\xAB");
    addEscapeStringSubstitute("reg", "\xAE");
    addEscapeStringSubstitute("acute", "\xB4");
    addEscapeStringSubstitute("para", "\xB6");
    addEscapeStringSubstitute("raquo", "\xBB");

    addEscapeStringSubstitute("Aacute", "\xC1");
    addEscapeStringSubstitute("Agrave", "\xC0");
    addEscapeStringSubstitute("Acirc", "\xC2");
    addEscapeStringSubstitute("Auml", "\xC4");
    addEscapeStringSubstitute("Atilde", "\xC3");
    addEscapeStringSubstitute("Aring", "\xC5");
    addEscapeStringSubstitute("aacute", "\xE1");
    addEscapeStringSubstitute("agrave", "\xE0");
    addEscapeStringSubstitute("acirc", "\xE2");
    addEscapeStringSubstitute("auml", "\xE4");
    addEscapeStringSubstitute("atilde", "\xE3");
    addEscapeStringSubstitute("aring", "\xE5");
    addEscapeStringSubstitute("Eacute", "\xC9");
    addEscapeStringSubstitute("Egrave", "\xC8");
    addEscapeStringSubstitute("Ecirc", "\xCA");
    addEscapeStringSubstitute("Euml", "\xCB");
    addEscapeStringSubstitute("eacute", "\xE9");
    addEscapeStringSubstitute("egrave", "\xE8");
    addEscapeStringSubstitute("ecirc", "\xEA");
    addEscapeStringSubstitute("euml", "\xEB");
    addEscapeStringSubstitute("Iacute", "\xCD");
    addEscapeStringSubstitute("Igrave", "\xCC");
    addEscapeStringSubstitute("Icirc", "\xCE");
    addEscapeStringSubstitute("Iuml", "\xCF");
    addEscapeStringSubstitute("iacute", "\xED");
    addEscapeStringSubstitute("igrave", "\xEC");
    addEscapeStringSubstitute("icirc", "\xEE");
    addEscapeStringSubstitute("iuml", "\xEF");
    addEscapeStringSubstitute("Oacute", "\xD3");
    addEscapeStringSubstitute("Ograve", "\xD2");
    addEscapeStringSubstitute("Ocirc", "\xD4");
    addEscapeStringSubstitute("Ouml", "\xD6");
    addEscapeStringSubstitute("Otilde", "\xD5");
    addEscapeStringSubstitute("oacute", "\xF3");
    addEscapeStringSubstitute("ograve", "\xF2");
    addEscapeStringSubstitute("ocirc", "\xF4");
    addEscapeStringSubstitute("ouml", "\xF6");
    addEscapeStringSubstitute("otilde", "\xF5");
    addEscapeStringSubstitute("Uacute", "\xDA");
    addEscapeStringSubstitute("Ugrave", "\xD9");
    addEscapeStringSubstitute("Ucirc", "\xDB");
    addEscapeStringSubstitute("Uuml", "\xDC");
    addEscapeStringSubstitute("uacute", "\xFA");
    addEscapeStringSubstitute("ugrave", "\xF9");
    addEscapeStringSubstitute("ucirc", "\xFB");
    addEscapeStringSubstitute("uuml", "\xFC");
    addEscapeStringSubstitute("Yacute", "\xDD");
    addEscapeStringSubstitute("yacute", "\xFD");
    addEscapeStringSubstitute("yuml", "\xFF");

    addEscapeStringSubstitute("deg", "\xB0");
    addEscapeStringSubstitute("plusmn", "\xB1");
    addEscapeStringSubstitute("sup2", "\xB2");
    addEscapeStringSubstitute("sup3", "\xB3");
    addEscapeStringSubstitute("sup1", "\xB9");
    addEscapeStringSubstitute("nbsp", "\xBA");
    addEscapeStringSubstitute("pound", "\xA3");
    addEscapeStringSubstitute("cent", "\xA2");
    addEscapeStringSubstitute("frac14", "\xBC");
    addEscapeStringSubstitute("frac12", "\xBD");
    addEscapeStringSubstitute("frac34", "\xBE");
    addEscapeStringSubstitute("iquest", "\xBF");
    addEscapeStringSubstitute("iexcl", "\xA1");
    addEscapeStringSubstitute("ETH", "\xD0");
    addEscapeStringSubstitute("eth", "\xF0");
    addEscapeStringSubstitute("THORN", "\xDE");
    addEscapeStringSubstitute("thorn", "\xFE");
    addEscapeStringSubstitute("AElig", "\xC6");
    addEscapeStringSubstitute("aelig", "\xE6");
    addEscapeStringSubstitute("Oslash", "\xD8");
    addEscapeStringSubstitute("curren", "\xA4");
    addEscapeStringSubstitute("Ccedil", "\xC7");
    addEscapeStringSubstitute("ccedil", "\xE7");
    addEscapeStringSubstitute("szlig", "\xDF");
    addEscapeStringSubstitute("Ntilde", "\xD1");
    addEscapeStringSubstitute("ntilde", "\xF1");
    addEscapeStringSubstitute("yen", "\xA5");
    addEscapeStringSubstitute("not", "\xAC");
    addEscapeStringSubstitute("ordf", "\xAA");
    addEscapeStringSubstitute("uml", "\xA8");
    addEscapeStringSubstitute("shy", "\xAD");
    addEscapeStringSubstitute("macr", "\xAF");

    addEscapeStringSubstitute("micro",  "\xB5");
    addEscapeStringSubstitute("middot", "\xB7");
    addEscapeStringSubstitute("cedil",  "\xB8");
    addEscapeStringSubstitute("ordm",   "\xBA");
    addEscapeStringSubstitute("times",  "\xD7");
    addEscapeStringSubstitute("divide", "\xF7");
    addEscapeStringSubstitute("oslash", "\xF8");

    setTokenCaseSensitive(true);


    addTokenSubstitute("br", "\\line ");
    addTokenSubstitute("br /", "\\line ");
    addTokenSubstitute("i", "{\\i1 ");
    addTokenSubstitute("/i", "}");
    addTokenSubstitute("b", "{\\b1 ");
    addTokenSubstitute("/b", "}");
    addTokenSubstitute("p", "{\\fi200\\par}");
    addTokenSubstitute("p /", "\\pard\\par\\par ");

    //we need uppercase forms for the moment to support a few early ThML modules that aren't XHTML compliant
    addTokenSubstitute("BR", "\\line ");
    addTokenSubstitute("I", "{\\i1 ");
    addTokenSubstitute("/I", "}");
    addTokenSubstitute("B", "{\\b1 ");
    addTokenSubstitute("/B", "}");
    addTokenSubstitute("P", "\\par ");
    addTokenSubstitute("scripture", "{\\i1 ");
    addTokenSubstitute("/scripture", "}");
     addTokenSubstitute("center", "\\qc ");
     addTokenSubstitute("/center", "\\pard ");
}


char ThMLRTF::processText(std::string &text, const SWKey *key, const SWModule *module) {

    // preprocess text buffer to escape RTF control codes
    const char *from;
    std::string orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++) {  //loop to remove extra spaces
        switch (*from) {
        case '{':
        case '}':
        case '\\':
            text += "\\";
            text += *from;
            break;
        default:
            text += *from;
        }
    }
    text += (char)0;

    SWBasicFilter::processText(text, key, module);  //handle tokens as usual

    orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++) {  //loop to remove extra spaces
        if ((strchr(" \t\n\r", *from))) {
            while (*(from+1) && (strchr(" \t\n\r", *(from+1)))) {
                from++;
            }
            text += " ";
        }
        else {
            text += *from;
        }
    }
    text += (char)0;    // probably not needed, but don't want to remove without investigating (same as above)
    return 0;
}


ThMLRTF::MyUserData::MyUserData(const SWModule *module, const SWKey *key) : BasicFilterUserData(module, key) {
    this->SecHead = false;
    XMLTag startTag = "";
    if (module) {
        version = module->getName();
        BiblicalText = (module->getType() == "Biblical Texts");
    }
}


bool ThMLRTF::handleToken(std::string &buf, const char *token, BasicFilterUserData *userData) {
    if (!substituteToken(buf, token)) { // manually process if it wasn't a simple substitution
        MyUserData *u = (MyUserData *)userData;
        XMLTag tag(token);
        if ((!tag.isEndTag()) && (!tag.isEmpty()))
            u->startTag = tag;
        if (tag.name() == "sync") {
            std::string value = tag.attribute("value");
            if (!tag.attribute("type").empty() && !strcmp(tag.attribute("type").c_str(), "morph")) { //&gt;
                buf += formatted(" {\\cf4 \\sub (%s)}", value.c_str());
            }
            else if(!tag.attribute("type").empty() && !strcmp(tag.attribute("type").c_str(), "Strongs")) {
                if (value[0] == 'H' || value[0] == 'G' || value[0] == 'A') {
                    value.erase(0u, 1u);
                    buf += formatted(" {\\cf3 \\sub <%s>}", value.c_str());
                }
                else if (value[0] == 'T') {
                    value.erase(0u, 1u);
                    buf += formatted(" {\\cf4 \\sub (%s)}", value.c_str());
                }
            }
            else if (!tag.attribute("type").empty() && !strcmp(tag.attribute("type").c_str(), "Dict")) {
                if (!tag.isEndTag())
                    buf += "{\\b ";
                else    buf += "}";
            }
        }
        // <note> tag
        else if (tag.name() == "note") {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    std::string type = tag.attribute("type");
                    std::string footnoteNumber = tag.attribute("swordFootnote");
                    if (VerseKey const * const vkey = dynamic_cast<VerseKey const *>(u->key)) {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        char ch = ((!tag.attribute("type").empty() && ((!strcmp(tag.attribute("type").c_str(), "crossReference")) || (!strcmp(tag.attribute("type").c_str(), "x-cross-ref")))) ? 'x':'n');
                        buf += formatted("{\\super <a href=\"\">*%c%i.%s</a>} ", ch, vkey->getVerse(), footnoteNumber.c_str());
                    }
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {
                u->suspendTextPassThru = false;
            }
        }


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
                    buf += "<a href=\"\">";
                    buf += refList.c_str();
//                    buf += u->lastTextNode.c_str();
                    buf += "</a>";
                }
                else {
                    std::string footnoteNumber = u->startTag.attribute("swordFootnote");
                    if (VerseKey const * const vkey = dynamic_cast<VerseKey const *>(u->key)) {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        buf += formatted("{\\super <a href=\"\">*x%i.%s</a>} ", vkey->getVerse(), footnoteNumber.c_str());
                    }
                }

                // let's let text resume to output again
                u->suspendTextPassThru = false;
            }
        }

        else if (tag.name() == "div") {
            if (tag.isEndTag() && u->SecHead) {
                buf += "\\par}";
                u->SecHead = false;
            }
            else if (!tag.attribute("class").empty()) {
                if (!stricmp(tag.attribute("class").c_str(), "sechead")) {
                    u->SecHead = true;
                    buf += "{\\par\\i1\\b1 ";
                }
                else if (!stricmp(tag.attribute("class").c_str(), "title")) {
                    u->SecHead = true;
                    buf += "{\\par\\i1\\b1 ";
                }
            }
        }
        else if ((tag.name() == "img") || (tag.name() == "image")) {
            auto const src(tag.attribute("src"));
            if (src.empty())        // assert we have a src attribute
                return false;

            char const * const absoluteDataPath =
                    userData->module->getConfigEntry("AbsoluteDataPath");
            auto const filepath(
                        std::string(absoluteDataPath ? absoluteDataPath : "")
                        + src);

// we do this because BibleCS looks for this EXACT format for an image tag
            buf+="<img src=\"";
            buf+=filepath;
            buf+="\" />";
        }
        else {
            return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
