/******************************************************************************
 *
 *  thmlstrongs.cpp -    SWFilter descendant to hide or show Strong's number
 *            in a ThML module
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
#include <cstring>
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
    using namespace std::literals::string_view_literals;

    (void) key;
    char token[2048]; // cheese.  Fix.
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

    std::string out;

    for (auto const * from = text.c_str(); *from; from++) {
        if (*from == '<') {
            intoken = true;
            tokpos = 0;
            token[0] = 0;
            token[1] = 0;
            token[2] = 0;
            textEnd = out.size();
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
                    if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        // normal strongs number
                        sprintf(wordstr, "%03d", word);
                        module->getEntryAttributes()["Word"][wordstr]["PartCount"] = "1";
                        module->getEntryAttributes()["Word"][wordstr]["Lemma"] = val;
                        module->getEntryAttributes()["Word"][wordstr]["LemmaClass"] = "strong";
                        tmp = "";
                        tmp.append(out.c_str()+textStart, (int)(textEnd - textStart));
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

                if (isOptionOff()) {	// if we don't want strongs
                    if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
                        if (lastspace)
                            out.pop_back();
                    }
                    if (newText) {textStart = text.length(); newText = false; }
                    continue;
                }
            }
            if (module->isProcessEntryAttributes()) {
                if (startsWith(token, "sync type=\"morph\""sv)) {
                    for (ch = token+17; *ch; ch++) {
                        if (startsWith(ch, "class=\""sv)) {
                            valto = val;
                            for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                                *valto++ = ch[i];
                            *valto = 0;
                            sprintf(wordstr, "%03d", word-1);
                            if ((caseInsensitiveEquals(val, "Robinsons")) || (caseInsensitiveEquals(val, "Robinson"))) {
                                std::strcpy(val, "robinson");
                            }
                            module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = val;
                        }
                        if (startsWith(ch, "value=\""sv)) {
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
            out += '<';
            out += token;
            out += '>';
            if (newText) {textStart = out.size(); newText = false; }
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
            out += *from;
            lastspace = (*from == ' ');
        }
    }
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */
