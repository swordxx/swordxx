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
            else if (!strncmp("nbsp", token, 4)) text += ' ';
            else if (!strncmp("quot", token, 4)) text += '"';
            else if (!strncmp("amp", token, 3)) text += '&';
            else if (!strncmp("lt", token, 2)) text += '<';
            else if (!strncmp("gt", token, 2)) text += '>';
            else if (!strncmp("brvbar", token, 6)) text += '\xA6';
            else if (!strncmp("sect", token, 4)) text += '\xA7';
            else if (!strncmp("copy", token, 4)) text += '\xA9';
            else if (!strncmp("laquo", token, 5)) text += '\xAB';
            else if (!strncmp("reg", token, 3)) text += '\xAE';
            else if (!strncmp("acute", token, 5)) text += '\xB4';
            else if (!strncmp("para", token, 4)) text += '\xB6';
            else if (!strncmp("raquo", token, 5)) text += '\xBB';
            else if (!strncmp("Aacute", token, 6)) text += '\xC1';
            else if (!strncmp("Agrave", token, 6)) text += '\xC0';
            else if (!strncmp("Acirc", token, 5)) text += '\xC2';
            else if (!strncmp("Auml", token, 4)) text += '\xC4';
            else if (!strncmp("Atilde", token, 6)) text += '\xC3';
            else if (!strncmp("Aring", token, 5)) text += '\xC5';
            else if (!strncmp("aacute", token, 6)) text += '\xE1';
            else if (!strncmp("agrave", token, 6)) text += '\xE0';
            else if (!strncmp("acirc", token, 5)) text += '\xE2';
            else if (!strncmp("auml", token, 4)) text += '\xE4';
            else if (!strncmp("atilde", token, 6)) text += '\xE3';
            else if (!strncmp("aring", token, 5)) text += '\xE5';
            else if (!strncmp("Eacute", token, 6)) text += '\xC9';
            else if (!strncmp("Egrave", token, 6)) text += '\xC8';
            else if (!strncmp("Ecirc", token, 5)) text += '\xCA';
            else if (!strncmp("Euml", token, 4)) text += '\xCB';
            else if (!strncmp("eacute", token, 6)) text += '\xE9';
            else if (!strncmp("egrave", token, 6)) text += '\xE8';
            else if (!strncmp("ecirc", token, 5)) text += '\xEA';
            else if (!strncmp("euml", token, 4)) text += '\xEB';
            else if (!strncmp("Iacute", token, 6)) text += '\xCD';
            else if (!strncmp("Igrave", token, 6)) text += '\xCC';
            else if (!strncmp("Icirc", token, 5)) text += '\xCE';
            else if (!strncmp("Iuml", token, 4)) text += '\xCF';
            else if (!strncmp("iacute", token, 6)) text += '\xED';
            else if (!strncmp("igrave", token, 6)) text += '\xEC';
            else if (!strncmp("icirc", token, 5)) text += '\xEE';
            else if (!strncmp("iuml", token, 4)) text += '\xEF';
            else if (!strncmp("Oacute", token, 6)) text += '\xD3';
            else if (!strncmp("Ograve", token, 6)) text += '\xD2';
            else if (!strncmp("Ocirc", token, 5)) text += '\xD4';
            else if (!strncmp("Ouml", token, 4)) text += '\xD6';
            else if (!strncmp("Otilde", token, 6)) text += '\xD5';
            else if (!strncmp("oacute", token, 6)) text += '\xF3';
            else if (!strncmp("ograve", token, 6)) text += '\xF2';
            else if (!strncmp("ocirc", token, 5)) text += '\xF4';
            else if (!strncmp("ouml", token, 4)) text += '\xF6';
            else if (!strncmp("otilde", token, 6)) text += '\xF5';
            else if (!strncmp("Uacute", token, 6)) text += '\xDA';
            else if (!strncmp("Ugrave", token, 6)) text += '\xD9';
            else if (!strncmp("Ucirc", token, 5)) text += '\xDB';
            else if (!strncmp("Uuml", token, 4)) text += '\xDC';
            else if (!strncmp("uacute", token, 6)) text += '\xFA';
            else if (!strncmp("ugrave", token, 6)) text += '\xF9';
            else if (!strncmp("ucirc", token, 5)) text += '\xFB';
            else if (!strncmp("uuml", token, 4)) text += '\xFC';
            else if (!strncmp("Yacute", token, 6)) text += '\xDD';
            else if (!strncmp("yacute", token, 6)) text += '\xFD';
            else if (!strncmp("yuml", token, 4)) text += '\xFF';

            else if (!strncmp("deg", token, 3)) text += '\xB0';
            else if (!strncmp("plusmn", token, 6)) text += '\xB1';
            else if (!strncmp("sup2", token, 4)) text += '\xB2';
            else if (!strncmp("sup3", token, 4)) text += '\xB3';
            else if (!strncmp("sup1", token, 4)) text += '\xB9';
            else if (!strncmp("nbsp", token, 4)) text += '\xBA';
            else if (!strncmp("pound", token, 5)) text += '\xA3';
            else if (!strncmp("cent", token, 4)) text += '\xA2';
            else if (!strncmp("frac14", token, 6)) text += '\xBC';
            else if (!strncmp("frac12", token, 6)) text += '\xBD';
            else if (!strncmp("frac34", token, 6)) text += '\xBE';
            else if (!strncmp("iquest", token, 6)) text += '\xBF';
            else if (!strncmp("iexcl", token, 5)) text += '\xA1';
            else if (!strncmp("ETH", token, 3)) text += '\xD0';
            else if (!strncmp("eth", token, 3)) text += '\xF0';
            else if (!strncmp("THORN", token, 5)) text += '\xDE';
            else if (!strncmp("thorn", token, 5)) text += '\xFE';
            else if (!strncmp("AElig", token, 5)) text += '\xC6';
            else if (!strncmp("aelig", token, 5)) text += '\xE6';
            else if (!strncmp("Oslash", token, 6)) text += '\xD8';
            else if (!strncmp("curren", token, 6)) text += '\xA4';
            else if (!strncmp("Ccedil", token, 6)) text += '\xC7';
            else if (!strncmp("ccedil", token, 6)) text += '\xE7';
            else if (!strncmp("szlig", token, 5)) text += '\xDF';
            else if (!strncmp("Ntilde", token, 6)) text += '\xD1';
            else if (!strncmp("ntilde", token, 6)) text += '\xF1';
            else if (!strncmp("yen", token, 3)) text += '\xA5';
            else if (!strncmp("not", token, 3)) text += '\xAC';
            else if (!strncmp("ordf", token, 4)) text += '\xAA';
            else if (!strncmp("uml", token, 3)) text += '\xA8';
            else if (!strncmp("shy", token, 3)) text += '\xAD';
            else if (!strncmp("macr", token, 4)) text += '\xAF';
            else if (!strncmp("micro", token, 5)) text += "\xB5";
            else if (!strncmp("middot", token, 6)) text +="\xB7";
            else if (!strncmp("cedil", token, 5)) text += "\xB8";
            else if (!strncmp("ordm", token, 4)) text +=  "\xBA";
            else if (!strncmp("times", token, 5)) text += "\xD7";
            else if (!strncmp("divide", token, 6)) text +="\xF7";
            else if (!strncmp("oslash", token, 6)) text +="\xF8";
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
            if (!strncmp(token, "div type=\"variant\"", 18)) {
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
            if (!strncmp(token, "scripRef", 8)) {
    //            pushString(buf, "<reference osisRef=\"");
                suspendTextPassThru = true;
                newText = true;
                handled = true;
            }
            else    if (!strncmp(token, "/scripRef", 9)) {
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
            if (!strncmp(token, "note", 4)) {
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
            else    if (!strncmp(token, "img ", 4)) {
                const char *src = strstr(token, "src");
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
                if (atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                    // normal strongs number
                    strstrip(val);

                    if (!strncmp(wordStart, "<w ", 3)) {
                        const char *attStart = strstr(wordStart, "lemma");
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
            else    if (!strncmp(token, "sync type=\"morph\"", 17)) {
                std::string cls = "";
                std::string morph = "";
                for (ch = token+17; *ch; ch++) {
                    if (!strncmp(ch, "class=\"", 7)) {
                        valto = val;
                        for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                            *valto++ = ch[i];
                        *valto = 0;
                        strstrip(val);
                        cls = val;
                    }
                    if (!strncmp(ch, "value=\"", 7)) {
                        valto = val;
                        for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                            *valto++ = ch[i];
                        *valto = 0;
                        strstrip(val);
                        morph = val;
                    }
                }
                if (!strncmp(wordStart, "<w ", 3)) {
                    const char *attStart = strstr(wordStart, "morph");
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
