/******************************************************************************
 *  swld.h   - code for base class 'SWLD'.  SWLD is the basis for all
 *				types of Lexicon and Dictionary modules (hence the 'LD').
 *
 * $Id: swld.h,v 1.6 2001/06/17 21:20:08 scribe Exp $
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

#ifndef SWLD_H
#define SWLD_H

#include <swmodule.h>
#include <strkey.h>

#include <defs.h>

  /** the basis for all types of Lexicon and
  * Dictionary modules (hence the 'LD').
  */
class SWDLLEXPORT SWLD:public SWModule
{
protected:
  char *entkeytxt;
public:
  /** Initializes data for instance of SWLD
  *
  * @param imodname Internal name for module
  * @param imoddesc Name to display to user for module
  * @param idisp Display object to use for displaying
  */
  SWLD (const char *imodname = 0, const char *imoddesc = 0, SWDisplay * idisp = 0, bool unicode = false);
  virtual ~ SWLD ();
  virtual SWKey *CreateKey ()
  {
    return new StrKey ();
  }
  /** Sets/gets module KeyText, getting from saved text if key is persistent
  *
  * @param ikeytext value which to set keytext;
  *  [0] - only get
  * @return pointer to keytext
  */
  virtual const char *KeyText (const char *ikeytext);
};


#endif
