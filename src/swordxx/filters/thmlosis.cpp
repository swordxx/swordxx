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
    (void) module;
    char token[2048]; // cheese.  Fix.
    int tokpos = 0;
    bool intoken = false;
    bool keepToken = false;
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
            else if (!std::strncmp("nbsp", token, 4)) text += ' ';
            else if (!std::strncmp("quot", token, 4)) text += '"';
            else if (!std::strncmp("amp", token, 3)) text += '&';
            else if (!std::strncmp("lt", token, 2)) text += '<';
            else if (!std::strncmp("gt", token, 2)) text += '>';
            else if (!std::strncmp("brvbar", token, 6)) text += '\xA6';
            else if (!std::strncmp("sect", token, 4)) text += '\xA7';
            else if (!std::strncmp("copy", token, 4)) text += '\xA9';
            else if (!std::strncmp("laquo", token, 5)) text += '\xAB';
            else if (!std::strncmp("reg", token, 3)) text += '\xAE';
            else if (!std::strncmp("acute", token, 5)) text += '\xB4';
            else if (!std::strncmp("para", token, 4)) text += '\xB6';
            else if (!std::strncmp("raquo", token, 5)) text += '\xBB';
            else if (!std::strncmp("Aacute", token, 6)) text += '\xC1';
            else if (!std::strncmp("Agrave", token, 6)) text += '\xC0';
            else if (!std::strncmp("Acirc", token, 5)) text += '\xC2';
            else if (!std::strncmp("Auml", token, 4)) text += '\xC4';
            else if (!std::strncmp("Atilde", token, 6)) text += '\xC3';
            else if (!std::strncmp("Aring", token, 5)) text += '\xC5';
            else if (!std::strncmp("aacute", token, 6)) text += '\xE1';
            else if (!std::strncmp("agrave", token, 6)) text += '\xE0';
            else if (!std::strncmp("acirc", token, 5)) text += '\xE2';
            else if (!std::strncmp("auml", token, 4)) text += '\xE4';
            else if (!std::strncmp("atilde", token, 6)) text += '\xE3';
            else if (!std::strncmp("aring", token, 5)) text += '\xE5';
            else if (!std::strncmp("Eacute", token, 6)) text += '\xC9';
            else if (!std::strncmp("Egrave", token, 6)) text += '\xC8';
            else if (!std::strncmp("Ecirc", token, 5)) text += '\xCA';
            else if (!std::strncmp("Euml", token, 4)) text += '\xCB';
            else if (!std::strncmp("eacute", token, 6)) text += '\xE9';
            else if (!std::strncmp("egrave", token, 6)) text += '\xE8';
            else if (!std::strncmp("ecirc", token, 5)) text += '\xEA';
            else if (!std::strncmp("euml", token, 4)) text += '\xEB';
            else if (!std::strncmp("Iacute", token, 6)) text += '\xCD';
            else if (!std::strncmp("Igrave", token, 6)) text += '\xCC';
            else if (!std::strncmp("Icirc", token, 5)) text += '\xCE';
            else if (!std::strncmp("Iuml", token, 4)) text += '\xCF';
            else if (!std::strncmp("iacute", token, 6)) text += '\xED';
            else if (!std::strncmp("igrave", token, 6)) text += '\xEC';
            else if (!std::strncmp("icirc", token, 5)) text += '\xEE';
            else if (!std::strncmp("iuml", token, 4)) text += '\xEF';
            else if (!std::strncmp("Oacute", token, 6)) text += '\xD3';
            else if (!std::strncmp("Ograve", token, 6)) text += '\xD2';
            else if (!std::strncmp("Ocirc", token, 5)) text += '\xD4';
            else if (!std::strncmp("Ouml", token, 4)) text += '\xD6';
            else if (!std::strncmp("Otilde", token, 6)) text += '\xD5';
            else if (!std::strncmp("oacute", token, 6)) text += '\xF3';
            else if (!std::strncmp("ograve", token, 6)) text += '\xF2';
            else if (!std::strncmp("ocirc", token, 5)) text += '\xF4';
            else if (!std::strncmp("ouml", token, 4)) text += '\xF6';
            else if (!std::strncmp("otilde", token, 6)) text += '\xF5';
            else if (!std::strncmp("Uacute", token, 6)) text += '\xDA';
            else if (!std::strncmp("Ugrave", token, 6)) text += '\xD9';
            else if (!std::strncmp("Ucirc", token, 5)) text += '\xDB';
            else if (!std::strncmp("Uuml", token, 4)) text += '\xDC';
            else if (!std::strncmp("uacute", token, 6)) text += '\xFA';
            else if (!std::strncmp("ugrave", token, 6)) text += '\xF9';
            else if (!std::strncmp("ucirc", token, 5)) text += '\xFB';
            else if (!std::strncmp("uuml", token, 4)) text += '\xFC';
            else if (!std::strncmp("Yacute", token, 6)) text += '\xDD';
            else if (!std::strncmp("yacute", token, 6)) text += '\xFD';
            else if (!std::strncmp("yuml", token, 4)) text += '\xFF';

            else if (!std::strncmp("deg", token, 3)) text += '\xB0';
            else if (!std::strncmp("plusmn", token, 6)) text += '\xB1';
            else if (!std::strncmp("sup2", token, 4)) text += '\xB2';
            else if (!std::strncmp("sup3", token, 4)) text += '\xB3';
            else if (!std::strncmp("sup1", token, 4)) text += '\xB9';
            else if (!std::strncmp("nbsp", token, 4)) text += '\xBA';
            else if (!std::strncmp("pound", token, 5)) text += '\xA3';
            else if (!std::strncmp("cent", token, 4)) text += '\xA2';
            else if (!std::strncmp("frac14", token, 6)) text += '\xBC';
            else if (!std::strncmp("frac12", token, 6)) text += '\xBD';
            else if (!std::strncmp("frac34", token, 6)) text += '\xBE';
            else if (!std::strncmp("iquest", token, 6)) text += '\xBF';
            else if (!std::strncmp("iexcl", token, 5)) text += '\xA1';
            else if (!std::strncmp("ETH", token, 3)) text += '\xD0';
            else if (!std::strncmp("eth", token, 3)) text += '\xF0';
            else if (!std::strncmp("THORN", token, 5)) text += '\xDE';
            else if (!std::strncmp("thorn", token, 5)) text += '\xFE';
            else if (!std::strncmp("AElig", token, 5)) text += '\xC6';
            else if (!std::strncmp("aelig", token, 5)) text += '\xE6';
            else if (!std::strncmp("Oslash", token, 6)) text += '\xD8';
            else if (!std::strncmp("curren", token, 6)) text += '\xA4';
            else if (!std::strncmp("Ccedil", token, 6)) text += '\xC7';
            else if (!std::strncmp("ccedil", token, 6)) text += '\xE7';
            else if (!std::strncmp("szlig", token, 5)) text += '\xDF';
            else if (!std::strncmp("Ntilde", token, 6)) text += '\xD1';
            else if (!std::strncmp("ntilde", token, 6)) text += '\xF1';
            else if (!std::strncmp("yen", token, 3)) text += '\xA5';
            else if (!std::strncmp("not", token, 3)) text += '\xAC';
            else if (!std::strncmp("ordf", token, 4)) text += '\xAA';
            else if (!std::strncmp("uml", token, 3)) text += '\xA8';
            else if (!std::strncmp("shy", token, 3)) text += '\xAD';
            else if (!std::strncmp("macr", token, 4)) text += '\xAF';
            else if (!std::strncmp("micro", token, 5)) text += "\xB5";
            else if (!std::strncmp("middot", token, 6)) text +="\xB7";
            else if (!std::strncmp("cedil", token, 5)) text += "\xB8";
            else if (!std::strncmp("ordm", token, 4)) text +=  "\xBA";
            else if (!std::strncmp("times", token, 5)) text += "\xD7";
            else if (!std::strncmp("divide", token, 6)) text +="\xF7";
            else if (!std::strncmp("oslash", token, 6)) text +="\xF8";
            continue;
        }

        // handle silly <variant word> items in greek whnu, remove when module is fixed
        if ((*from == '>') && (*(from-1) < 0)) {
            text += "&gt;";
            continue;
        }

        if (*from == '>') {    // process tokens
            intoken = false;
            keepToken = false;
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
            if (!std::strncmp(token, "div type=\"variant\"", 18)) {
                XMLTag tag = token;
                text.append("<seg type=\"x-variant\"");
                std::string cls = "x-class:";
                cls += tag.attribute("class");
                if (cls.length()>8)
                    text += formatted(" subType=\"%s\"", cls.c_str());

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
            if (!std::strncmp(token, "scripRef", 8)) {
    //            pushString(buf, "<reference osisRef=\"");
                suspendTextPassThru = true;
                newText = true;
                handled = true;
            }
            else    if (!std::strncmp(token, "/scripRef", 9)) {
                std::string tmp;
                tmp = "";
                tmp.append(textStart, (int)(textEnd - textStart)+1);
                //pushString(&to, convertToOSIS(tmp.c_str(), key));
                text.append(VerseKey::convertToOSIS(tmp.c_str(), key));
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
            if (!std::strncmp(token, "note", 4)) {
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
            else    if (!std::strncmp(token, "img ", 4)) {
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

                    if (!std::strncmp(wordStart, "<w ", 3)) {
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
            else    if (!std::strncmp(token, "sync type=\"morph\"", 17)) {
                std::string cls = "";
                std::string morph = "";
                for (ch = token+17; *ch; ch++) {
                    if (!std::strncmp(ch, "class=\"", 7)) {
                        valto = val;
                        for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                            *valto++ = ch[i];
                        *valto = 0;
                        strstrip(val);
                        cls = val;
                    }
                    if (!std::strncmp(ch, "value=\"", 7)) {
                        valto = val;
                        for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                            *valto++ = ch[i];
                        *valto = 0;
                        strstrip(val);
                        morph = val;
                    }
                }
                if (!std::strncmp(wordStart, "<w ", 3)) {
                    const char *attStart = std::strstr(wordStart, "morph");
                    if (attStart) { //existing morph attribute, append this one to it
                        attStart += 7;
                        buf = formatted("%s:%s ", ((cls.length())?cls.c_str():"robinson"), morph.c_str());
                    }
                    else { // no lemma attribute
                        attStart = wordStart + 3;
                        buf = formatted("morph=\"%s:%s\" ", ((cls.length())?cls.c_str():"robinson"), morph.c_str());
                    }

                    text.insert(attStart - text.c_str(), buf); //hack, we have to
                }
                else { //no existing <w> attribute fond
                    buf = formatted("<w morph=\"%s:%s\">", ((cls.length())?cls.c_str():"robinson"), morph.c_str());
                    text.insert(wordStart - text.c_str(), buf);
                    text += "</w>";
                    lastspace = false;

                }
                handled = true;
            }

            if (!keepToken) {
                if (!handled) {
                    SWLog::getSystemLog()->logError(
                            "Unprocessed Token: <%s> in key %s",
                            token,
                            key ? key->getText() : "<unknown>");
//                    exit(-1);
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

            // if not a strongs token, keep token in text
            text += formatted("<%s>", token);

            if (newText) {
                textStart = text.c_str() + text.length();
                newWord = false;
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
        if (vkey->getVerse()) {
            ref = formatted("\t\t<verse osisID=\"%s\">", vkey->getOSISRef());
        }

        if (ref.length() > 0) {

            text = ref + text;

            if (vkey->getVerse()) {
                std::unique_ptr<VerseKey> tmp(
                            static_cast<VerseKey *>(vkey->clone().release()));
                *tmp = *vkey;
                tmp->setAutoNormalize(false);
                tmp->setIntros(true);

                text += "</verse>";

                *tmp = Position::MaxVerse;
                if (*vkey == *tmp) {
                    tmp->setVerse(0);
//                    sprintf(ref, "\t</div>");
//                    pushString(&to, ref);
                    *tmp = Position::MaxChapter;
                    *tmp = Position::MaxVerse;
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
