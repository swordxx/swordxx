/*
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
#ifdef ICU

#ifndef UTF8TRANSLITERATOR_H
#define UTF8TRANSLITERATOR_H

enum scriptEnum {SE_OFF, SE_LATIN, SE_GREEK, SE_HEBREW, SE_CYRILLIC, SE_ARABIC, SE_SYRIAC, SE_KANA, SE_JAMO, SE_HANGUL, SE_DEVANAGARI, SE_TAMIL, SE_BENGALI, SE_GURMUKHI, SE_GUJARATI, SE_ORIYA, SE_TELUGU, SE_KANNADA, SE_MALAYALAM, /*one-way (to) transliterators*/ SE_MALTESE, SE_BETA, SE_BGREEK, SE_MCHEBREW, SE_MCSYRIAC, /*one-way (from) transliterators*/ SE_HAN, SE_KANJI};
#define NUMSCRIPTS 26

#include <swfilter.h>

#include <defs.h>

  /** This Filter shows/hides strong's numbers in a GBF text
  */
class SWDLLEXPORT UTF8Transliterator : public SWFilter
{
  char option;

  static const char optionstring[][NUMSCRIPTS];

  static const char optName[];
  static const char optTip[];
  OptionsList options;
 public:
  char lang[2];
  UTF8Transliterator ();
  virtual ~ UTF8Transliterator ();
  virtual char ProcessText (char *text, int maxlen, const SWKey * key);
  virtual const char *getOptionName ()
    {
      return optName;
    }
  virtual const char *getOptionTip ()
    {
      return optTip;
    }
  virtual void setOptionValue (const char *ival);
  virtual const char *getOptionValue ();
  virtual OptionsList getOptionValues ()
    {
      return options;
    }
};

#endif

#endif
