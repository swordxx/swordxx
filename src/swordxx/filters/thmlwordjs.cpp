/******************************************************************************
 *
 *  thmlwordjs.cpp -    SWFilter descendant to ???
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

#include "thmlwordjs.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Word Javascript";
    static const char oTip[]  = "Toggles Word Javascript data";

}

ThMLWordJS::ThMLWordJS()
    : OffOnOptionFilter(oName, oTip)
{
     m_defaultGreekLex   = nullptr;
     m_defaultHebLex     = nullptr;
     m_defaultGreekParse = nullptr;
     m_defaultHebParse   = nullptr;
     m_mgr               = nullptr;
}

ThMLWordJS::~ThMLWordJS() {
}


char ThMLWordJS::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    if (isOptionOn()) {
        char token[2112]; // cheese.  Fix.
        int tokpos = 0;
        bool intoken = false;
        int word = 1;
        char val[128];
        char *valto;
        char *ch;
        char wordstr[11u];
        unsigned int textStart = 0, lastAppendLen = 0, textEnd = 0;
        std::string tmp;
        bool newText = false;
        bool needWordOut = false;
        AttributeValue * wordAttrs = nullptr;
        std::string modName = (module)?module->getName():"";
        std::string wordSrcPrefix = modName;

        std::string out;
        VerseKey const * const vkey = dynamic_cast<VerseKey const *>(key);

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
                    valto = val;
                    for (unsigned int i = 27; token[i] != '\"' && i < 150; i++)
                        *valto++ = token[i];
                    *valto = 0;
                    if (std::atoi((!charIsDigit(*val))?val+1:val) < 5627) {
                        // normal strongs number
                        sprintf(wordstr, "%03d", word++);
                        needWordOut = (word > 2);
                        wordAttrs = &(module->getEntryAttributes()["Word"][wordstr]);
                        (*wordAttrs)["Strongs"] = val;
    //printf("Adding: [\"Word\"][%s][\"Strongs\"] = %s\n", wordstr, val);
                        tmp = "";
                        tmp.append(out.c_str()+textStart, (int)(textEnd - textStart));
                        (*wordAttrs)["Text"] = tmp;
                        out.append("</span>");
                        std::string ts;
                        ts += formatted("%d", textStart);
                        (*wordAttrs)["TextStart"] = ts;
    //printf("Adding: [\"Word\"][%s][\"Text\"] = %s\n", wordstr, tmp.c_str());
                        newText = true;
                    }
                    else {
                        // verb morph
                        (*wordAttrs)["Morph"] = val;
    //printf("Adding: [\"Word\"][%s][\"Morph\"] = %s\n", wordstr, val);
                    }

                }
                if (startsWith(token, "sync type=\"morph\""sv)) {
                    for (ch = token+17; *ch; ch++) {
                        if (startsWith(ch, "class=\""sv)) {
                            valto = val;
                            for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                                *valto++ = ch[i];
                            *valto = 0;
                            (*wordAttrs)["MorphClass"] = val;
    //printf("Adding: [\"Word\"][%s][\"MorphClass\"] = %s\n", wordstr, val);
                        }
                        if (startsWith(ch, "value=\""sv)) {
                            valto = val;
                            for (unsigned int i = 7; ch[i] != '\"' && i < 127; i++)
                                *valto++ = ch[i];
                            *valto = 0;
                            (*wordAttrs)["Morph"] = val;
    //printf("Adding: [\"Word\"][%s][\"Morph\"] = %s\n", wordstr, val);
                        }
                    }
                    newText = true;
                }
                // if not a strongs token, keep token in text
                out += '<';
                out += token;
                out += '>';
                if (needWordOut) {
                    char wstr[11u];
                    sprintf(wstr, "%03d", word-2);
                    AttributeValue *wAttrs = &(module->getEntryAttributes()["Word"][wstr]);
                    needWordOut = false;
                    std::string strong = (*wAttrs)["Strongs"];
                    std::string morph = (*wAttrs)["Morph"];
                    std::string morphClass = (*wAttrs)["MorphClass"];
                    std::string wordText = (*wAttrs)["Text"];
                    std::string textSt = (*wAttrs)["TextStart"];
                    if (strong.size()) {
                        char gh = 0;
                        gh = charIsDigit(strong[0]) ? 0:strong[0];
                        if (!gh) {
                            if (vkey) {
                                gh = vkey->getTestament() ? 'H' : 'G';
                            }
                        }
                        else strong.erase(0u, 1u);

                        SWModule * sLex = nullptr;
                        SWModule * sMorph = nullptr;
                        if (gh == 'G') {
                            sLex = m_defaultGreekLex;
                            sMorph = m_defaultGreekParse;
                        }
                        if (gh == 'H') {
                            sLex = m_defaultHebLex;
                            sMorph = m_defaultHebParse;
                        }
                        std::string lexName = "";
                        if (sLex) {
                            // we can pass the real lex name in, but we have some
                            // aliases in the javascript to optimize bandwidth
                            lexName = sLex->getName();
                            if (lexName == "StrongsGreek")
                                lexName = "G";
                            if (lexName == "StrongsHebrew")
                                lexName = "H";
                        }
                        std::string wordID;
                        if (vkey) {
                            // optimize for bandwidth and use only the verse as the unique entry id
                            wordID += formatted("%d", vkey->getVerse());
                        }
                        else {
                            wordID = key->getText();
                        }
                        for (unsigned int i = 0; i < wordID.size(); i++) {
                            if ((!charIsDigit(wordID[i])) && (!charIsAlpha(wordID[i]))) {
                                wordID[i] = '_';
                            }
                        }
                        wordID += formatted("_%s%d", wordSrcPrefix, std::atoi(wstr));
                        if (textSt.size()) {
                            int textStr = std::atoi(textSt.c_str());
                            textStr += lastAppendLen;
                            std::string spanStart = "";



                            if (!sMorph)
                                sMorph = nullptr; // avoid unused warnings for now
/*
                            if (sMorph) {
                                std::string popMorph = "<a onclick=\"";
                                popMorph.appendFormatted("p(\'%s\',\'%s\','%s','');\" >%s</a>", sMorph->getName(), morph.c_str(), wordID.c_str(), morph.c_str());
                                morph = popMorph;
                            }
*/

                            // 'p' = 'fillpop' to save bandwidth
                            const char *m = std::strchr(morph.c_str(), ':');
                            if (m) m++;
                            else m = morph.c_str();
                            spanStart += formatted("<span class=\"clk\" onclick=\"p('%s','%s','%s','%s','','%s');\" >", lexName, strong, wordID, m, modName);
                            out.insert(textStr, spanStart);
                            lastAppendLen = spanStart.length();
                        }
                    }

                }
                if (newText) {
                    textStart = out.size(); newText = false;
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
            else	{
                out += *from;
            }
        }

        char wstr[11u];
        sprintf(wstr, "%03d", word-1);
        AttributeValue *wAttrs = &(module->getEntryAttributes()["Word"][wstr]);
        needWordOut = false;
        std::string strong = (*wAttrs)["Strongs"];
        std::string morph = (*wAttrs)["Morph"];
        std::string morphClass = (*wAttrs)["MorphClass"];
        std::string wordText = (*wAttrs)["Text"];
        std::string textSt = (*wAttrs)["TextStart"];
        if (strong.size()) {
            char gh = 0;
            gh = charIsDigit(strong[0]) ? 0:strong[0];
            if (!gh) {
                if (vkey) {
                    gh = vkey->getTestament() ? 'H' : 'G';
                }
            }
            else strong.erase(0u, 1u);

            SWModule * sLex = nullptr;
            if (gh == 'G') {
                sLex = m_defaultGreekLex;
            }
            if (gh == 'H') {
                sLex = m_defaultHebLex;
            }
            std::string lexName = "";
            if (sLex) {
                // we can pass the real lex name in, but we have some
                // aliases in the javascript to optimize bandwidth
                lexName = sLex->getName();
                if (lexName == "StrongsGreek")
                    lexName = "G";
                if (lexName == "StrongsHebrew")
                    lexName = "H";
            }
            std::string wordID;
            if (vkey) {
                // optimize for bandwidth and use only the verse as the unique entry id
                wordID += formatted("%d", vkey->getVerse());
            }
            else {
                wordID = key->getText();
            }
            for (unsigned int i = 0; i < wordID.size(); i++) {
                if ((!charIsDigit(wordID[i])) && (!charIsAlpha(wordID[i]))) {
                    wordID[i] = '_';
                }
            }
            wordID += formatted("_%s%d", wordSrcPrefix, std::atoi(wstr));
            if (textSt.size()) {
                int textStr = std::atoi(textSt.c_str());
                textStr += lastAppendLen;
                std::string spanStart = "";
                // 'p' = 'fillpop' to save bandwidth
                const char *m = std::strchr(morph.c_str(), ':');
                if (m) m++;
                else m = morph.c_str();
                spanStart += formatted("<span class=\"clk\" onclick=\"p('%s','%s','%s','%s','','%s');\" >", lexName, strong, wordID, m, modName);
                out.insert(textStr, spanStart);
            }
        }
        text = std::move(out);
    }

    return 0;
}

} /* namespace swordxx */
