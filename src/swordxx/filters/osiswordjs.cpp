/******************************************************************************
 *
 *  osiswordjs.cpp -    SWFilter descendant for ???
 *
 * $Id$
 *
 * Copyright 2005-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osiswordjs.h"

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Word Javascript";
    static const char oTip[]  = "Toggles Word Javascript data";

    static const StringList *oValues() {
        static const std::string choices[3] = {"Off", "On", ""};
        static const StringList oVals(&choices[0], &choices[2]);
        return &oVals;
    }
}


OSISWordJS::OSISWordJS() : SWOptionFilter(oName, oTip, oValues()) {

     defaultGreekLex   = 0;
     defaultHebLex     = 0;
     defaultGreekParse = 0;
     defaultHebParse   = 0;
     mgr               = 0;
}


OSISWordJS::~OSISWordJS() {
}


char OSISWordJS::processText(std::string &text, const SWKey *key, const SWModule *module) {
    if (option) {
        char token[2112]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        int wordNum = 1;
        char wordstr[5];
        std::string modName = (module)?module->getName():"";
        // add TR to w src in KJV then remove this next line
        std::string wordSrcPrefix = (modName == "KJV")?std::string("TR"):modName;

        VerseKey const * vkey = 0;
        if (key) {
            vkey = dynamic_cast<VerseKey const *>(key);
        }

        const std::string orig = text;
        const char * from = orig.c_str();

        for (text = ""; *from; ++from) {
            if (*from == '<') {
                intoken = true;
                tokpos = 0;
                token[0] = 0;
                token[1] = 0;
                token[2] = 0;
                continue;
            }
            if (*from == '>') {	// process tokens
                intoken = false;
                if ((*token == 'w') && (token[1] == ' ')) {	// Word
                    XMLTag wtag(token);
                    sprintf(wordstr, "%03d", wordNum);
                    std::string lemmaClass;
                    std::string lemma;
                    std::string morph;
                    std::string page;
                    std::string src;
                    char gh = 0;
                    page = module->getEntryAttributes()["Word"][wordstr]["Page"].c_str();
                    if (page.length()) page = (std::string)"p:" + page;
                    int count = atoi(module->getEntryAttributes()["Word"][wordstr]["PartCount"].c_str());
                    for (int i = 0; i < count; i++) {

                        // for now, lemma class can just be equal to last lemma class in multi part word
                        std::string tmp = "LemmaClass";
                        if (count > 1) tmp += formatted(".%d", i+1);
                        lemmaClass = module->getEntryAttributes()["Word"][wordstr][tmp];

                        tmp = "Lemma";
                        if (count > 1) tmp += formatted(".%d", i+1);
                        tmp = (module->getEntryAttributes()["Word"][wordstr][tmp].c_str());

                        // if we're strongs,
                        if (lemmaClass == "strong") {
                            gh = tmp[0];
                            tmp.erase(0u, 1u);
                        }
                        if (lemma.size()) lemma += "|";
                        lemma += tmp;

                        tmp = "Morph";
                        if (count > 1) tmp += formatted(".%d", i+1);
                        tmp = (module->getEntryAttributes()["Word"][wordstr][tmp].c_str());
                        if (morph.size()) morph += "|";
                        morph += tmp;

                        tmp = "Src";
                        if (count > 1) tmp += formatted(".%d", i+1);
                        tmp = (module->getEntryAttributes()["Word"][wordstr][tmp].c_str());
                        if (!tmp.length()) tmp += formatted("%d", wordNum);
                        tmp.insert(0, wordSrcPrefix);
                        if (src.size()) src += "|";
                        src += tmp;
                    }

                    std::string lexName = "";
                    // we can pass the real lex name in, but we have some
                    // aliases in the javascript to optimize bandwidth
                    if ((gh == 'G') && (defaultGreekLex)) {
                        lexName = (!strcmp(defaultGreekLex->getName(), "StrongsGreek"))?"G":defaultGreekLex->getName();
                    }
                    else if ((gh == 'H') && (defaultHebLex)) {
                        lexName = (!strcmp(defaultHebLex->getName(), "StrongsHebrew"))?"H":defaultHebLex->getName();
                    }

                    std::string xlit = wtag.getAttribute("xlit");

                    if ((lemmaClass != "strong") && (hasPrefix(xlit, "betacode:"))) {
                        lexName = "betacode";
//						const char *m = strchr(xlit.c_str(), ':');
//						strong = ++m;
                    }
                    std::string wordID;
                    if (vkey) {
                        // optimize for bandwidth and use only the verse as the unique entry id
                        wordID += formatted("%d", vkey->getVerse());
                    }
                    else {
                        wordID = key->getText();
                    }
                    wordID += formatted("_%s", src.c_str());
                    // clean up our word ID for XHTML
                    for (unsigned int i = 0; i < wordID.size(); i++) {
                        if ((!isdigit(wordID[i])) && (!isalpha(wordID[i]))) {
                            wordID[i] = '_';
                        }
                    }
                    // 'p' = 'fillpop' to save bandwidth
                    text += formatted("<span class=\"clk\" onclick=\"p('%s','%s','%s','%s','%s','%s');\" >", lexName.c_str(), lemma.c_str(), wordID.c_str(), morph.c_str(), page.c_str(), modName.c_str());
                    wordNum++;

                    if (wtag.isEmpty()) {
                        text += "</w></span>";
                    }
                }
                if ((*token == '/') && (token[1] == 'w') && option) {	// Word
                    text += "</w></span>";
                    continue;
                }

                // if not a strongs token, keep token in text
                text.push_back('<');
                text.append(token);
                text.push_back('>');

                continue;
            }
            if (intoken) {
                if (tokpos < 2045) {
                    token[tokpos++] = *from;
                    token[tokpos+2] = 0;
                }
            }
            else	{
                text.push_back(*from);
            }
        }
    }
    return 0;
}

} /* namespace swordxx */
