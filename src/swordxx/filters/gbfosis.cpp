/******************************************************************************
 *
 *  gbfosis.cpp -    GBF to OSIS filter
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

#include "gbfosis.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../keys/versekey.h"
#include "../swlog.h"
#include "../swmodule.h"
#include "../utilstr.h"


namespace swordxx {

GBFOSIS::GBFOSIS() {
}


GBFOSIS::~GBFOSIS() {
}


char GBFOSIS::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    (void) module;
    char token[2048]; //cheesy, we seem to like cheese :)
    int tokpos = 0;
    bool intoken = false;

    std::string tmp;
    std::string value;

    bool suspendTextPassThru = false;
    bool handled = false;
    bool newWord = false;
    bool newText = false;
    bool lastspace = false;

    const char *wordStart = text.c_str();
    const char * wordEnd = nullptr;

    const char * textStart = nullptr;
    const char * textEnd = nullptr;

    std::string textNode = "";

    std::string buf;

    std::string out;
    for (const char* from = text.c_str(); *from; ++from) {
        if (*from == '<') { //start of new token detected
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            textEnd = from-1; //end of last text node found
            wordEnd = out.c_str() + out.size();//not good, instead of wordEnd = to!

            continue;
        }

        if (*from == '>') {    // process tokens
            intoken = false;
            suspendTextPassThru = false;
            newWord = true;
            handled = false;

            while (wordStart < (out.c_str() + out.length())) { //hack
                if (std::strchr(";,. :?!()'\"", *wordStart) && wordStart[0] && wordStart[1])
                    wordStart++;
                else break;
            }
            while (wordEnd > wordStart) {
                if (std::strchr(" ,;:.?!()'\"", *wordEnd))
                    wordEnd--;
                else break;
            }

            // Scripture Reference
            if (startsWith(token, "scripRef"sv)) {
                suspendTextPassThru = true;
                newText = true;
                handled = true;
            }
            else if (startsWith(token, "/scripRef"sv)) {
                tmp = "";
                tmp.append(textStart, (int)(textEnd - textStart)+1);
                out += VerseKey::convertToOSIS(tmp, key);

                lastspace = false;
                suspendTextPassThru = false;
                handled = true;
            }

            // Footnote
            if (!std::strcmp(token, "RF") || startsWith(token, "RF "sv)) { //the GBFFootnotes filter adds the attribute "swordFootnote", we want to catch that, too
    //            pushString(buf, "<reference work=\"Bible.KJV\" reference=\"");
                out += "<note type=\"x-StudyNote\">";
                newText = true;
                lastspace = false;
                handled = true;
            }
            else    if (!std::strcmp(token, "Rf")) {
                out += "</note>";
                lastspace = false;
                handled = true;
            }
            // hebrew titles
            if (!std::strcmp(token, "TH")) {
                out += "<title type=\"psalm\">";
                newText = true;
                lastspace = false;
                handled = true;
            }
            else    if (!std::strcmp(token, "Th")) {
                out += "</title>";
                lastspace = false;
                handled = true;
            }
            // Italics assume transchange
            if (!std::strcmp(token, "FI")) {
                out += "<transChange type=\"added\">";
                newText = true;
                lastspace = false;
                handled = true;
            }
            else    if (!std::strcmp(token, "Fi")) {
                out += "</transChange>";
                lastspace = false;
                handled = true;
            }
            // less than
            if (!std::strcmp(token, "CT")) {
                out += "&lt;";
                newText = true;
                lastspace = false;
                handled = true;
            }
            // greater than
            if (!std::strcmp(token, "CG")) {
                out += "&gt;";
                newText = true;
                lastspace = false;
                handled = true;
            }
            // Paragraph break.  For now use empty paragraph element
            if (!std::strcmp(token, "CM")) {
                out += "<milestone type=\"x-p\" />";
                newText = true;
                lastspace = false;
                handled = true;
            }

            // Figure
            else    if (startsWith(token, "img "sv)) {
                const char *src = std::strstr(token, "src");
                if (!src)        // assert we have a src attribute
                    continue;
//                    return false;

                out += "<figure src=\"";
                const char *c;
                for (c = src;((*c) && (*c != '"')); c++);

// uncomment for Sword++ absolute path logic
//                if (*(c+1) == '/') {
//                    pushString(buf, "file:");
//                    pushString(buf, module->getConfigEntry("AbsoluteDataPath"));
//                    if (*((*buf)-1) == '/')
//                        c++;        // skip '/'
//                }
// end of uncomment for asolute path logic

                for (c++;((*c) && (*c != '"')); c++) {
                    out += *c;
                }
                out += "\" />";

                lastspace = false;
                handled = true;
            }

            // Strongs numbers
            else if (*token == 'W' && (token[1] == 'G' || token[1] == 'H')) {    // Strongs
                bool divineName = false;
                value = token+1;

                // normal strongs number
                //std::strstrip(val);
                if (startsWith(wordStart, "<w "sv)) {
                    const char *attStart = std::strstr(wordStart, "lemma");
                    if (attStart) {
                        attStart += 7;

                        buf = formatted("strong:%s ", value);
                    }
                    else { // no lemma attribute
                        attStart = wordStart + 3;

                        buf = formatted("lemma=\"strong:%s\" ", value);
                    }

                    out.insert(attStart - out.c_str(), buf);
                }
                else { //wordStart doesn't point to an existing <w> attribute!
                    if (value == "H03068") {    //divineName
                        buf = formatted("<divineName><w lemma=\"strong:%s\">", value);

                        divineName = true;
                    }
                    else {
                        buf = formatted("<w lemma=\"strong:%s\">", value);
                    }

                    out.insert(wordStart - out.c_str(), buf);

                    if (divineName) {
                        wordStart += 12;
                        out += "</w></divineName>";
                    }
                    else    out += "</w>";

                    lastspace = false;
                }
                handled = true;
            }

            // Morphology
            else if (*token == 'W' && token[1] == 'T') {
                if (token[2] == 'G' || token[2] == 'H') {    // Strongs
                    value = token+2;
                }
                else value = token+1;

                if (startsWith(wordStart, "<w "sv)) {
                    const char *attStart = std::strstr(wordStart, "morph");
                    if (attStart) { //existing morph attribute, append this one to it
                        attStart += 7;
                        buf = formatted("robinson:%s ", value);
                    }
                    else { // no lemma attribute
                        attStart = wordStart + 3;
                        buf = formatted("morph=\"robinson:%s\" ", value);
                    }

                    out.insert(attStart - out.c_str(), buf); //hack, we have to
                }
                else { //no existing <w> attribute fond
                    buf = formatted("<w morph=\"robinson:%s\">", value);
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
                    wordStart = out.c_str() + out.length();
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
                auto const tmp2(std::static_pointer_cast<VerseKey>(vkey->clone()));
                *tmp2 = *vkey;
                tmp2->setAutoNormalize(false);
                tmp2->setIntros(true);

                out += "</verse>";

                tmp2->positionToMaxVerse();
                if (*vkey == *tmp2) {
                    tmp2->setVerse(0);
//                    sprintf(ref, "\t</div>");
//                    pushString(&to, ref);
                    tmp2->positionToMaxChapter();
                    tmp2->positionToMaxVerse();
                    if (*vkey == *tmp2) {
                        tmp2->setChapter(0);
                        tmp2->setVerse(0);
//                        sprintf(ref, "\t</div>");
//                        pushString(&to, ref);
                    }
                }
            }
//            else if (vkey->Chapter()) {
//                sprintf(ref, "\t<div type=\"chapter\" osisID=\"%s\">", vkey->getOSISRef());
//            }
//            else sprintf(ref, "\t<div type=\"book\" osisID=\"%s\">", vkey->getOSISRef());
        }
    }
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */
