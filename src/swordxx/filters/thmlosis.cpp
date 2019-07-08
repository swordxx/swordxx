/******************************************************************************
 *
 *  thmlosis.cpp -    filter to convert ThML to OSIS
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "thmlosis.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "../keys/versekey.h"
#include "../swlog.h"
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

ThMLOSIS::ThMLOSIS() {
}


ThMLOSIS::~ThMLOSIS() {
}


char ThMLOSIS::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    (void) module;
    char token[2048]; // cheese.  Fix.
    int tokpos = 0;
    bool intoken = false;
    bool ampersand = false;
    bool lastspace = false;
    char val[128];
    static auto const strstrip =
            [](char * const v) {
                std::string s(v);
                trimString(s);
                std::strcpy(v, s.c_str());
            };
    std::string buf;
    char *valto;
    char *ch;

    const char *wordStart = text.c_str();
    char const * wordEnd = nullptr;

    char const * textStart = nullptr;
    char const * textEnd = nullptr;

    bool suspendTextPassThru = false;
    bool handled = false;
    bool newText = false;
    bool newWord = false;

//     std::string tmp;
    std::string divEnd = "";

    std::string orig = text;
    const char* from = orig.c_str();

    text = "";
    for (from = orig.c_str(); *from; ++from) {

        // handle silly <variant word> items in greek whnu, remove when module is fixed
        if ((*from == '<') && (*(from+1) < 0)) {
            text += "&lt;";
            continue;
        }

        if (*from == '<') { //start of new token detected
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            ampersand = false;
            textEnd = from-1;
            wordEnd = text.c_str() + text.length();//not good, instead of wordEnd = to!

//             wordEnd = to;
            continue;
        }

        if (*from == '&') {
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            ampersand = true;
            continue;
        }

        if (*from == ';' && ampersand) {
            intoken = false;
            ampersand = false;

            if (*token == '#') {
                text += '&';
                text += token;
                text += ';';
            }
            else if (startsWith(token, "nbsp"sv)) text += ' ';
            else if (startsWith(token, "quot"sv)) text += '"';
            else if (startsWith(token, "amp"sv)) text += '&';
            else if (startsWith(token, "lt"sv)) text += '<';
            else if (startsWith(token, "gt"sv)) text += '>';
            else if (startsWith(token, "brvbar"sv)) text += '\xA6';
            else if (startsWith(token, "sect"sv)) text += '\xA7';
            else if (startsWith(token, "copy"sv)) text += '\xA9';
            else if (startsWith(token, "laquo"sv)) text += '\xAB';
            else if (startsWith(token, "reg"sv)) text += '\xAE';
            else if (startsWith(token, "acute"sv)) text += '\xB4';
            else if (startsWith(token, "para"sv)) text += '\xB6';
            else if (startsWith(token, "raquo"sv)) text += '\xBB';
            else if (startsWith(token, "Aacute"sv)) text += '\xC1';
            else if (startsWith(token, "Agrave"sv)) text += '\xC0';
            else if (startsWith(token, "Acirc"sv)) text += '\xC2';
            else if (startsWith(token, "Auml"sv)) text += '\xC4';
            else if (startsWith(token, "Atilde"sv)) text += '\xC3';
            else if (startsWith(token, "Aring"sv)) text += '\xC5';
            else if (startsWith(token, "aacute"sv)) text += '\xE1';
            else if (startsWith(token, "agrave"sv)) text += '\xE0';
            else if (startsWith(token, "acirc"sv)) text += '\xE2';
            else if (startsWith(token, "auml"sv)) text += '\xE4';
            else if (startsWith(token, "atilde"sv)) text += '\xE3';
            else if (startsWith(token, "aring"sv)) text += '\xE5';
            else if (startsWith(token, "Eacute"sv)) text += '\xC9';
            else if (startsWith(token, "Egrave"sv)) text += '\xC8';
            else if (startsWith(token, "Ecirc"sv)) text += '\xCA';
            else if (startsWith(token, "Euml"sv)) text += '\xCB';
            else if (startsWith(token, "eacute"sv)) text += '\xE9';
            else if (startsWith(token, "egrave"sv)) text += '\xE8';
            else if (startsWith(token, "ecirc"sv)) text += '\xEA';
            else if (startsWith(token, "euml"sv)) text += '\xEB';
            else if (startsWith(token, "Iacute"sv)) text += '\xCD';
            else if (startsWith(token, "Igrave"sv)) text += '\xCC';
            else if (startsWith(token, "Icirc"sv)) text += '\xCE';
            else if (startsWith(token, "Iuml"sv)) text += '\xCF';
            else if (startsWith(token, "iacute"sv)) text += '\xED';
            else if (startsWith(token, "igrave"sv)) text += '\xEC';
            else if (startsWith(token, "icirc"sv)) text += '\xEE';
            else if (startsWith(token, "iuml"sv)) text += '\xEF';
            else if (startsWith(token, "Oacute"sv)) text += '\xD3';
            else if (startsWith(token, "Ograve"sv)) text += '\xD2';
            else if (startsWith(token, "Ocirc"sv)) text += '\xD4';
            else if (startsWith(token, "Ouml"sv)) text += '\xD6';
            else if (startsWith(token, "Otilde"sv)) text += '\xD5';
            else if (startsWith(token, "oacute"sv)) text += '\xF3';
            else if (startsWith(token, "ograve"sv)) text += '\xF2';
            else if (startsWith(token, "ocirc"sv)) text += '\xF4';
            else if (startsWith(token, "ouml"sv)) text += '\xF6';
            else if (startsWith(token, "otilde"sv)) text += '\xF5';
            else if (startsWith(token, "Uacute"sv)) text += '\xDA';
            else if (startsWith(token, "Ugrave"sv)) text += '\xD9';
            else if (startsWith(token, "Ucirc"sv)) text += '\xDB';
            else if (startsWith(token, "Uuml"sv)) text += '\xDC';
            else if (startsWith(token, "uacute"sv)) text += '\xFA';
            else if (startsWith(token, "ugrave"sv)) text += '\xF9';
            else if (startsWith(token, "ucirc"sv)) text += '\xFB';
            else if (startsWith(token, "uuml"sv)) text += '\xFC';
            else if (startsWith(token, "Yacute"sv)) text += '\xDD';
            else if (startsWith(token, "yacute"sv)) text += '\xFD';
            else if (startsWith(token, "yuml"sv)) text += '\xFF';

            else if (startsWith(token, "deg"sv)) text += '\xB0';
            else if (startsWith(token, "plusmn"sv)) text += '\xB1';
            else if (startsWith(token, "sup2"sv)) text += '\xB2';
            else if (startsWith(token, "sup3"sv)) text += '\xB3';
            else if (startsWith(token, "sup1"sv)) text += '\xB9';
            else if (startsWith(token, "nbsp"sv)) text += '\xBA';
            else if (startsWith(token, "pound"sv)) text += '\xA3';
            else if (startsWith(token, "cent"sv)) text += '\xA2';
            else if (startsWith(token, "frac14"sv)) text += '\xBC';
            else if (startsWith(token, "frac12"sv)) text += '\xBD';
            else if (startsWith(token, "frac34"sv)) text += '\xBE';
            else if (startsWith(token, "iquest"sv)) text += '\xBF';
            else if (startsWith(token, "iexcl"sv)) text += '\xA1';
            else if (startsWith(token, "ETH"sv)) text += '\xD0';
            else if (startsWith(token, "eth"sv)) text += '\xF0';
            else if (startsWith(token, "THORN"sv)) text += '\xDE';
            else if (startsWith(token, "thorn"sv)) text += '\xFE';
            else if (startsWith(token, "AElig"sv)) text += '\xC6';
            else if (startsWith(token, "aelig"sv)) text += '\xE6';
            else if (startsWith(token, "Oslash"sv)) text += '\xD8';
            else if (startsWith(token, "curren"sv)) text += '\xA4';
            else if (startsWith(token, "Ccedil"sv)) text += '\xC7';
            else if (startsWith(token, "ccedil"sv)) text += '\xE7';
            else if (startsWith(token, "szlig"sv)) text += '\xDF';
            else if (startsWith(token, "Ntilde"sv)) text += '\xD1';
            else if (startsWith(token, "ntilde"sv)) text += '\xF1';
            else if (startsWith(token, "yen"sv)) text += '\xA5';
            else if (startsWith(token, "not"sv)) text += '\xAC';
            else if (startsWith(token, "ordf"sv)) text += '\xAA';
            else if (startsWith(token, "uml"sv)) text += '\xA8';
            else if (startsWith(token, "shy"sv)) text += '\xAD';
            else if (startsWith(token, "macr"sv)) text += '\xAF';
            else if (startsWith(token, "micro"sv)) text += "\xB5";
            else if (startsWith(token, "middot"sv)) text +="\xB7";
            else if (startsWith(token, "cedil"sv)) text += "\xB8";
            else if (startsWith(token, "ordm"sv)) text +=  "\xBA";
            else if (startsWith(token, "times"sv)) text += "\xD7";
            else if (startsWith(token, "divide"sv)) text +="\xF7";
            else if (startsWith(token, "oslash"sv)) text +="\xF8";
            continue;
        }

        // handle silly <variant word> items in greek whnu, remove when module is fixed
        if ((*from == '>') && (*(from-1) < 0)) {
            text += "&gt;";
            continue;
        }

        if (*from == '>') {    // process tokens
            intoken = false;
            suspendTextPassThru = false;
            newWord = true;
            handled = false;

            while (wordStart < (text.c_str() + text.length())) { //hack
                if (std::strchr(";,. :?!()'\"", *wordStart) && wordStart[0] && wordStart[1])
                    wordStart++;
                else break;
            }
            while (wordEnd > wordStart) {
                if (std::strchr(" ,;:.?!()'\"", *wordEnd))
                    wordEnd--;
                else break;
            }

            // variants
            if (startsWith(token, "div type=\"variant\""sv)) {
                XMLTag tag = token;
                text.append("<seg type=\"x-variant\"");
                std::string cls = "x-class:";
                cls += tag.attribute("class");
                if (cls.length()>8)
                    text += formatted(" subType=\"%s\"", cls);

                text += ">";
                divEnd = "</seg>";
                newText = true;
                lastspace = false;
                handled = true;
            }
            // section titles
            if (!std::strcmp(token, "div class=\"sechead\"")) {
//                 pushString(&to, "<title>");
                text.append("<title>");
                divEnd = "</title>";
                newText = true;
                lastspace = false;
                handled = true;
            }
            else    if (!std::strcmp(token, "/div")) {
                //pushString(&to, divEnd.c_str());
                text.append(divEnd);
                lastspace = false;
                handled = true;
            }
            // Scripture Reference
            if (startsWith(token, "scripRef"sv)) {
    //            pushString(buf, "<reference osisRef=\"");
                suspendTextPassThru = true;
                newText = true;
                handled = true;
            }
            else if (startsWith(token, "/scripRef"sv)) {
                std::string tmp;
                tmp = "";
                tmp.append(textStart, (int)(textEnd - textStart)+1);
                //pushString(&to, convertToOSIS(tmp, key));
                text.append(VerseKey::convertToOSIS(tmp, key));
                suspendTextPassThru = false;
                handled = true;
            }
//      Usage of italics to represent transChange isn't domaninant;
//        solution: mark in OSIS instead, assume no semantics other than emphasis
//                of italicized text
//                        if (!std::strcmp(module->Type(), "Biblical Texts")) {
//                    // Italics assume transchange for Biblical texts
//                    if (caseInsensitiveEquals(token, "i")) {
//                        pushString(&to, "<transChange type=\"added\">");
//                        newText = true;
//                        lastspace = false;
//                        handled = true;
//                    }
//                    else    if (caseInsensitiveEquals(token, "/i")) {
//                        pushString(&to, "</transChange>");
//                        lastspace = false;
//                        handled = true;
//                    }
//                        }
//                        else {
//                            // otherwise, italics are just italics
//-- end italics for transchange
                    if (caseInsensitiveEquals(token, "i")) {
//                         pushString(&to, "<hi type=\"i\">");
                    text.append("<hi type=\"i\">");
                        newText = true;
                     lastspace = false;
                        handled = true;
                    }
                    else    if (caseInsensitiveEquals(token, "/i")) {
//                         pushString(&to, "</hi>");
                    text.append("</hi>");
                        lastspace = false;
                        handled = true;
                    }
//                        }

                if (!std::strcmp(token, "b")) {
//                     pushString(&to, "<hi type=\"b\">");
                text.append("<hi type=\"b\">");
                newText = true;
                lastspace = false;
                    handled = true;
                }
            else if (!std::strcmp(token, "/b")) {
//                  pushString(&to, "</hi>");
                text.append("</hi>");
                    lastspace = false;
                    handled = true;
            }

            // Footnote
            if (startsWith(token, "note"sv)) {
                    //pushString(&to, "<note>");
                text.append("<note>");
                newText = true;
                lastspace = false;
                handled = true;
            }
            else    if (!std::strcmp(token, "/note")) {
                // pushString(&to, "</note>");
                text.append("</note>");
                lastspace = false;
                handled = true;
            }

            // Figure
            else if (startsWith(token, "img "sv)) {
                const char *src = std::strstr(token, "src");
                if (!src)        // assert we have a src attribute
                    continue;
//                    return false;

                //pushString(&to, "<figure src=\"");
                text.append("<figure src=\"");

                const char* end = std::strchr(src+2, '"'); //start search behind src="

                if (end) { //append the path
                    text.append(src+2, end - (src+2));
                }

//                 const char *c;
//                 for (c = src;((*c) && (*c != '"')); c++);

// uncomment for Sword++ absolute path logic
//                if (*(c+1) == '/') {
//                    pushString(buf, "file:");
//                    pushString(buf, module->getConfigEntry("AbsoluteDataPath"));
//                    if (*((*buf)-1) == '/')
//                        c++;        // skip '/'
//                }
//                end of uncomment for asolute path logic

//                 for (c++;((*c) && (*c != '"')); c++)
//                     *to++ = *c;

                //pushString(&to, "\" />");
                text.append("\" />");
                handled = true;
            }

            // Strongs numbers
            else    if (!strnicmp(token, "sync type=\"Strongs\" ", 20)) {    // Strongs
                valto = val;
                for (unsigned int i = 27; token[i] != '\"' && i < 150; i++)
                    *valto++ = token[i];
                *valto = 0;
                if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                    // normal strongs number
                    strstrip(val);

                    if (startsWith(wordStart, "<w "sv)) {
                        const char *attStart = std::strstr(wordStart, "lemma");
                        if (attStart) { //existing morph attribute, append this one to it
                            attStart += 7;
                            buf = formatted("strong:%s ", val);
                        }
                        else { // no lemma attribute
                            attStart = wordStart + 3;
                            buf = formatted("lemma=\"strong:%s\" ", val);
                        }

                        text.insert(attStart - text.c_str(), buf);
                    }
                    else { //wordStart doesn't point to an existing <w> attribute!
                        buf = formatted("<w lemma=\"strong:%s\">", val);
                        text.insert(wordStart - text.c_str(), buf);
                        text += "</w>";
                        lastspace = false;
                    }
                }
                // OLB verb morph, leave it out of OSIS tag
                else {
                }
                handled = true;
            }

            // Morphology
            else if (startsWith(token, "sync type=\"morph\""sv)) {
                std::string cls = "";
                std::string morph = "";
                for (ch = token+17; *ch; ch++) {
                    if (startsWith(ch, "class=\""sv)) {
                        valto = val;
                        for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                            *valto++ = ch[i];
                        *valto = 0;
                        strstrip(val);
                        cls = val;
                    }
                    if (startsWith(ch, "value=\""sv)) {
                        valto = val;
                        for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                            *valto++ = ch[i];
                        *valto = 0;
                        strstrip(val);
                        morph = val;
                    }
                }
                if (startsWith(wordStart, "<w "sv)) {
                    const char *attStart = std::strstr(wordStart, "morph");
                    if (attStart) { //existing morph attribute, append this one to it
                        attStart += 7;
                        buf = formatted("%s:%s ", (!cls.empty() ? cls.c_str() :"robinson"), morph);
                    }
                    else { // no lemma attribute
                        attStart = wordStart + 3;
                        buf = formatted("morph=\"%s:%s\" ", (!cls.empty() ? cls.c_str() :"robinson"), morph);
                    }

                    text.insert(attStart - text.c_str(), buf); //hack, we have to
                }
                else { //no existing <w> attribute fond
                    buf = formatted("<w morph=\"%s:%s\">", (!cls.empty() ? cls.c_str() : "robinson"), morph);
                    text.insert(wordStart - text.c_str(), buf);
                    text += "</w>";
                    lastspace = false;

                }
                handled = true;
            }

            if (!handled) {
                SWLog::getSystemLog()->logError(
                        "Unprocessed Token: <%s> in key %s",
                        token,
                        key ? key->getText().c_str() : "<unknown>");
//                    std::exit(-1);
            }
            if (from[1] && std::strchr(" ,;.:?!()'\"", from[1])) {
                if (lastspace) {
                    text.pop_back();
                }
            }
            if (newText) {
                textStart = from+1;
                newText = false;
            }
            continue;
        }
        if (intoken) {
            if ((tokpos < 2045) && ((*from != 10)&&(*from != 13))) {
                token[tokpos++] = *from;
                token[tokpos+2] = 0;
            }
        }
        else    {
            switch (*from) {
            case '\'':
            case '\"':
            case '`':
                text += *from;
                //from++; //this line removes chars after an apostrophe! Needs fixing.
                break;
            default:
                if (newWord && (*from != ' ')) {
                    wordStart = text.c_str() + text.length();
                    newWord = false;

                    //fix this if required?
                    //std::memset(to, 0, 10);

                }

                if (!suspendTextPassThru) {
                    text += (*from);
                    lastspace = (*from == ' ');
                }
            }
        }
    }

    if (VerseKey const * const vkey = dynamic_cast<VerseKey const *>(key)) {
        std::string ref;
        if (vkey->getVerse())
            ref.append("\t\t<verse osisID=\"").append(vkey->getOSISRef())
               .append("\">");

        if (ref.length() > 0) {

            text = ref + text;

            if (vkey->getVerse()) {
                std::unique_ptr<VerseKey> tmp(
                            static_cast<VerseKey *>(vkey->clone().release()));
                *tmp = *vkey;
                tmp->setAutoNormalize(false);
                tmp->setIntros(true);

                text += "</verse>";

                tmp->positionToMaxVerse();
                if (*vkey == *tmp) {
                    tmp->setVerse(0);
//                    sprintf(ref, "\t</div>");
//                    pushString(&to, ref);
                    tmp->positionToMaxChapter();
                    tmp->positionToMaxVerse();
                    if (*vkey == *tmp) {
                        tmp->setChapter(0);
                        tmp->setVerse(0);
//                        sprintf(ref, "\t</div>");
//                        pushString(&to, ref);
                    }
                }
            }
//            else if (vkey->getChapter()) {
//                sprintf(ref, "\t<div type=\"chapter\" osisID=\"%s\">", vkey->getOSISRef());
//            }
//            else sprintf(ref, "\t<div type=\"book\" osisID=\"%s\">", vkey->getOSISRef());
        }
    }
    return 0;
}

} /* namespace swordxx */
