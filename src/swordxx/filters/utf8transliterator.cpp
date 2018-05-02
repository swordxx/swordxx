/******************************************************************************
 *
 *  utf8transliterator.cpp -    SWFilter descendant to transliterate between
 *                ICU-supported scripts
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

#include "utf8transliterator.h"

#if SWORDXX_HAS_ICU

#include <cstdlib>
#include <unicode/resbund.h>
#include <unicode/uchar.h>
#include <unicode/ucnv.h>
#include "../swlog.h"
#include "../swmodule.h"
#include "../utilstr.h"


namespace swordxx {

const char UTF8Transliterator::optionstring[NUMTARGETSCRIPTS][16] = {
        "Off",
        "Latin",
        /*
        "IPA",
        "Basic Latin",
        "SBL",
        "TC",
        "Beta",
        "BGreek",
        "SERA",
        "Hugoye",
        "UNGEGN",
        "ISO",
        "ALA-LC",
        "BGN",
        "Greek",
        "Hebrew",
        "Cyrillic",
        "Arabic",
        "Syriac",
        "Katakana",
        "Hiragana",
        "Hangul",
        "Devanagari",
        "Tamil",
        "Bengali",
        "Gurmukhi",
        "Gujarati",
        "Oriya",
        "Telugu",
        "Kannada",
        "Malayalam",
        "Thai",
        "Georgian",
        "Armenian",
        "Ethiopic",
        "Gothic",
        "Ugaritic",
        "Coptic",
        "Linear B",
        "Cypriot",
        "Runic",
        "Ogham",
        "Thaana",
        "Glagolitic",
        "Cherokee",
        */
};

const char UTF8Transliterator::optName[] = "Transliteration";
const char UTF8Transliterator::optTip[] = "Transliterates between scripts";

UTF8Transliterator::UTF8Transliterator() {
    m_utf8TransliteratorOption = 0;
        unsigned long i;
    for (i = 0; i < NUMTARGETSCRIPTS; i++) {
        options.push_back(optionstring[i]);
    }
}


UTF8Transliterator::~UTF8Transliterator() {
}

bool UTF8Transliterator::addTrans(const char* newTrans, std::string* transList) {
        *transList += newTrans;
        *transList += ";";
        return true;
}


std::unique_ptr<Transliterator> UTF8Transliterator::createTrans(
        UnicodeString const & ID,
        UTransDirection /* dir */,
        UErrorCode & status)
{
    std::unique_ptr<Transliterator> r(
                Transliterator::createInstance(ID, UTRANS_FORWARD, status));
    if (U_FAILURE(status))
        return nullptr;
    return r;
}

void UTF8Transliterator::setOptionValue(const char *ival)
{
    unsigned char i = m_utf8TransliteratorOption = NUMTARGETSCRIPTS;
    while (i && !caseInsensitiveEquals(ival, optionstring[i])) {
        i--;
        m_utf8TransliteratorOption = i;
    }
}

const char *UTF8Transliterator::getOptionValue()
{ return (NUMTARGETSCRIPTS > m_utf8TransliteratorOption) ? optionstring[m_utf8TransliteratorOption] : nullptr; }

