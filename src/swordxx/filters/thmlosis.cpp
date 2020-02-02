/******************************************************************************
 *
 *  thmlosis.cpp -    filter to convert ThML to OSIS
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
#include <cstring>
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

    std::string out;

    for (auto const * from = text.c_str(); *from; ++from) {

        // handle silly <variant word> items in greek whnu, remove when module is fixed
        if ((*from == '<') && (*(from+1) < 0)) {
            out += "&lt;";
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
            wordEnd = out.c_str() + out.size();//not good, instead of wordEnd = to!

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
                out += '&';
                out += token;
                out += ';';
            }
            else if (startsWith(token, "nbsp"sv)) out += ' ';
            else if (startsWith(token, "quot"sv)) out += '"';
            else if (startsWith(token, "amp"sv)) out += '&';
            else if (startsWith(token, "lt"sv)) out += '<';
            else if (startsWith(token, "gt"sv)) out += '>';
            else if (startsWith(token, "brvbar"sv)) out += '\xA6';
            else if (startsWith(token, "sect"sv)) out += '\xA7';
            else if (startsWith(token, "copy"sv)) out += '\xA9';
            else if (startsWith(token, "laquo"sv)) out += '\xAB';
            else if (startsWith(token, "reg"sv)) out += '\xAE';
            else if (startsWith(token, "acute"sv)) out += '\xB4';
            else if (startsWith(token, "para"sv)) out += '\xB6';
            else if (startsWith(token, "raquo"sv)) out += '\xBB';
            else if (startsWith(token, "Aacute"sv)) out += '\xC1';
            else if (startsWith(token, "Agrave"sv)) out += '\xC0';
            else if (startsWith(token, "Acirc"sv)) out += '\xC2';
            else if (startsWith(token, "Auml"sv)) out += '\xC4';
            else if (startsWith(token, "Atilde"sv)) out += '\xC3';
            else if (startsWith(token, "Aring"sv)) out += '\xC5';
            else if (startsWith(token, "aacute"sv)) out += '\xE1';
            else if (startsWith(token, "agrave"sv)) out += '\xE0';
            else if (startsWith(token, "acirc"sv)) out += '\xE2';
            else if (startsWith(token, "auml"sv)) out += '\xE4';
            else if (startsWith(token, "atilde"sv)) out += '\xE3';
            else if (startsWith(token, "aring"sv)) out += '\xE5';
            else if (startsWith(token, "Eacute"sv)) out += '\xC9';
            else if (startsWith(token, "Egrave"sv)) out += '\xC8';
            else if (startsWith(token, "Ecirc"sv)) out += '\xCA';
            else if (startsWith(token, "Euml"sv)) out += '\xCB';
            else if (startsWith(token, "eacute"sv)) out += '\xE9';
            else if (startsWith(token, "egrave"sv)) out += '\xE8';
            else if (startsWith(token, "ecirc"sv)) out += '\xEA';
            else if (startsWith(token, "euml"sv)) out += '\xEB';
            else if (startsWith(token, "Iacute"sv)) out += '\xCD';
            else if (startsWith(token, "Igrave"sv)) out += '\xCC';
            else if (startsWith(token, "Icirc"sv)) out += '\xCE';
            else if (startsWith(token, "Iuml"sv)) out += '\xCF';
            else if (startsWith(token, "iacute"sv)) out += '\xED';
            else if (startsWith(token, "igrave"sv)) out += '\xEC';
            else if (startsWith(token, "icirc"sv)) out += '\xEE';
            else if (startsWith(token, "iuml"sv)) out += '\xEF';
            else if (startsWith(token, "Oacute"sv)) out += '\xD3';
            else if (startsWith(token, "Ograve"sv)) out += '\xD2';
            else if (startsWith(token, "Ocirc"sv)) out += '\xD4';
            else if (startsWith(token, "Ouml"sv)) out += '\xD6';
            else if (startsWith(token, "Otilde"sv)) out += '\xD5';
            else if (startsWith(token, "oacute"sv)) out += '\xF3';
            else if (startsWith(token, "ograve"sv)) out += '\xF2';
            else if (startsWith(token, "ocirc"sv)) out += '\xF4';
            else if (startsWith(token, "ouml"sv)) out += '\xF6';
            else if (startsWith(token, "otilde"sv)) out += '\xF5';
            else if (startsWith(token, "Uacute"sv)) out += '\xDA';
            else if (startsWith(token, "Ugrave"sv)) out += '\xD9';
            else if (startsWith(token, "Ucirc"sv)) out += '\xDB';
            else if (startsWith(token, "Uuml"sv)) out += '\xDC';
            else if (startsWith(token, "uacute"sv)) out += '\xFA';
            else if (startsWith(token, "ugrave"sv)) out += '\xF9';
            else if (startsWith(token, "ucirc"sv)) out += '\xFB';
            else if (startsWith(token, "uuml"sv)) out += '\xFC';
            else if (startsWith(token, "Yacute"sv)) out += '\xDD';
            else if (startsWith(token, "yacute"sv)) out += '\xFD';
            else if (startsWith(token, "yuml"sv)) out += '\xFF';

            else if (startsWith(token, "deg"sv)) out += '\xB0';
            else if (startsWith(token, "plusmn"sv)) out += '\xB1';
            else if (startsWith(token, "sup2"sv)) out += '\xB2';
            else if (startsWith(token, "sup3"sv)) out += '\xB3';
            else if (startsWith(token, "sup1"sv)) out += '\xB9';
            else if (startsWith(token, "nbsp"sv)) out += '\xBA';
            else if (startsWith(token, "pound"sv)) out += '\xA3';
            else if (startsWith(token, "cent"sv)) out += '\xA2';
            else if (startsWith(token, "frac14"sv)) out += '\xBC';
            else if (startsWith(token, "frac12"sv)) out += '\xBD';
            else if (startsWith(token, "frac34"sv)) out += '\xBE';
            else if (startsWith(token, "iquest"sv)) out += '\xBF';
            else if (startsWith(token, "iexcl"sv)) out += '\xA1';
            else if (startsWith(token, "ETH"sv)) out += '\xD0';
            else if (startsWith(token, "eth"sv)) out += '\xF0';
            else if (startsWith(token, "THORN"sv)) out += '\xDE';
            else if (startsWith(token, "thorn"sv)) out += '\xFE';
            else if (startsWith(token, "AElig"sv)) out += '\xC6';
            else if (startsWith(token, "aelig"sv)) out += '\xE6';
            else if (startsWith(token, "Oslash"sv)) out += '\xD8';
            else if (startsWith(token, "curren"sv)) out += '\xA4';
            else if (startsWith(token, "Ccedil"sv)) out += '\xC7';
            else if (startsWith(token, "ccedil"sv)) out += '\xE7';
            else if (startsWith(token, "szlig"sv)) out += '\xDF';
            else if (startsWith(token, "Ntilde"sv)) out += '\xD1';
            else if (startsWith(token, "ntilde"sv)) out += '\xF1';
            else if (startsWith(token, "yen"sv)) out += '\xA5';
            else if (startsWith(token, "not"sv)) out += '\xAC';
            else if (startsWith(token, "ordf"sv)) out += '\xAA';
            else if (startsWith(token, "uml"sv)) out += '\xA8';
            else if (startsWith(token, "shy"sv)) out += '\xAD';
            else if (startsWith(token, "macr"sv)) out += '\xAF';
            else if (startsWith(token, "micro"sv)) out += "\xB5";
            else if (startsWith(token, "middot"sv)) out +="\xB7";
            else if (startsWith(token, "cedil"sv)) out += "\xB8";
            else if (startsWith(token, "ordm"sv)) out +=  "\xBA";
            else if (startsWith(token, "times"sv)) out += "\xD7";
            else if (startsWith(token, "divide"sv)) out +="\xF7";
            else if (startsWith(token, "oslash"sv)) out +="\xF8";
            continue;
        }

        // handle silly <variant word> items in greek whnu, remove when module is fixed
        if ((*from == '>') && (*(from-1) < 0)) {
            out += "&gt;";
            continue;
        }

        if (*from == '>') {    // process tokens
            intoken = false;
            suspendTextPassThru = false;
            newWord = true;
            handled = false;

            while (wordStart < (out.c_str() + out.size())) { //hack
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
                out.append("<seg type=\"x-variant\"");
                std::string cls = "x-class:";
                cls += tag.attribute("class");
                if (cls.length()>8)
                    out += formatted(" subType=\"%s\"", cls);

                out += ">";
                divEnd = "</seg>";
                newText = true;
                lastspace = false;
                handled = true;
            }
            // section titles
            if (!std::strcmp(token, "div class=\"sechead\"")) {
//                 pushString(&to, "<title>");
                out.append("<title>");
                divEnd = "</title>";
                newText = true;
                lastspace = false;
                handled = true;
            }
            else    if (!std::strcmp(token, "/div")) {
                //pushString(&to, divEnd.c_str());
                out.append(divEnd);
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
                out.append(VerseKey::convertToOSIS(tmp, key));
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
                    out.append("<hi type=\"i\">");
                        newText = true;
                     lastspace = false;
                        handled = true;
                    }
                    else    if (caseInsensitiveEquals(token, "/i")) {
//                         pushString(&to, "</hi>");
                    out.append("</hi>");
                        lastspace = false;
                        handled = true;
                    }
//                        }

                if (!std::strcmp(token, "b")) {
//                     pushString(&to, "<hi type=\"b\">");
                out.append("<hi type=\"b\">");
                newText = true;
                lastspace = false;
                    handled = true;
                }
            else if (!std::strcmp(token, "/b")) {
//                  pushString(&to, "</hi>");
                out.append("</hi>");
                    lastspace = false;
                    handled = true;
            }

            // Footnote
            if (startsWith(token, "note"sv)) {
                    //pushString(&to, "<note>");
                out.append("<note>");
                newText = true;
                lastspace = false;
                handled = true;
            }
            else    if (!std::strcmp(token, "/note")) {
                // pushString(&to, "</note>");
                out.append("</note>");
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
                out.append("<figure src=\"");

                const char* end = std::strchr(src+2, '"'); //start search behind src="

                if (end) { //append the path
                    out.append(src+2, end - (src+2));
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
                out.append("\" />");
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

                        out.insert(attStart - out.c_str(), buf);
                    }
                    else { //wordStart doesn't point to an existing <w> attribute!
                        buf = formatted("<w lemma=\"strong:%s\">", val);
                        out.insert(wordStart - out.c_str(), buf);
                        out += "</w>";
                        lastspace = false;
                    }
                } else {
                    // OLB verb morph, leave it out of OSIS tag
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

                    out.insert(attStart - out.c_str(), buf); //hack, we have to
                }
                else { //no existing <w> attribute fond
                    buf = formatted("<w morph=\"%s:%s\">", (!cls.empty() ? cls.c_str() : "robinson"), morph);
                    out.insert(wordStart - out.c_str(), buf);
                    out += "</w>";
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
                    out.pop_back();
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
                out += *from;
                //from++; //this line removes chars after an apostrophe! Needs fixing.
                break;
            default:
                if (newWord && (*from != ' ')) {
                    wordStart = out.c_str() + out.size();
                    newWord = false;

                    //fix this if required?
                    //std::memset(to, 0, 10);

                }

                if (!suspendTextPassThru) {
                    out += (*from);
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

            out = ref + out;

            if (vkey->getVerse()) {
                auto const tmp(std::static_pointer_cast<VerseKey>(vkey->clone()));
                *tmp = *vkey;
                tmp->setAutoNormalize(false);
                tmp->setIntros(true);

                out += "</verse>";

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
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */
