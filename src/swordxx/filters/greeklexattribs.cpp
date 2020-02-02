/******************************************************************************
 *
 *  greeklexattribs.cpp -    SWFilter descendant to set entry attributes
 *                for greek lexicons
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "greeklexattribs.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include "../swmodule.h"
#include "../utilstr.h"


using std::string;

namespace swordxx {

GreekLexAttribs::GreekLexAttribs()
    : SWOptionFilter(
          "Greek lexicon entry attributes",
          "Sets entry attributes for Greek lexicons",
          {"On"})
{}


char GreekLexAttribs::processText(std::string &text, const SWKey *key, const SWModule *module) {
    using namespace std::literals::string_view_literals;

    (void) key;

    if (module->isProcessEntryAttributes()) {
        const char *from;
        bool inAV = false;
        string phrase;
        string freq;
        char val[128], *valto;
        char wordstr[11u];
        char const * currentPhrase = nullptr;
        char const * currentPhraseEnd = nullptr;
        int number = 0;


        for (from = text.c_str(); *from; from++) {
            if (inAV) {
                if (currentPhrase == nullptr) {
                    if (charIsAlpha(*from))
                        currentPhrase = from;
                }
                else {
                    if ((!charIsAlpha(*from)) && (*from != ' ') && (*from != '+') && (*from !='(') && (*from != ')') && (*from != '\'')) {
                        if (*from == '<') {
                            if (!currentPhraseEnd)
                                currentPhraseEnd = from - 1;
                            for (; *from && *from != '>'; from++) {
                                if (startsWith(from, "value=\""sv)) {
                                    valto = val;
                                    from += 7;
                                    for (unsigned int i = 0; from[i] != '\"' && i < 127; i++)
                                        *valto++ = from[i];
                                    *valto = 0;
                                    sprintf(wordstr, "%03d", number+1);
                                    module->getEntryAttributes()["AVPhrase"][wordstr]["CompoundedWith"] = val;
                                    from += std::strlen(val);
                                }
                            }
                            continue;
                        }

                        phrase = "";
                        phrase.append(currentPhrase, (int)(((currentPhraseEnd>currentPhrase)?currentPhraseEnd:from) - currentPhrase)-1);
                        currentPhrase = from;
                        while (*from && charIsDigit(*from)) from++;
                        freq = "";
                        freq.append(currentPhrase, (int)(from - currentPhrase));
                        if ((freq.length() > 0) && (phrase.length() > 0)) {
                            sprintf(wordstr, "%03d", ++number);
                            if ((std::strchr(phrase.c_str(), '(') > phrase.c_str()) && (std::strchr(phrase.c_str(), ')') > phrase.c_str() + 1)) {
                                string tmp = phrase.substr(0, phrase.find_first_of("("));
                                phrase.erase(phrase.find_first_of("("), 1);
                                phrase.erase(phrase.find_first_of(")"), 1);
                                phrase.erase(0,phrase.find_first_not_of("\r\n\v\t ")); phrase.erase(phrase.find_last_not_of("\r\n\v\t ")+1);
                                module->getEntryAttributes()["AVPhrase"][wordstr]["Alt"] = phrase;
                                phrase = tmp;
                            }
                            phrase.erase(0,phrase.find_first_not_of("\r\n\v\t ")); phrase.erase(phrase.find_last_not_of("\r\n\v\t ")+1);
                            freq.erase(0,freq.find_first_not_of("\r\n\v\t ")); freq.erase(freq.find_last_not_of("\r\n\v\t ")+1);
                            module->getEntryAttributes()["AVPhrase"][wordstr]["Phrase"] = phrase;
                            module->getEntryAttributes()["AVPhrase"][wordstr]["Frequency"] = freq;
                            currentPhrase = nullptr;
                            currentPhraseEnd = nullptr;
                        }
                    }
                }
                if (*from == ';') inAV = false;

            }
            else if (startsWith(from, "AV-"sv)) {
                inAV = true;
                from+=2;
            }
        }
    }
    return 0;
}


} /* namespace swordxx */
