/******************************************************************************
 *
 *  thmlstrongs.cpp -    SWFilter descendant to hide or show Strong's number
 *            in a ThML module
 *
 * $Id$
 *
 * Copyright 2001-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "thmlstrongs.h"

#include <cstdio>
#include <cstdlib>
#include "../swmodule.h"
#include "../utilstr.h"


namespace swordxx {

namespace {

    static const char oName[] = "Strong's Numbers";
    static const char oTip[]  = "Toggles Strong's Numbers On and Off if they exist";

}

ThMLStrongs::ThMLStrongs()
    : OffOnOptionFilter(oName, oTip)
{}

ThMLStrongs::~ThMLStrongs() {
}


char ThMLStrongs::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    char token[2048]; // cheese.  Fix.
    const char *from;
    int tokpos = 0;
    bool intoken = false;
    bool lastspace = false;
    int word = 1;
    char val[128];
    char wordstr[11u];
    char *valto;
    char *ch;
    unsigned int textStart = 0, textEnd = 0;
    std::string tmp;
    bool newText = false;

    std::string orig = text;
    from = orig.c_str();

    for (text = ""; *from; from++) {
        if (*from == '<') {
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            textEnd = text.length();
            continue;
        }
        if (*from == '>') {    // process tokens
            intoken = false;
            if (!strnicmp(token, "sync type=\"Strongs\" ", 20)) {    // Strongs
                if (module->isProcessEntryAttributes()) {
                    valto = val;
                    for (unsigned int i = 27; token[i] != '\"' && i < 150; i++)
                        *valto++ = token[i];
                    *valto = 0;
                    if (atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        // normal strongs number
                        sprintf(wordstr, "%03d", word);
                        module->getEntryAttributes()["Word"][wordstr]["PartCount"] = "1";
                        module->getEntryAttributes()["Word"][wordstr]["Lemma"] = val;
                        module->getEntryAttributes()["Word"][wordstr]["LemmaClass"] = "strong";
                        tmp = "";
                        tmp.append(text.c_str()+textStart, (int)(textEnd - textStart));
                        module->getEntryAttributes()["Word"][wordstr]["Text"] = tmp;
                        newText = true;
                    }
                    else {
/*
                        // verb morph
                        sprintf(wordstr, "%03d", word);
                        module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
                        module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = "OLBMorph";
*/
                        word--;	// for now, completely ignore this word attribute.
                    }
                    word++;
                }

                if (!option) {	// if we don't want strongs
                    if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
                        if (lastspace)
                            text.pop_back();
                    }
                    if (newText) {textStart = text.length(); newText = false; }
                    continue;
                }
            }
            if (module->isProcessEntryAttributes()) {
                if (!strncmp(token, "sync type=\"morph\"", 17)) {
                    for (ch = token+17; *ch; ch++) {
                        if (!strncmp(ch, "class=\"", 7)) {
                            valto = val;
                            for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                                *valto++ = ch[i];
                            *valto = 0;
                            sprintf(wordstr, "%03d", word-1);
                            if ((caseInsensitiveEquals(val, "Robinsons")) || (caseInsensitiveEquals(val, "Robinson"))) {
                                strcpy(val, "robinson");
                            }
                            module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = val;
                        }
                        if (!strncmp(ch, "value=\"", 7)) {
                            valto = val;
                            for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                                *valto++ = ch[i];
                            *valto = 0;
                            sprintf(wordstr, "%03d", word-1);
                            module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
                        }
                    }
                    newText = true;
                }
            }
            // if not a strongs token, keep token in text
            text += '<';
            text += token;
            text += '>';
            if (newText) {textStart = text.length(); newText = false; }
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
            text += *from;
            lastspace = (*from == ' ');
        }
    }
    return 0;
}

} /* namespace swordxx */
