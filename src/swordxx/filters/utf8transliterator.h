/******************************************************************************
 *
 *  utf8transliterator.h -    Implementation of UTF8Transliterator
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

#ifndef UTF8TRANSLITERATOR_H
#define UTF8TRANSLITERATOR_H

#include "../swoptfilter.h"

#include <map>
#include <list>
#include <string>
#include <unicode/translit.h>
#include <unicode/unistr.h>
#include "../defs.h"


enum scriptEnum {SE_OFF, SE_LATIN,
/*one-way (to) transliterators*/
SE_IPA, SE_BASICLATIN, SE_SBL, SE_TC, SE_BETA, SE_BGREEK, SE_SERA, SE_HUGOYE, SE_UNGEGN, SE_ISO, SE_ALALC, SE_BGN,
/*two-way transliterators*/
SE_GREEK, SE_HEBREW, SE_CYRILLIC, SE_ARABIC, SE_SYRIAC, SE_KATAKANA, SE_HIRAGANA, SE_HANGUL, SE_DEVANAGARI, SE_TAMIL, SE_BENGALI, SE_GURMUKHI, SE_GUJARATI, SE_ORIYA, SE_TELUGU, SE_KANNADA, SE_MALAYALAM, SE_THAI, SE_GEORGIAN, SE_ARMENIAN, SE_ETHIOPIC, SE_GOTHIC, SE_UGARITIC, SE_COPTIC, SE_MEROITIC, SE_LINEARB, SE_CYPRIOT, SE_RUNIC, SE_OGHAM, SE_THAANA, SE_GLAGOLITIC, SE_CHEROKEE,
/*SE_TENGWAR, SE_CIRTH,*/
/*one-way (from) transliterators*/
SE_JAMO, SE_HAN, SE_KANJI
};

#define NUMSCRIPTS 49
#define NUMTARGETSCRIPTS 2 //NUMSCRIPTS-3//6

namespace swordxx {

class SWModule;

struct SWTransData {
    UnicodeString resource;
    UTransDirection dir;
};
typedef std::map<const UnicodeString, SWTransData> SWTransMap;
typedef std::pair<UnicodeString, SWTransData> SWTransPair;

/** This Filter uses ICU for transliteration
*/
class SWDLLEXPORT UTF8Transliterator : public SWOptionFilter {
private:

    unsigned char option;

    static const char optionstring[NUMTARGETSCRIPTS][16];

    static const char optName[];
    static const char optTip[];
    std::list<std::string> options;

#ifdef ICU_CUSTOM_RESOURCE_BUILDING
    static const char SW_RB_RULE_BASED_IDS[];
    static const char SW_RB_RULE[];
    static const char SW_RESDATA[];
    static SWTransMap transMap;
    UErrorCode utf8status;

    void Load(UErrorCode &status);
    void registerTrans(const UnicodeString& ID, const UnicodeString& resource, UTransDirection dir, UErrorCode &status);
    bool checkTrans(const UnicodeString& ID, UErrorCode &status);
#endif
    bool addTrans(const char* newTrans, std::string* transList);
    Transliterator *createTrans(const UnicodeString& ID, UTransDirection dir, UErrorCode &status);

public:
    UTF8Transliterator();
    ~UTF8Transliterator() override;

    char processText(std::string & text,
                         SWKey const * key = nullptr,
                         SWModule const * module = nullptr) override;

    char const * getOptionName() override { return optName; }
    char const * getOptionTip() override { return optTip; }
    void setOptionValue(char const * ival) override;
    char const * getOptionValue() override;
    std::list<std::string> getOptionValues() override { return options; }

};

} /* namespace swordxx */
#endif
