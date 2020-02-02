/******************************************************************************
 *
 *  gbfstrongs.cpp -    SWFilter descendant to hide or show Strong's number
 *            in a GBF module
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

#include "gbfstrongs.h"

#include <cstdio>
#include <cstdlib>
#include "../swmodule.h"
#include "../utilstr.h"


namespace swordxx {

namespace {

    static const char oName[] = "Strong's Numbers";
    static const char oTip[]  = "Toggles Strong's Numbers On and Off if they exist";

}

GBFStrongs::GBFStrongs()
    : OffOnOptionFilter(oName, oTip)
{}

GBFStrongs::~GBFStrongs() {
}


char GBFStrongs::processText(std::string &text, const SWKey *key, const SWModule *module) {
    (void) key;
    char token[2048]; // cheese.  Fix.
    int tokpos = 0;
    bool intoken = false;
    bool lastspace = false;
    int word = 1;
    char val[128];
    char wordstr[11u];
    char *valto;
    unsigned int textStart = 0, textEnd = 0;
    bool newText = false;
    std::string tmp;

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
        if (*from == '>') {	// process tokens
            intoken = false;
            if (*token == 'W' && (token[1] == 'G' || token[1] == 'H')) {	// Strongs
                if (module->isProcessEntryAttributes()) {
                    valto = val;
                    for (unsigned int i = 1; ((token[i]) && (i < 150)); i++)
                        *valto++ = token[i];
                    *valto = 0;
                    if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        // normal strongs number
                        sprintf(wordstr, "%03d", word++);
                        module->getEntryAttributes()["Word"][wordstr]["PartsCount"] = "1";
                        module->getEntryAttributes()["Word"][wordstr]["Lemma"] = val;
                        module->getEntryAttributes()["Word"][wordstr]["LemmaClass"] = "strong";
                        tmp = "";
                        tmp.append(out.c_str()+textStart, (int)(textEnd - textStart));
                        module->getEntryAttributes()["Word"][wordstr]["Text"] = tmp;
                        newText = true;
                    }
                    else {
                        // verb morph
                        sprintf(wordstr, "%03d", word-1);
                        module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
                        module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = "OLBMorph";
                    }
                }

                if (isOptionOff()) {
                    if ((from[1] == ' ') || (from[1] == ',') || (from[1] == ';') || (from[1] == '.') || (from[1] == '?') || (from[1] == '!') || (from[1] == ')') || (from[1] == '\'') || (from[1] == '\"')) {
                        if (lastspace)
                            out.pop_back();
                    }
                    if (newText) {textStart = out.size(); newText = false; }
                    continue;
                }
            }
            if (module->isProcessEntryAttributes()) {
                if ((*token == 'W') && (token[1] == 'T')) {	// Morph
                    valto = val;
                    for (unsigned int i = 2; ((token[i]) && (i < 150)); i++)
                        *valto++ = token[i];
                    *valto = 0;
                    sprintf(wordstr, "%03d", word-1);
                    module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = "GBFMorph";
                    module->getEntryAttributes()["Word"][wordstr]["Morph"] = val;
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
