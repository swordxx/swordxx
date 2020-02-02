/******************************************************************************
 *
 *  osiswordjs.cpp -    SWFilter descendant for ???
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

}

OSISWordJS::OSISWordJS()
    : OffOnOptionFilter(oName,
                        oTip)
{
     m_defaultGreekLex   = nullptr;
     m_defaultHebLex     = nullptr;
     m_defaultGreekParse = nullptr;
     m_defaultHebParse   = nullptr;
     m_mgr               = nullptr;
}

OSISWordJS::~OSISWordJS() {
}


char OSISWordJS::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    if (isOptionOn()) {
        char token[2112]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        int wordNum = 1;
        char wordstr[11u];
        std::string modName = (module)?module->getName():"";
        // add TR to w src in KJV then remove this next line
        std::string wordSrcPrefix = (modName == "KJV")?std::string("TR"):modName;

        VerseKey const * vkey = nullptr;
        if (key) {
            vkey = dynamic_cast<VerseKey const *>(key);
        }

        std::string out;

        for (auto const * from = text.c_str(); *from; ++from) {
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
                    int count = std::atoi(module->getEntryAttributes()["Word"][wordstr]["PartCount"].c_str());
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
                    if ((gh == 'G') && (m_defaultGreekLex)) {
                        lexName = (m_defaultGreekLex->getName() == "StrongsGreek")?"G":m_defaultGreekLex->getName();
                    }
                    else if ((gh == 'H') && (m_defaultHebLex)) {
                        lexName = (m_defaultHebLex->getName() == "StrongsHebrew")?"H":m_defaultHebLex->getName();
                    }

                    std::string xlit = wtag.attribute("xlit");

                    if ((lemmaClass != "strong") && (startsWith(xlit, "betacode:"sv))) {
                        lexName = "betacode";
//						const char *m = std::strchr(xlit.c_str(), ':');
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
                    wordID.append(1u, '_').append(src);
                    // clean up our word ID for XHTML
                    for (unsigned int i = 0; i < wordID.size(); i++) {
                        if ((!charIsDigit(wordID[i])) && (!charIsAlpha(wordID[i]))) {
                            wordID[i] = '_';
                        }
                    }
                    // 'p' = 'fillpop' to save bandwidth
                    out += formatted("<span class=\"clk\" onclick=\"p('%s','%s','%s','%s','%s','%s');\" >", lexName, lemma, wordID, morph, page, modName);
                    wordNum++;

                    if (wtag.isEmpty()) {
                        out += "</w></span>";
                    }
                }
                if ((*token == '/') && (token[1] == 'w') && isOptionOn()) {	// Word
                    out += "</w></span>";
                    continue;
                }

                // if not a strongs token, keep token in text
                out.push_back('<');
                out.append(token);
                out.push_back('>');

                continue;
            }
            if (intoken) {
                if (tokpos < 2045) {
                    token[tokpos++] = *from;
                    token[tokpos+2] = 0;
                }
            }
            else	{
                out.push_back(*from);
            }
        }
        text = std::move(out);
    }
    return 0;
}

} /* namespace swordxx */
