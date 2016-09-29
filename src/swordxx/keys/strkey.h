/******************************************************************************
 *
 *  strkey.h -    code for class 'strkey'- a standard Biblical verse key
 *
 * $Id$
 *
 * Copyright 1997-2013 CrossWire Bible Society (http://www.crosswire.org)
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


#ifndef STRKEY_H
#define STRKEY_H

#include "../swkey.h"

#include "../defs.h"


namespace swordxx {

  /** a standard string key class (used
  * for modules that index on single strings (eg. cities,
  * names, words, etc.)
  */
class SWDLLEXPORT StrKey:public SWKey
{
public:
  /** initializes instance of StrKey
  *
  * @param ikey text key (word, city, name, etc.)
  */
  StrKey(char const * ikey = nullptr);
  /** cleans up instance of StrKey
  */
  virtual ~ StrKey ();

  SWKey * clone() const noexcept override;

  SWKEY_OPERATORS(StrKey)

};
} /* namespace swordxx */

#endif
