/******************************************************************************
 *
 * $Id: utf8transliterator.h,v 1.19 2003/07/07 00:25:59 scribe Exp $
 *
 * Copyright 2001 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

enum scriptEnum {SE_OFF, SE_LATIN, /*one-way (to) transliterators*/ SE_BASICLATIN, SE_BETA, SE_BGREEK, /*two-way transliterators*/ SE_GREEK, SE_HEBREW, SE_CYRILLIC, SE_ARABIC, SE_SYRIAC, SE_KATAKANA, SE_HIRAGANA, SE_JAMO, SE_HANGUL, SE_DEVANAGARI, SE_TAMIL, SE_BENGALI, SE_GURMUKHI, SE_GUJARATI, SE_ORIYA, SE_TELUGU, SE_KANNADA, SE_MALAYALAM, SE_THAI, SE_GEORGIAN, SE_ARMENIAN, SE_ETHIOPIC, SE_GOTHIC, SE_UGARITIC, SE_COPTIC, /*one-way (from) transliterators*/ SE_HAN, SE_KANJI};
#define NUMSCRIPTS 32
#define NUMTARGETSCRIPTS 5

#include <swfilter.h>
#include <swmodule.h>

//#include <unicode/utypes.h>
//#include <unicode/ucnv.h>
//#include <unicode/ustring.h>
//#include <unicode/uchar.h>

#include <unicode/unistr.h>

//#ifdef _ICUSWORD_
//#include <unicode/utrans.h>
//#else
#include <unicode/translit.h>
//#endif

#include <defs.h>
#include <map>


//class UnicodeString;

SWORD_NAMESPACE_START

struct SWTransData {
	UnicodeString resource;
	UTransDirection dir;
};
typedef std::map <const UnicodeString, SWTransData> SWTransMap;
typedef std::pair<UnicodeString, SWTransData> SWTransPair;

  /** This Filter uses ICU for transliteration
  */
class SWDLLEXPORT UTF8Transliterator : public SWFilter {
private:

	char option;

	static const char optionstring[NUMTARGETSCRIPTS][16];

	static const char optName[];
	static const char optTip[];
	static const char SW_RB_RULE_BASED_IDS[];
	static const char SW_RB_RULE[];
	static const char SW_RESDATA[];
	StringList options;
	static SWTransMap transMap;
	UErrorCode utf8status;

	void Load(UErrorCode &status);
	void registerTrans(const UnicodeString& ID, const UnicodeString& resource, UTransDirection dir, UErrorCode &status);
	bool checkTrans(const UnicodeString& ID, UErrorCode &status);
	Transliterator *createTrans(const UnicodeString& preID, const UnicodeString& ID, const UnicodeString& postID, UTransDirection dir, UErrorCode &status);

public:
	UTF8Transliterator();
	virtual char processText(SWBuf &text, const SWKey *key = 0, const SWModule *module = 0);
	virtual const char *getOptionName() { return optName; }
	virtual const char *getOptionTip() { return optTip; }
	virtual void setOptionValue(const char *ival);
	virtual const char *getOptionValue();
	virtual StringList getOptionValues() { return options; }
};

SWORD_NAMESPACE_END
#endif
