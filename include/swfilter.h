/******************************************************************************
 *  swfilter.h	- definition of class SWFilter used to filter text between
 *				different formats
 *
 * $Id: swfilter.h,v 1.6 2001/02/09 15:38:51 jansorg Exp $
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef SWFILTER_H
#define SWFILTER_H
#include <string>
#include <list>
#include <swkey.h>

#include <defs.h>

typedef
  std::list < std::string > OptionsList;

class SWDLLEXPORT
  SWFilter
{
public:
  virtual const char *
  getOptionName ()
  {
    return 0;
  }
  virtual const char *
  getOptionTip ()
  {
    return 0;
  }
  virtual OptionsList
  getOptionValues ()
  {
    OptionsList
      options;
    return options;
  }
  virtual void
  setOptionValue (const char *ival)
  {
  }
  virtual const char *
  getOptionValue ()
  {
    return 0;
  }
  virtual char
  ProcessText (char *text, int maxlen, const SWKey * key)
  {
    return ProcessText (text, maxlen);
  }
  virtual char
  ProcessText (char *text, int maxlen =
	       -1) { return ProcessText (text, maxlen, 0);
  }
};

#endif
