/******************************************************************************
 *
 *  osisstrongs.cpp -    SWFilter descendant to hide or show Strong's number
 *            in a OSIS module
 *
 * $Id$
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
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilstr.h"
#include "../utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Strong's Numbers";
    static const char oTip[]  = "Toggles Strong's Numbers On and Off if they exist";

}

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

    const std::string orig = text;
    const char * from = orig.c_str();

    for (text = ""; *from; ++from) {
        if (*from == '<') {
            intoken = true;
            token = "";
            continue;
        }
        if (*from == '>') {    // process tokens
            intoken = false;

            // possible page seg --------------------------------
            if (hasPrefix(token, "seg ")) {
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

            if (hasPrefix(token, "w ")) {    // Word
                XMLTag wtag(token.c_str());

                // Always save off lemma if we haven't yet:
                if (!wtag.attribute("savlm").empty()) {
                    auto const lemma(wtag.attribute("lemma"));
                    if (!lemma.empty())
                        wtag.setAttribute("savlm", lemma.c_str());
                }

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

                    std::string attrib;
                    sprintf(wordstr, "%03d", wordNum);

                    // why is morph entry attribute processing done in here?  Well, it's faster.  It makes more local sense to place this code in osismorph.
                    // easier to keep lemma and morph in same wordstr number too maybe.
                    if (!(attrib = wtag.attribute("morph")).empty()) {
                        int count = wtag.attributePartCount("morph", ' ');
                        int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                        do {
                            std::string mClass = "";
                            std::string mp = "";
                            attrib = wtag.attribute("morph", i, ' ');
                            if (i < 0) i = 0;    // to handle our -1 condition

                            const char *m = std::strchr(attrib.c_str(), ':');
                            if (m) {
                                int len = m-attrib.c_str();
                                mClass.append(attrib.c_str(), len);
                                attrib.erase(0, len+1);
                            }
                            if ((mClass == "x-Robinsons") || (mClass == "x-Robinson") || (mClass == "Robinson")) {
                                mClass = "robinson";
                            }
                            if (i) { morphClass += " "; morph += " "; }
                            mp += attrib;
                            morphClass += mClass;
                            morph += mp;
                            if (count > 1) {
                                std::string tmp(formatted("Morph.%d", i+1));
                                module->getEntryAttributes()["Word"][wordstr][tmp] = mp;
                                tmp = formatted("MorphClass.%d", i+1);
                                module->getEntryAttributes()["Word"][wordstr][tmp] = mClass;
                            }
                        } while (++i < count);
                    }

                    if (!(attrib = wtag.attribute("savlm")).empty()) {
                        int count = wtag.attributePartCount("savlm", ' ');
                        int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                        do {
                            gh = 0;
                            std::string lClass = "";
                            std::string l = "";
                            attrib = wtag.attribute("savlm", i, ' ');
                            if (i < 0) i = 0;    // to handle our -1 condition

                            const char *m = std::strchr(attrib.c_str(), ':');
                            if (m) {
                                int len = m-attrib.c_str();
                                lClass.append(attrib.c_str(), len);
                                attrib.erase(0u, len+1);
                            }
                            if ((lClass == "x-Strongs") || (lClass == "strong") || (lClass == "Strong")) {
                                if (charIsDigit(attrib[0])) {
                                    if (vkey) {
                                        gh = vkey->getTestament() ? 'H' : 'G';
                                    }
                                }
                                else {
                                    gh = *attrib.begin();
                                    attrib.erase(0u, 1u);
                                }
                                lClass = "strong";
                            }
                            if (gh) l += gh;
                            l += attrib;
                            if (i) { lemmaClass += " "; lemma += " "; }
                            lemma += l;
                            lemmaClass += lClass;
                            if (count > 1) {
                                std::string tmp(formatted("Lemma.%d", i+1));
                                module->getEntryAttributes()["Word"][wordstr][tmp] = l;
                                tmp = formatted("LemmaClass.%d", i+1);
                                module->getEntryAttributes()["Word"][wordstr][tmp] = lClass;
                            }
                        } while (++i < count);
                        module->getEntryAttributes()["Word"][wordstr]["PartCount"] = formatted("%d", count);
                    }

                    if (!(attrib = wtag.attribute("src")).empty()) {
                        int count = wtag.attributePartCount("src", ' ');
                        int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                        do {
                            std::string mp = "";
                            attrib = wtag.attribute("src", i, ' ');
                            if (i < 0) i = 0;    // to handle our -1 condition

                            if (i) src += " ";
                            mp += attrib;
                            src += mp;
                            if (count > 1) {
                                std::string tmp(formatted("Src.%d", i+1));
                                module->getEntryAttributes()["Word"][wordstr][tmp] = mp;
                            }
                        } while (++i < count);
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
                if (!option) {
                    int count = wtag.attributePartCount("lemma", ' ');
                    for (int i = 0; i < count; ++i) {
                        auto a(wtag.attribute("lemma", i, ' '));
                        auto const prefix(stripPrefix(a, ':'));
                        if (!prefix.empty() && ((prefix == "x-Strongs") || (prefix == "strong") || (prefix == "Strong"))) {
                            // remove attribute part
                            wtag.setAttribute("lemma", nullptr, i, ' ');
                            --i;
                            --count;
                        }
                    }
                }
                token = wtag.toString();
                trimString(token);
                // drop <>:
                assert(token.size() >= 2u);
                assert((*token.begin()) == '<');
                assert((*token.rbegin()) == '>');
                token.pop_back();
                token.erase(0, 1u);
            }
            if (hasPrefix(token, "/w")) {    // Word End
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
            text.push_back('<');
            text.append(token);
            text.push_back('>');

            continue;
        }
        if (intoken) {
            token += *from;
        }
        else    {
            text.push_back(*from);
        }
    }
    return 0;
}

} /* namespace swordxx */
