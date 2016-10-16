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
    addEscapeStringSubstitute("brvbar", "¦");
    addEscapeStringSubstitute("sect", "§");
    addEscapeStringSubstitute("copy", "©");
    addEscapeStringSubstitute("laquo", "«");
    addEscapeStringSubstitute("reg", "®");
    addEscapeStringSubstitute("acute", "´");
    addEscapeStringSubstitute("para", "¶");
    addEscapeStringSubstitute("raquo", "»");

    addEscapeStringSubstitute("Aacute", "Á");
    addEscapeStringSubstitute("Agrave", "À");
    addEscapeStringSubstitute("Acirc", "Â");
    addEscapeStringSubstitute("Auml", "Ä");
    addEscapeStringSubstitute("Atilde", "Ã");
    addEscapeStringSubstitute("Aring", "Å");
    addEscapeStringSubstitute("aacute", "á");
    addEscapeStringSubstitute("agrave", "à");
    addEscapeStringSubstitute("acirc", "â");
    addEscapeStringSubstitute("auml", "ä");
    addEscapeStringSubstitute("atilde", "ã");
    addEscapeStringSubstitute("aring", "å");
    addEscapeStringSubstitute("Eacute", "É");
    addEscapeStringSubstitute("Egrave", "È");
    addEscapeStringSubstitute("Ecirc", "Ê");
    addEscapeStringSubstitute("Euml", "Ë");
    addEscapeStringSubstitute("eacute", "é");
    addEscapeStringSubstitute("egrave", "è");
    addEscapeStringSubstitute("ecirc", "ê");
    addEscapeStringSubstitute("euml", "ë");
    addEscapeStringSubstitute("Iacute", "Í");
    addEscapeStringSubstitute("Igrave", "Ì");
    addEscapeStringSubstitute("Icirc", "Î");
    addEscapeStringSubstitute("Iuml", "Ï");
    addEscapeStringSubstitute("iacute", "í");
    addEscapeStringSubstitute("igrave", "ì");
    addEscapeStringSubstitute("icirc", "î");
    addEscapeStringSubstitute("iuml", "ï");
    addEscapeStringSubstitute("Oacute", "Ó");
    addEscapeStringSubstitute("Ograve", "Ò");
    addEscapeStringSubstitute("Ocirc", "Ô");
    addEscapeStringSubstitute("Ouml", "Ö");
    addEscapeStringSubstitute("Otilde", "Õ");
    addEscapeStringSubstitute("oacute", "ó");
    addEscapeStringSubstitute("ograve", "ò");
    addEscapeStringSubstitute("ocirc", "ô");
    addEscapeStringSubstitute("ouml", "ö");
    addEscapeStringSubstitute("otilde", "õ");
    addEscapeStringSubstitute("Uacute", "Ú");
    addEscapeStringSubstitute("Ugrave", "Ù");
    addEscapeStringSubstitute("Ucirc", "Û");
    addEscapeStringSubstitute("Uuml", "Ü");
    addEscapeStringSubstitute("uacute", "ú");
    addEscapeStringSubstitute("ugrave", "ù");
    addEscapeStringSubstitute("ucirc", "û");
    addEscapeStringSubstitute("uuml", "ü");
    addEscapeStringSubstitute("Yacute", "Ý");
    addEscapeStringSubstitute("yacute", "ý");
    addEscapeStringSubstitute("yuml", "ÿ");

    addEscapeStringSubstitute("deg", "°");
    addEscapeStringSubstitute("plusmn", "±");
    addEscapeStringSubstitute("sup2", "²");
    addEscapeStringSubstitute("sup3", "³");
    addEscapeStringSubstitute("sup1", "¹");
    addEscapeStringSubstitute("nbsp", "º");
    addEscapeStringSubstitute("pound", "£");
    addEscapeStringSubstitute("cent", "¢");
    addEscapeStringSubstitute("frac14", "¼");
    addEscapeStringSubstitute("frac12", "½");
    addEscapeStringSubstitute("frac34", "¾");
    addEscapeStringSubstitute("iquest", "¿");
    addEscapeStringSubstitute("iexcl", "¡");
    addEscapeStringSubstitute("ETH", "Ð");
    addEscapeStringSubstitute("eth", "ð");
    addEscapeStringSubstitute("THORN", "Þ");
    addEscapeStringSubstitute("thorn", "þ");
    addEscapeStringSubstitute("AElig", "Æ");
    addEscapeStringSubstitute("aelig", "æ");
    addEscapeStringSubstitute("Oslash", "Ø");
    addEscapeStringSubstitute("curren", "¤");
    addEscapeStringSubstitute("Ccedil", "Ç");
    addEscapeStringSubstitute("ccedil", "ç");
    addEscapeStringSubstitute("szlig", "ß");
    addEscapeStringSubstitute("Ntilde", "Ñ");
    addEscapeStringSubstitute("ntilde", "ñ");
    addEscapeStringSubstitute("yen", "¥");
    addEscapeStringSubstitute("not", "¬");
    addEscapeStringSubstitute("ordf", "ª");
    addEscapeStringSubstitute("uml", "¨");
    addEscapeStringSubstitute("shy", "­");
    addEscapeStringSubstitute("macr", "¯");

    addEscapeStringSubstitute("micro",  "µ");
    addEscapeStringSubstitute("middot", "·");
    addEscapeStringSubstitute("cedil",  "¸");
    addEscapeStringSubstitute("ordm",   "º");
    addEscapeStringSubstitute("times",  "×");
    addEscapeStringSubstitute("divide", "÷");
    addEscapeStringSubstitute("oslash", "ø");

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
        if (tag.getName() && !strcmp(tag.getName(), "sync")) {
            std::string value = tag.getAttribute("value");
            if (!tag.getAttribute("type").empty() && !strcmp(tag.getAttribute("type").c_str(), "morph")) { //&gt;
                buf += formatted(" {\\cf4 \\sub (%s)}", value.c_str());
            }
            else if(!tag.getAttribute("type").empty() && !strcmp(tag.getAttribute("type").c_str(), "Strongs")) {
                if (value[0] == 'H' || value[0] == 'G' || value[0] == 'A') {
                    value.erase(0u, 1u);
                    buf += formatted(" {\\cf3 \\sub <%s>}", value.c_str());
                }
                else if (value[0] == 'T') {
                    value.erase(0u, 1u);
                    buf += formatted(" {\\cf4 \\sub (%s)}", value.c_str());
                }
            }
            else if (!tag.getAttribute("type").empty() && !strcmp(tag.getAttribute("type").c_str(), "Dict")) {
                if (!tag.isEndTag())
                    buf += "{\\b ";
                else    buf += "}";
            }
        }
        // <note> tag
        else if (!strcmp(tag.getName(), "note")) {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    std::string type = tag.getAttribute("type");
                    std::string footnoteNumber = tag.getAttribute("swordFootnote");
                    if (VerseKey const * const vkey = dynamic_cast<VerseKey const *>(u->key)) {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        char ch = ((!tag.getAttribute("type").empty() && ((!strcmp(tag.getAttribute("type").c_str(), "crossReference")) || (!strcmp(tag.getAttribute("type").c_str(), "x-cross-ref")))) ? 'x':'n');
                        buf += formatted("{\\super <a href=\"\">*%c%i.%s</a>} ", ch, vkey->getVerse(), footnoteNumber.c_str());
                    }
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {
                u->suspendTextPassThru = false;
            }
        }


        else if (!strcmp(tag.getName(), "scripRef")) {
            if (!tag.isEndTag()) {
                if (!tag.isEmpty()) {
                    u->suspendTextPassThru = true;
                }
            }
            if (tag.isEndTag()) {    //    </scripRef>
                if (!u->BiblicalText) {
                    std::string refList = u->startTag.getAttribute("passage");
                    if (!refList.length())
                        refList = u->lastTextNode;
                    std::string version = tag.getAttribute("version");
                    buf += "<a href=\"\">";
                    buf += refList.c_str();
//                    buf += u->lastTextNode.c_str();
                    buf += "</a>";
                }
                else {
                    std::string footnoteNumber = u->startTag.getAttribute("swordFootnote");
                    if (VerseKey const * const vkey = dynamic_cast<VerseKey const *>(u->key)) {
                        // leave this special osis type in for crossReference notes types?  Might thml use this some day? Doesn't hurt.
                        buf += formatted("{\\super <a href=\"\">*x%i.%s</a>} ", vkey->getVerse(), footnoteNumber.c_str());
                    }
                }

                // let's let text resume to output again
                u->suspendTextPassThru = false;
            }
        }

        else if (tag.getName() && !strcmp(tag.getName(), "div")) {
            if (tag.isEndTag() && u->SecHead) {
                buf += "\\par}";
                u->SecHead = false;
            }
            else if (!tag.getAttribute("class").empty()) {
                if (!stricmp(tag.getAttribute("class").c_str(), "sechead")) {
                    u->SecHead = true;
                    buf += "{\\par\\i1\\b1 ";
                }
                else if (!stricmp(tag.getAttribute("class").c_str(), "title")) {
                    u->SecHead = true;
                    buf += "{\\par\\i1\\b1 ";
                }
            }
        }
        else if (tag.getName() && (!strcmp(tag.getName(), "img") || !strcmp(tag.getName(), "image"))) {
            auto const src(tag.getAttribute("src"));
            if (src.empty())        // assert we have a src attribute
                return false;

            char* filepath = new char[strlen(u->module->getConfigEntry("AbsoluteDataPath")) + strlen(token)];
            *filepath = 0;
            strcpy(filepath, userData->module->getConfigEntry("AbsoluteDataPath"));
            strcat(filepath, src.c_str());

// we do this because BibleCS looks for this EXACT format for an image tag
            buf+="<img src=\"";
            buf+=filepath;
            buf+="\" />";
            delete [] filepath;
        }
        else {
            return false;  // we still didn't handle token
        }
    }
    return true;
}


} /* namespace swordxx */
