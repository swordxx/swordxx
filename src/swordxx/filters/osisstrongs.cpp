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

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include "../keys/versekey.h"
#include "../swmodule.h"
#include "../utilfuns/utilxml.h"


namespace swordxx {

namespace {

    static const char oName[] = "Strong's Numbers";
    static const char oTip[]  = "Toggles Strong's Numbers On and Off if they exist";

    static const StringList *oValues() {
        static const std::string choices[3] = {"Off", "On", ""};
        static const StringList oVals(&choices[0], &choices[2]);
        return &oVals;
    }
}


OSISStrongs::OSISStrongs() : SWOptionFilter(oName, oTip, oValues()) {
}


OSISStrongs::~OSISStrongs() {
}


char OSISStrongs::processText(std::string &text, const SWKey *key, const SWModule *module) {
    std::string token;
    bool intoken = false;
    int wordNum = 1;
    char wordstr[5];
    const char *wordStart = 0;
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
                std::string type = stag.getAttribute("type");
                if (type == "page") {
                    std::string number = stag.getAttribute("subtype");
                    if (number.length()) {
                        page = number;
                    }
                }
            }
            // ---------------------------------------------------

            if (hasPrefix(token, "w ")) {    // Word
                XMLTag wtag(token.c_str());
                if (module->isProcessEntryAttributes()) {
                    wordStart = from+1;
                    char gh = 0;
                    VerseKey *vkey = 0;
                    if (key) {
                        vkey = SWDYNAMIC_CAST(VerseKey, key);
                    }
                    std::string lemma      = "";
                    std::string morph      = "";
                    std::string src        = "";
                    std::string morphClass = "";
                    std::string lemmaClass = "";

                    std::string attrib;
                    sprintf(wordstr, "%03d", wordNum);

                    // why is morph entry attribute processing done in here?  Well, it's faster.  It makes more local sense to place this code in osismorph.
                    // easier to keep lemma and morph in same wordstr number too maybe.
                    if (!(attrib = wtag.getAttribute("morph")).empty()) {
                        int count = wtag.getAttributePartCount("morph", ' ');
                        int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                        do {
                            std::string mClass = "";
                            std::string mp = "";
                            attrib = wtag.getAttribute("morph", i, ' ');
                            if (i < 0) i = 0;    // to handle our -1 condition

                            const char *m = strchr(attrib.c_str(), ':');
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

                    if (!(attrib = wtag.getAttribute("lemma")).empty()) {
                        int count = wtag.getAttributePartCount("lemma", ' ');
                        int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                        do {
                            gh = 0;
                            std::string lClass = "";
                            std::string l = "";
                            attrib = wtag.getAttribute("lemma", i, ' ');
                            if (i < 0) i = 0;    // to handle our -1 condition

                            const char *m = strchr(attrib.c_str(), ':');
                            if (m) {
                                int len = m-attrib.c_str();
                                lClass.append(attrib.c_str(), len);
                                attrib.erase(0u, len+1);
                            }
                            if ((lClass == "x-Strongs") || (lClass == "strong") || (lClass == "Strong")) {
                                if (isdigit(attrib[0])) {
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

                    if (!(attrib = wtag.getAttribute("src")).empty()) {
                        int count = wtag.getAttributePartCount("src", ' ');
                        int i = (count > 1) ? 0 : -1;        // -1 for whole value cuz it's faster, but does the same thing as 0
                        do {
                            std::string mp = "";
                            attrib = wtag.getAttribute("src", i, ' ');
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
                        for (j = token.length()-1; ((j>0) && (strchr(" /", token[j]))); j--);
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

                if (!option) {
/*
 * Code which handles multiple lemma types.  Kindof works but breaks at least WEBIF filters for strongs.
 *
                    int count = wtag.getAttributePartCount("lemma", ' ');
                    for (int i = 0; i < count; i++) {
                        std::string a = wtag.getAttribute("lemma", i, ' ');
                        const char *prefix = a.stripPrefix(':');
                        if ((prefix) && (!strcmp(prefix, "x-Strongs") || !strcmp(prefix, "strong") || !strcmp(prefix, "Strong"))) {
                            // remove attribute part
                            wtag.setAttribute("lemma", 0, i, ' ');
                            i--;
                            count--;
                        }
                    }
* Instead the codee below just removes the lemma attribute
*****/
                    auto savlm(wtag.getAttribute("lemma"));
                    if (!savlm.empty()) {
                        wtag.setAttribute("lemma", 0);
                        wtag.setAttribute("savlm", savlm.c_str());
                        token = wtag;
                        trimString(token);
                        // drop <>
                        token.erase(0u, 1u);
                        token.pop_back();
                    }
                }
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
                wordStart = 0;
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
