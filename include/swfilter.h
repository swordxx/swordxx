/******************************************************************************
 *  swfilter.h	- definition of class SWFilter used to filter text between
 *				different formats
 *
 * $Id: swfilter.h,v 1.8 2001/02/16 20:19:22 jansorg Exp $
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

/**
* The type definitoin for option types
*/
typedef std::list < std::string > OptionsList;


/** Base class for all filters in sword.
* Filters are used to filter/convert text between different formats
* like GBF, HTML, RTF ...
*/
class SWDLLEXPORT  SWFilter{
public:
  /** the type definition for option lists,
  * see @ref getOptionValues
  */
//  typedef std::list < std::string > OptionsList;
  /** gets the name of the option of this filter
  * @return option name
  */
  virtual const char * getOptionName ()
  {
    return 0;
  }
  /** gets a short explanation of the option of this filter;
  * it could be presented to the user in frontend programs
  * @return option tip/explanation
  */
  virtual const char * getOptionTip ()
  {
    return 0;
  }
  /** returns a list of the possible option values
  * might be
  * @return list of option values
  */
  virtual OptionsList getOptionValues ()
  {
    OptionsList options;
    return options;
  }
  /** sets the value of the option of this filter,
  * e.g maybe a strong's filter mioght be set to "on" / "off" -
  * that would mean to show or not to show the strongs in the text,
  * see also @ref getOptionValues
  * @param ival the new option valus
  */
  virtual void setOptionValue (const char *ival)
  {
  }
  virtual const char * getOptionValue ()
  {
    return 0;
  }
  /** This is the main filter function
  * @param text the text to be filtered/converted
  * @param maxlen maximal length of text to be processed
  * @param key sorry I don't know
  * @return 0
  */
  virtual char ProcessText (char *text, int maxlen, const SWKey * key)
  {
    return ProcessText (text, maxlen);
  }
  /** This is the main filter function without the 3rd parameter
  * @param text the text to be filtered/converted
  * @param maxlen maximal length of text to be processed
  * @return 0
  */
  virtual char ProcessText (char *text, int maxlen = -1)
  {
    return ProcessText (text, maxlen, 0);
  }
};

#endif
