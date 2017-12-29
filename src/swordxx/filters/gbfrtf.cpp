/******************************************************************************
 *
 *  gbfrtf.cpp -    SWFilter descendant to convert all GBF tags to RTF tags
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "gbfrtf.h"

#include <cstdlib>
#include <string>
#include "../utilstr.h"


namespace swordxx {

GBFRTF::GBFRTF() {
}


char GBFRTF::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) key;
    (void) module;
    char token[2048];
    char val[128];
    char *valto;
    char *num;
    int tokpos = 0;
    bool intoken = false;
    const char *tok;
    std::string strongnum;
    std::string strongtense;
    bool hideText = false;
    int wordCount = 0;

    const char *from;
    std::string orig = text;
    from = orig.c_str();
    for (text = ""; *from; from++) {
        if (*from == '<') {
            wordCount = 0;
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            continue;
        }
        if (*from == '>') {
            intoken = false;
                        // process desired tokens
            // deal with OSIS note tags.  Just hide till OSISRTF
            if (!std::strncmp(token, "note ", 5)) {
                hideText = true;
            }
            if (!std::strncmp(token, "/note", 5)) {
                hideText = false;
            }

            switch (*token) {
            case 'w':    // OSIS Word (temporary until OSISRTF is done)
                strongnum = "";
                strongtense = "";
                valto = val;
                num = std::strstr(token, "lemma=\"x-Strongs:");
                if (num) {
                    for (num+=17; ((*num) && (*num != '\"')); num++)
                        *valto++ = *num;
                    *valto = 0;
                    if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        // normal strongs number
                        strongnum += "{\\cf3 \\sub <";
                        for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++)
                            strongnum += *tok;
                        strongnum += ">}";
                    }
                    /*    forget these for now
                    else {
                        // verb morph
                        sprintf(wordstr, "%03d", word-1);
                        module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
                    }
                    */
                }
                else {
                    num = std::strstr(token, "lemma=\"strong:");
                    if (num) {
                        for (num+=14; ((*num) && (*num != '\"')); num++)
                            *valto++ = *num;
                        *valto = 0;
                        if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                            // normal strongs number
                            strongnum += "{\\cf3 \\sub <";
                            for (tok = (!charIsDigit(*val))?val+1:val; *tok; tok++)
                                strongnum += *tok;
                            strongnum += ">}";
                        }
                        /*    forget these for now
                        else {
                            // verb morph
                            sprintf(wordstr, "%03d", word-1);
                            module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
                        }
                        */
                    }
                }
                valto = val;
                num = std::strstr(token, "morph=\"x-Robinson:");
                if (num) {
                    for (num+=18; ((*num) && (*num != '\"')); num++)
                        *valto++ = *num;
                    *valto = 0;
                    // normal robinsons tense
                    strongtense += "{\\cf4 \\sub (";
                    for (tok = val; *tok; tok++)
                        strongtense += *tok;
                    strongtense += ")}";
                }
                continue;

            case '/':
                if (token[1] == 'w') {
                    if ((wordCount > 0) || (strongnum != "{\\cf3 \\sub <3588>}")) {
                        //for (i = 0; i < strongnum.length(); i++)
                            text += strongnum;
                    //for (i = 0; i < strongtense.length(); i++)
                        text += strongtense;
                    }
                }
                continue;

            case 'W':    // Strongs
                switch(token[1]) {
                case 'G':               // Greek
                case 'H':               // Hebrew
                    text += "{\\cf3 \\sub <";
                    for (tok = token + 2; *tok; tok++)
                        text += *tok;
                    text += ">}";
                    continue;

                case 'T':               // Tense
                    text += "{\\cf4 \\sub (";
                    bool separate = false;
                    for (tok = token + 2; *tok; tok++) {
                        if (separate) {
                            text += "; ";
                            separate = false;
                        }
                        switch (*tok) {
                        case 'G':
                        case 'H':
                            for (tok++; *tok; tok++) {
                                if (charIsDigit(*tok)) {
                                    text += *tok;
                                    separate = true;
                                }
                                else {
                                    tok--;
                                    break;
                                }
                            }
                            break;
                        default:
                            for (; *tok; tok++) {
                                   text += *tok;
                            }
                        }
                    }
                    text += ")}";
                    continue;
                }
                break;
            case 'R':
                switch(token[1]) {
                case 'X':
                    text += "<a href=\"\">";
                  continue;
                case 'x':
                    text += "</a>";
                  continue;
                case 'F':               // footnote begin
                    text += "{\\i1 \\sub [ ";
                    continue;
                case 'f':               // footnote end
                    text += " ] }";
                    continue;
                }
                break;
            case 'F':            // font tags
                switch(token[1]) {
                case 'I':        // italic start
                    text += "\\i1 ";
                    continue;
                case 'i':        // italic end
                    text += "\\i0 ";
                    continue;
                case 'B':        // bold start
                    text += "\\b1 ";
                    continue;
                case 'b':        // bold end
                    text += "\\b0 ";
                    continue;
                case 'N':
                    text += '{';
                    if (!strnicmp(token+2, "Symbol", 6))
                        text += "\\f7 ";
                                        if (!strnicmp(token+2, "Courier", 7))
                        text += "\\f8 ";
                    continue;
                case 'n':
                    text += '}';
                    continue;
                case 'S':
                    text += "{\\super ";
                    continue;
                case 's':
                    text += '}';
                    continue;
                case 'R':
                    text += "{\\cf6 ";
                    continue;
                case 'r':
                    text += '}';
                    continue;
                case 'O':
                case 'C':
                    text += "\\scaps1 ";
                    continue;
                case 'o':
                case 'c':
                    text += "\\scaps0 ";
                    continue;
                case 'V':
                    text += "{\\sub ";
                    continue;
                case 'v':
                    text += '}';
                    continue;
                case 'U':
                    text += "\\ul1 ";
                    continue;
                case 'u':
                    text += "\\ul0 ";
                    continue;
                }
                break;
            case 'C':            // special character tags
                switch(token[1]) {
                case 'A':               // ASCII value
                    text += (char)std::atoi(&token[2]);
                    continue;
                case 'G':
                    text += '>';
                    continue;
                case 'L':               // line break
                    text += "\\line ";
                    continue;
                case 'M':               // new paragraph
                    text += "\\par ";
                    continue;
                case 'T':
                    text += '<';
                }
                break;
            case 'T':            // title formatting
              switch(token[1])
                {
                case 'T':               // Book title begin
                    text += "{\\large ";
                 continue;
                case 't':
                 text += '}';
                 continue;
                case 'S':
                    text += "\\par {\\i1\\b1 ";
                  continue;
                case 's':
                    text += "}\\par ";
                  continue;
                }
              break;
            case 'J':    // Strongs
                switch(token[1]) {
                case 'L':
                    text += "\\ql ";
                case 'C':
                    text += "\\qc ";
                case 'R':
                    text += "\\qr ";
                                case 'F':
                                    text += "\\qj ";
                                }
            }
            continue;
        }
        if (intoken) {
            if (tokpos < 2045) {
                token[tokpos++] = *from;
                // TODO: why is this + 2 ?
                token[tokpos+2] = 0;
            }
        }
        else {
            if (!hideText) {
                wordCount++;
                text += *from;
            }
        }
    }
    return 0;
}

} /* namespace swordxx */

