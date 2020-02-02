/******************************************************************************
 *
 *  osisstrongs.cpp -    SWFilter descendant to hide or show Strong's number
 *            in a OSIS module
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "osisstrongs.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../keys/versekey.h"
#include "../SimpleTokenizer.h"
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Strong's Numbers";
    static const char oTip[]  = "Toggles Strong's Numbers On and Off if they exist";

template <typename Input>
std::vector<std::string> split(Input const & input, char const separator) {
    std::vector<std::string> r;
    for (auto const & token : SimpleTokenizer<>::tokenize(input, separator))
        r.emplace_back(token);
    return r;
}

} // anonymous namespace

OSISStrongs::OSISStrongs()
    : OffOnOptionFilter(oName, oTip)
{}

OSISStrongs::~OSISStrongs() {
}


char OSISStrongs::processText(std::string &text, const SWKey *key, const SWModule *module) {
    std::string token;
    bool intoken = false;
    int wordNum = 1;
    char wordstr[11u];
    char const * wordStart = nullptr;
    std::string page = "";        // some modules include <seg> page info, so we add these to the words

    std::string out;

    using namespace std::literals::string_view_literals;

    for (auto const * from = text.c_str(); *from; ++from) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }
        if (*from == '>') {    // process tokens
            intoken = false;

            // possible page seg --------------------------------
            if (startsWith(token, "seg "sv)) {
                XMLTag stag(token.c_str());
                std::string type = stag.attribute("type");
                if (type == "page") {
                    std::string number = stag.attribute("subtype");
                    if (number.length()) {
                        page = number;
                    }
                }
            }
            // ---------------------------------------------------

            if (startsWith(token, "w "sv)) {    // Word
                XMLTag wtag(token.c_str());

                // Always save off lemma if we haven't yet:
                if (!wtag.attribute("savlm").empty())
                    if (auto lemma = wtag.attribute("lemma"); !lemma.empty())
                        wtag.attributes()["savlm"] = std::move(lemma);

                if (module->isProcessEntryAttributes()) {
                    wordStart = from+1;
                    char gh = 0;
                    VerseKey const * const vkey =
                            dynamic_cast<VerseKey const *>(key);
                    std::string lemma      = "";
                    std::string morph      = "";
                    std::string src        = "";
                    std::string morphClass = "";
                    std::string lemmaClass = "";

                    sprintf(wordstr, "%03d", wordNum);

                    // why is morph entry attribute processing done in here?  Well, it's faster.  It makes more local sense to place this code in osismorph.
                    // easier to keep lemma and morph in same wordstr number too maybe.
                    if (auto attrib = wtag.attribute("morph"); !attrib.empty()) {
                        auto const morphTokens(split(attrib, ' '));
                        std::size_t i = 0u;

                        for (std::string_view morphToken : morphTokens) {
                            std::string mClass = "";
                            std::string mp = "";

                            if (auto const pos = morphToken.find(':');
                                pos != std::string_view::npos)
                            {
                                mClass.append(morphToken.data(), pos);
                                morphToken.remove_prefix(pos + 1u);
                            }
                            if ((mClass == "x-Robinsons") || (mClass == "x-Robinson") || (mClass == "Robinson")) {
                                mClass = "robinson";
                            }
                            if (i) {
                                morphClass.append(1u, ' ');
                                morph.append(1u, ' ');
                            }
                            mp += morphToken;
                            morphClass += mClass;
                            morph += mp;
                            if (morphTokens.size() > 1u) {
                                auto const iStr(std::to_string(i + 1));
                                auto & c =
                                    module->getEntryAttributes()["Word"][wordstr];
                                c["Morph." + iStr] = mp;
                                c["MorphClass." + iStr] = mClass;
                            }
                            ++i;
                        }
                    }

                    if (auto attrib = wtag.attribute("savlm"); !attrib.empty()) {
                        auto const savlmTokens(split(attrib, ' '));
                        std::size_t i = 0u;

                        for (std::string_view savlmToken : savlmTokens) {
                            gh = 0;
                            std::string lClass = "";
                            std::string l = "";

                            if (auto const pos = savlmToken.find(':');
                                pos != std::string_view::npos)
                            {
                                lClass.append(savlmToken.data(), pos);
                                savlmToken.remove_prefix(pos + 1u);
                            }
                            if ((lClass == "x-Strongs") || (lClass == "strong") || (lClass == "Strong")) {
                                if (charIsDigit(savlmToken[0])) {
                                    if (vkey) {
                                        gh = vkey->getTestament() ? 'H' : 'G';
                                    }
                                }
                                else {
                                    gh = savlmToken.front();
                                    savlmToken.remove_prefix(1u);
                                }
                                lClass = "strong";
                            }
                            if (gh) l += gh;
                            l += savlmToken;
                            if (i) { lemmaClass += " "; lemma += " "; }
                            lemma += l;
                            lemmaClass += lClass;
                            if (savlmTokens.size() > 1u) {
                                auto const iStr(std::to_string(i + 1));
                                auto & c =
                                    module->getEntryAttributes()["Word"][wordstr];
                                c["Lemma." + iStr] = l;
                                c["LemmaClass." + iStr] = lClass;
                            }
                            ++i;
                        }
                        module->getEntryAttributes()["Word"][wordstr]["PartCount"] =
                                std::to_string(savlmTokens.size());
                    }

                    if (auto attrib = wtag.attribute("src"); !attrib.empty()) {
                        auto const srcTokens(split(attrib, ' '));
                        std::size_t i = 0u;
                        for (auto const & srcToken : srcTokens) {
                            if (i)
                                src.append(1u, ' ');
                            src += srcToken;
                            if (srcTokens.size() > 1u) {
                                auto & c =
                                    module->getEntryAttributes()["Word"][wordstr];
                                c["Src." + std::to_string(i + 1)] = srcToken;
                            }
                            ++i;
                        }
                    }


                    if (lemma.length())
                        module->getEntryAttributes()["Word"][wordstr]["Lemma"] = lemma;
                    if (lemmaClass.length())
                        module->getEntryAttributes()["Word"][wordstr]["LemmaClass"] = lemmaClass;
                    if (morph.length())
                        module->getEntryAttributes()["Word"][wordstr]["Morph"] = morph;
                    if (morphClass.length())
                        module->getEntryAttributes()["Word"][wordstr]["MorphClass"] = morphClass;
                    if (src.length())
                        module->getEntryAttributes()["Word"][wordstr]["Src"] = src;
                    if (page.length())
                        module->getEntryAttributes()["Word"][wordstr]["Page"] = page;

                    if (wtag.isEmpty()) {
                        int j;
                        for (j = token.length()-1; ((j>0) && (std::strchr(" /", token[j]))); j--);
                        token.resize(j + 1, '\0');
                    }

                    token += " wn=\"";
                    token += wordstr;
                    token += "\"";

                    if (wtag.isEmpty()) {
                        token += "/";
                    }

                    wordNum++;
                }

                // If we won't want strongs, then lets get them out of lemma:
                if (isOptionOff()) {
                    auto const oldAttrib(wtag.attribute("lemma"));
                    std::string newAttrib;
                    for (auto const & lemmaToken
                         : SimpleTokenizer<>::tokenize(oldAttrib, ' '))
                    {
                        if (!startsWith(lemmaToken, "x-Strongs:")
                            && !startsWith(lemmaToken, "strong:")
                            && !startsWith(lemmaToken, "Strong:"))
                        {
                            if (newAttrib.empty()) {
                                newAttrib = lemmaToken;
                            } else {
                                newAttrib.append(1u, ' ').append(lemmaToken);
                            }
                        }

                    }
                    wtag.attributes()["lemma"] = std::move(newAttrib);
                }
                token = wtag.toString();
                trimString(token);
                // drop <>:
                assert(token.size() >= 2u);
                assert(token.front() == '<');
                assert(token.back() == '>');
                token.pop_back();
                token.erase(0, 1u);
            }
            if (startsWith(token, "/w"sv)) {    // Word End
                if (module->isProcessEntryAttributes()) {
                    if (wordStart) {
                        std::string tmp;
                        tmp.append(wordStart, (from-wordStart)-3);
                        sprintf(wordstr, "%03d", wordNum-1);
                        module->getEntryAttributes()["Word"][wordstr]["Text"] = tmp;
                    }
                }
                wordStart = nullptr;
            }

            // keep token in text
            out.push_back('<');
            out.append(token);
            out.push_back('>');

            continue;
        }
        if (intoken) {
            token += *from;
        }
        else    {
            out.push_back(*from);
        }
    }
    text = std::move(out);
    return 0;
}

} /* namespace swordxx */