char UTF8Transliterator::processText(std::string &text, const SWKey *key, const SWModule *module)
{
    (void) key;
    if (m_utf8TransliteratorOption) {    // if we want transliteration
        unsigned long i, j;
                UErrorCode err = U_ZERO_ERROR;
                UConverter * conv = nullptr;
                conv = ucnv_open("UTF-8", &err);
                std::string ID;

                bool compat = false;

        // Convert UTF-8 string to UTF-16 (UChars)
                j = std::strlen(text.c_str());
                int32_t len = (j * 2) + 1;
                auto const source(std::make_unique<UChar[]>(len));
                err = U_ZERO_ERROR;
                len = ucnv_toUChars(conv, source.get(), len, text.c_str(), j, &err);
                source[len] = 0;

        // Figure out which scripts are used in the string
        unsigned char scripts[NUMSCRIPTS];

                for (i = 0; i < NUMSCRIPTS; i++) {
                        scripts[i] = false;
                }

                for (i = 0; i < (unsigned long)len; i++) {
                        j = ublock_getCode(source[i]);
            scripts[SE_LATIN] = true;
            switch (j) {
            //case UBLOCK_BASIC_LATIN: scripts[SE_LATIN] = true; break;
            case UBLOCK_GREEK: scripts[SE_GREEK] = true; break;
            case UBLOCK_HEBREW: scripts[SE_HEBREW] = true; break;
            case UBLOCK_CYRILLIC: scripts[SE_CYRILLIC] = true; break;
            case UBLOCK_ARABIC: scripts[SE_ARABIC] = true; break;
            case UBLOCK_SYRIAC: scripts[SE_SYRIAC] = true; break;
            case UBLOCK_KATAKANA: scripts[SE_KATAKANA] = true; break;
            case UBLOCK_HIRAGANA: scripts[SE_HIRAGANA] = true; break;
            case UBLOCK_HANGUL_SYLLABLES: scripts[SE_HANGUL] = true; break;
            case UBLOCK_HANGUL_JAMO: scripts[SE_JAMO] = true; break;
            case UBLOCK_DEVANAGARI: scripts[SE_DEVANAGARI] = true; break;
            case UBLOCK_TAMIL: scripts[SE_TAMIL] = true; break;
            case UBLOCK_BENGALI: scripts[SE_BENGALI] = true; break;
            case UBLOCK_GURMUKHI: scripts[SE_GURMUKHI] = true; break;
            case UBLOCK_GUJARATI: scripts[SE_GUJARATI] = true; break;
            case UBLOCK_ORIYA: scripts[SE_ORIYA] = true; break;
            case UBLOCK_TELUGU: scripts[SE_TELUGU] = true; break;
            case UBLOCK_KANNADA: scripts[SE_KANNADA] = true; break;
            case UBLOCK_MALAYALAM: scripts[SE_MALAYALAM] = true; break;
            case UBLOCK_THAI: scripts[SE_THAI] = true; break;
            case UBLOCK_GEORGIAN: scripts[SE_GEORGIAN] = true; break;
            case UBLOCK_ARMENIAN: scripts[SE_ARMENIAN] = true; break;
            case UBLOCK_ETHIOPIC: scripts[SE_ETHIOPIC] = true; break;
            case UBLOCK_GOTHIC: scripts[SE_GOTHIC] = true; break;
            case UBLOCK_UGARITIC: scripts[SE_UGARITIC] = true; break;
//            case UBLOCK_MEROITIC: scripts[SE_MEROITIC] = true; break;
            case UBLOCK_LINEAR_B_SYLLABARY: scripts[SE_LINEARB] = true; break;
            case UBLOCK_CYPRIOT_SYLLABARY: scripts[SE_CYPRIOT] = true; break;
            case UBLOCK_RUNIC: scripts[SE_RUNIC] = true; break;
            case UBLOCK_OGHAM: scripts[SE_OGHAM] = true; break;
            case UBLOCK_THAANA: scripts[SE_THAANA] = true; break;
            case UBLOCK_GLAGOLITIC: scripts[SE_GLAGOLITIC] = true; break;
                        case UBLOCK_CHEROKEE: scripts[SE_CHEROKEE] = true; break;
//            case UBLOCK_TENGWAR: scripts[SE_TENGWAR] = true; break;
//            case UBLOCK_CIRTH: scripts[SE_CIRTH] = true; break;
            case UBLOCK_CJK_RADICALS_SUPPLEMENT:
            case UBLOCK_KANGXI_RADICALS:
            case UBLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS:
            case UBLOCK_CJK_SYMBOLS_AND_PUNCTUATION:
            case UBLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A:
            case UBLOCK_CJK_UNIFIED_IDEOGRAPHS:
                                scripts[SE_HAN] = true;
                                break;
            case UBLOCK_CJK_COMPATIBILITY:
            case UBLOCK_CJK_COMPATIBILITY_IDEOGRAPHS:
            case UBLOCK_CJK_COMPATIBILITY_FORMS:
                                scripts[SE_HAN] = true;
                                compat = true;
                    break;
            case UBLOCK_HANGUL_COMPATIBILITY_JAMO:
                                scripts[SE_HANGUL] = true;
                                compat = true;
                                break;

                        //default: scripts[SE_LATIN] = true;
            }
        }
        scripts[m_utf8TransliteratorOption] = false; //turn off the reflexive transliteration

        //return if we have no transliteration to do for this text
                j = 0;
                   for (i = 0; !j && i < NUMSCRIPTS; i++) {
                if (scripts[i]) j++;
            }
               if (!j) {
                        ucnv_close(conv);
                        return 0;
                }

                if (compat) {
                        addTrans("NFKD", &ID);
                }
                else {
                        addTrans("NFD", &ID);
                }

        //Simple X to Latin transliterators
        if (scripts[SE_GREEK]) {
            if (strnicmp(((SWModule*)module)->getLanguage().c_str(), "cop", 3)) {
                if (m_utf8TransliteratorOption == SE_SBL)
                    addTrans("Greek-Latin/SBL", &ID);
                else if (m_utf8TransliteratorOption == SE_TC)
                    addTrans("Greek-Latin/TC", &ID);
                else if (m_utf8TransliteratorOption == SE_BETA)
                    addTrans("Greek-Latin/Beta", &ID);
                else if (m_utf8TransliteratorOption == SE_BGREEK)
                    addTrans("Greek-Latin/BGreek", &ID);
                            else if (m_utf8TransliteratorOption == SE_UNGEGN)
                                addTrans("Greek-Latin/UNGEGN", &ID);
                else if (m_utf8TransliteratorOption == SE_ISO)
                                addTrans("Greek-Latin/ISO", &ID);
                                else if (m_utf8TransliteratorOption == SE_ALALC)
                                addTrans("Greek-Latin/ALALC", &ID);
                                else if (m_utf8TransliteratorOption == SE_BGN)
                                addTrans("Greek-Latin/BGN", &ID);
                                else if (m_utf8TransliteratorOption == SE_IPA)
                                addTrans("Greek-IPA/Ancient", &ID);
                                else {
                                addTrans("Greek-Latin", &ID);
                    scripts[SE_LATIN] = true;
                                }
            }
            else {
                if (m_utf8TransliteratorOption == SE_SBL)
                    addTrans("Coptic-Latin/SBL", &ID);
                else if (m_utf8TransliteratorOption == SE_TC)
                    addTrans("Coptic-Latin/TC", &ID);
                else if (m_utf8TransliteratorOption == SE_BETA)
                    addTrans("Coptic-Latin/Beta", &ID);
                                else if (m_utf8TransliteratorOption == SE_IPA)
                                addTrans("Coptic-IPA", &ID);
                                else {
                                addTrans("Coptic-Latin", &ID);
                    scripts[SE_LATIN] = true;
                                }
            }
        }
        if (scripts[SE_HEBREW]) {
                        if (m_utf8TransliteratorOption == SE_SBL)
                                addTrans("Hebrew-Latin/SBL", &ID);
                        else if (m_utf8TransliteratorOption == SE_TC)
                                addTrans("Hebrew-Latin/TC", &ID);
            else if (m_utf8TransliteratorOption == SE_BETA)
                addTrans("Hebrew-Latin/Beta", &ID);
                        else if (m_utf8TransliteratorOption == SE_UNGEGN)
                                addTrans("Hebrew-Latin/UNGEGN", &ID);
                        else if (m_utf8TransliteratorOption == SE_ALALC)
                                addTrans("Hebrew-Latin/ALALC", &ID);
                        else if (m_utf8TransliteratorOption == SE_SYRIAC)
                                addTrans("Hebrew-Syriac", &ID);
            else {
                addTrans("Hebrew-Latin", &ID);
                                scripts[SE_LATIN] = true;
            }
        }
        if (scripts[SE_CYRILLIC]) {
                    if (m_utf8TransliteratorOption == SE_GLAGOLITIC)
                            addTrans("Cyrillic-Glagolitic", &ID);
                    else {
                addTrans("Cyrillic-Latin", &ID);
                scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_ARABIC]) {
            addTrans("Arabic-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_SYRIAC]) {
                        if (m_utf8TransliteratorOption == SE_TC)
                                addTrans("Syriac-Latin/TC", &ID);
                        else if (m_utf8TransliteratorOption == SE_BETA)
                    addTrans("Syriac-Latin/Beta", &ID);
                        else if (m_utf8TransliteratorOption == SE_HUGOYE)
                    addTrans("Syriac-Latin/Hugoye", &ID);
                        else if (m_utf8TransliteratorOption == SE_HEBREW)
                                addTrans("Syriac-Hebrew", &ID);
                        else {
                    addTrans("Syriac-Latin", &ID);
                    scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_THAI]) {
            addTrans("Thai-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_GEORGIAN]) {
                        if (m_utf8TransliteratorOption == SE_ISO)
                    addTrans("Georgian-Latin/ISO", &ID);
                        else if (m_utf8TransliteratorOption == SE_ALALC)
                    addTrans("Georgian-Latin/ALALC", &ID);
                        else if (m_utf8TransliteratorOption == SE_BGN)
                    addTrans("Georgian-Latin/BGN", &ID);
                        else if (m_utf8TransliteratorOption == SE_IPA)
                    addTrans("Georgian-IPA", &ID);
                        else {
                addTrans("Georgian-Latin", &ID);
                scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_ARMENIAN]) {
                        if (m_utf8TransliteratorOption == SE_ISO)
                    addTrans("Armenian-Latin/ISO", &ID);
                        else if (m_utf8TransliteratorOption == SE_ALALC)
                    addTrans("Armenian-Latin/ALALC", &ID);
                        else if (m_utf8TransliteratorOption == SE_BGN)
                    addTrans("Armenian-Latin/BGN", &ID);
                        else if (m_utf8TransliteratorOption == SE_IPA)
                    addTrans("Armenian-IPA", &ID);
                        else {
                addTrans("Armenian-Latin", &ID);
                scripts[SE_LATIN] = true;
                    }
        }
        if (scripts[SE_ETHIOPIC]) {
                        if (m_utf8TransliteratorOption == SE_UNGEGN)
                    addTrans("Ethiopic-Latin/UNGEGN", &ID);
                        else if (m_utf8TransliteratorOption == SE_ISO)
                    addTrans("Ethiopic-Latin/ISO", &ID);
                        else if (m_utf8TransliteratorOption == SE_ALALC)
                    addTrans("Ethiopic-Latin/ALALC", &ID);
                        else if (m_utf8TransliteratorOption == SE_SERA)
                    addTrans("Ethiopic-Latin/SERA", &ID);
                    else {
                addTrans("Ethiopic-Latin", &ID);
                scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_GOTHIC]) {
                        if (m_utf8TransliteratorOption == SE_BASICLATIN)
                    addTrans("Gothic-Latin/Basic", &ID);
                        else if (m_utf8TransliteratorOption == SE_IPA)
                    addTrans("Gothic-IPA", &ID);
                    else {
                addTrans("Gothic-Latin", &ID);
                scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_UGARITIC]) {
                    if (m_utf8TransliteratorOption == SE_SBL)
                            addTrans("Ugaritic-Latin/SBL", &ID);
                        else {
                addTrans("Ugaritic-Latin", &ID);
                scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_MEROITIC]) {
            addTrans("Meroitic-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_LINEARB]) {
            addTrans("LinearB-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_CYPRIOT]) {
            addTrans("Cypriot-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_RUNIC]) {
            addTrans("Runic-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_OGHAM]) {
            addTrans("Ogham-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_THAANA]) {
            if (m_utf8TransliteratorOption == SE_ALALC)
                            addTrans("Thaana-Latin/ALALC", &ID);
            else if (m_utf8TransliteratorOption == SE_BGN)
                            addTrans("Thaana-Latin/BGN", &ID);
            else {
                addTrans("Thaana-Latin", &ID);
                scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_GLAGOLITIC]) {
            if (m_utf8TransliteratorOption == SE_ISO)
                            addTrans("Glagolitic-Latin/ISO", &ID);
            else if (m_utf8TransliteratorOption == SE_ALALC)
                            addTrans("Glagolitic-Latin/ALALC", &ID);
            else if (m_utf8TransliteratorOption == SE_ALALC)
                            addTrans("Glagolitic-Cyrillic", &ID);
            else {
                addTrans("Glagolitic-Latin", &ID);
                scripts[SE_LATIN] = true;
                        }
        }
        if (scripts[SE_CHEROKEE]) {
            addTrans("Cherokee-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_THAI]) {
            addTrans("Thai-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_THAI]) {
            addTrans("Thai-Latin", &ID);
            scripts[SE_LATIN] = true;
        }

        if (scripts[SE_HAN]) {
                if (!strnicmp(((SWModule*)module)->getLanguage().c_str(), "ja", 2)) {
                     addTrans("Kanji-Romaji", &ID);
            }
            else {
                       addTrans("Han-Latin", &ID);
            }
            scripts[SE_LATIN] = true;
        }

               // Inter-Kana and Kana to Latin transliterators
        if (m_utf8TransliteratorOption == SE_HIRAGANA && scripts[SE_KATAKANA]) {
            addTrans("Katakana-Hiragana", &ID);
            scripts[SE_HIRAGANA] = true;
        }
        else if (m_utf8TransliteratorOption == SE_KATAKANA && scripts[SE_HIRAGANA]) {
            addTrans("Hiragana-Katakana", &ID);
            scripts[SE_KATAKANA] = true;
        }
        else {
                if (scripts[SE_KATAKANA]) {
                    addTrans("Katakana-Latin", &ID);
                    scripts[SE_LATIN] = true;
                }
                if (scripts[SE_HIRAGANA]) {
                    addTrans("Hiragana-Latin", &ID);
                    scripts[SE_LATIN] = true;
                }
                }

        // Korean to Latin transliterators
        if (scripts[SE_HANGUL]) {
            addTrans("Hangul-Latin", &ID);
            scripts[SE_LATIN] = true;
        }
        if (scripts[SE_JAMO]) {
            addTrans("Jamo-Latin", &ID);
            scripts[SE_LATIN] = true;
        }

        // Indic-Latin
        if (m_utf8TransliteratorOption < SE_DEVANAGARI || m_utf8TransliteratorOption > SE_MALAYALAM) {
            // Indic to Latin
            if (scripts[SE_TAMIL]) {
                addTrans("Tamil-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
            if (scripts[SE_BENGALI]) {
                addTrans("Bengali-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
            if (scripts[SE_GURMUKHI]) {
                addTrans("Gurmukhi-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
            if (scripts[SE_GUJARATI]) {
                addTrans("Gujarati-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
            if (scripts[SE_ORIYA]) {
                addTrans("Oriya-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
            if (scripts[SE_TELUGU]) {
                addTrans("Telugu-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
            if (scripts[SE_KANNADA]) {
                addTrans("Kannada-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
            if (scripts[SE_MALAYALAM]) {
                addTrans("Malayalam-Latin", &ID);
                scripts[SE_LATIN] = true;
            }
        }
        else {
            if (scripts[SE_LATIN]) {
                addTrans("Latin-InterIndic", &ID);
            }
            if (scripts[SE_DEVANAGARI]) {
                addTrans("Devanagari-InterIndic", &ID);
            }
            if (scripts[SE_TAMIL]) {
                addTrans("Tamil-InterIndic", &ID);
            }
            if (scripts[SE_BENGALI]) {
                addTrans("Bengali-InterIndic", &ID);
            }
            if (scripts[SE_GURMUKHI]) {
                addTrans("Gurmurkhi-InterIndic", &ID);
            }
            if (scripts[SE_GUJARATI]) {
                addTrans("Gujarati-InterIndic", &ID);
            }
            if (scripts[SE_ORIYA]) {
                addTrans("Oriya-InterIndic", &ID);
            }
            if (scripts[SE_TELUGU]) {
                addTrans("Telugu-InterIndic", &ID);
            }
            if (scripts[SE_KANNADA]) {
                addTrans("Kannada-InterIndic", &ID);
            }
            if (scripts[SE_MALAYALAM]) {
                addTrans("Malayalam-InterIndic", &ID);
            }

            switch(m_utf8TransliteratorOption) {
            case SE_DEVANAGARI:
                addTrans("InterIndic-Devanagari", &ID);
                break;
            case SE_TAMIL:
                addTrans("InterIndic-Tamil", &ID);
                break;
            case SE_BENGALI:
                addTrans("InterIndic-Bengali", &ID);
                break;
            case SE_GURMUKHI:
                addTrans("InterIndic-Gurmukhi", &ID);
                break;
            case SE_GUJARATI:
                addTrans("InterIndic-Gujarati", &ID);
                break;
            case SE_ORIYA:
                addTrans("InterIndic-Oriya", &ID);
                break;
            case SE_TELUGU:
                addTrans("InterIndic-Telugu", &ID);
                break;
            case SE_KANNADA:
                addTrans("InterIndic-Kannada", &ID);
                break;
            case SE_MALAYALAM:
                addTrans("InterIndic-Malayalam", &ID);
                break;
            default:
                addTrans("InterIndic-Latin", &ID);
                scripts[SE_LATIN] = true;
                break;
            }
        }

//        if (scripts[SE_TENGWAR]) {
//            addTrans("Tengwar-Latin", &ID);
//            scripts[SE_LATIN] = true;
//        }
//        if (scripts[SE_CIRTH]) {
//            addTrans("Cirth-Latin", &ID);
//            scripts[SE_LATIN] = true;
//        }

        if (scripts[SE_LATIN]) {
                switch (m_utf8TransliteratorOption) {
                        case SE_GREEK:
                addTrans("Latin-Greek", &ID);
                                break;
                        case SE_HEBREW:
                addTrans("Latin-Hebrew", &ID);
                                break;
                        case SE_CYRILLIC:
                addTrans("Latin-Cyrillic", &ID);
                                break;
                        case SE_ARABIC:
                addTrans("Latin-Arabic", &ID);
                                break;
                        case SE_SYRIAC:
                addTrans("Latin-Syriac", &ID);
                                break;
                        case SE_THAI:
                addTrans("Latin-Thai", &ID);
                                break;
                        case SE_GEORGIAN:
                addTrans("Latin-Georgian", &ID);
                                break;
                        case SE_ARMENIAN:
                addTrans("Latin-Armenian", &ID);
                                break;
                        case SE_ETHIOPIC:
                addTrans("Latin-Ethiopic", &ID);
                                break;
                        case SE_GOTHIC:
                addTrans("Latin-Gothic", &ID);
                                break;
                        case SE_UGARITIC:
                addTrans("Latin-Ugaritic", &ID);
                                break;
                        case SE_COPTIC:
                addTrans("Latin-Coptic", &ID);
                                break;
                        case SE_KATAKANA:
                addTrans("Latin-Katakana", &ID);
                                break;
                        case SE_HIRAGANA:
                addTrans("Latin-Hiragana", &ID);
                                break;
                        case SE_JAMO:
                addTrans("Latin-Jamo", &ID);
                                break;
                        case SE_HANGUL:
                addTrans("Latin-Hangul", &ID);
                                break;
                        case SE_MEROITIC:
                addTrans("Latin-Meroitic", &ID);
                                break;
                        case SE_LINEARB:
                addTrans("Latin-LinearB", &ID);
                                break;
                        case SE_CYPRIOT:
                addTrans("Latin-Cypriot", &ID);
                                break;
                        case SE_RUNIC:
                addTrans("Latin-Runic", &ID);
                                break;
                        case SE_OGHAM:
                addTrans("Latin-Ogham", &ID);
                                break;
                        case SE_THAANA:
                addTrans("Latin-Thaana", &ID);
                                break;
                        case SE_GLAGOLITIC:
                addTrans("Latin-Glagolitic", &ID);
                                break;
                        case SE_CHEROKEE:
                addTrans("Latin-Cherokee", &ID);
                                break;
//                        case SE_TENGWAR:
//                addTrans("Latin-Tengwar", &ID);
//                                break;
//                        case SE_CIRTH:
//                addTrans("Latin-Cirth", &ID);
//                                break;
                        }
                }

                if (m_utf8TransliteratorOption == SE_BASICLATIN) {
                        addTrans("Any-Latin1", &ID);
                }

                addTrans("NFC", &ID);

                err = U_ZERO_ERROR;
                if (auto const trans = createTrans(UnicodeString(ID.c_str()),
                                                   UTRANS_FORWARD,
                                                   err))
                {
                    if (!U_FAILURE(err)) {
                        UnicodeString target = UnicodeString(source.get());
                        trans->transliterate(target);
                        text.resize(text.size() * 2u, '\0');
                        len = ucnv_fromUChars(conv, &text[0u], text.size(), target.getBuffer(), target.length(), &err);
                        text.resize(len, '\0');
                    }
                }
                ucnv_close(conv);
        }
    return 0;
}

} /* namespace swordxx */
#endif



