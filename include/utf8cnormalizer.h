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
#ifdef _ICU_

#ifndef UTF8CNORMALIZER_H
#define UTF8CNORMALIZER_H

#include <swfilter.h>

#include <unicode/utypes.h>
#include <unicode/convert.h>
#include <unicode/ustring.h>
#include <unicode/normlzr.h>

#include <defs.h>

  /** This Filter shows/hides strong's numbers in a GBF text
  */
class SWDLLEXPORT UTF8CNormalizer : public SWFilter
{
 public:
  virtual char ProcessText (char *text, int maxlen, const SWKey * key);
};

#endif

#endif
