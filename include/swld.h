/******************************************************************************
 *  swld.h   - code for base class 'SWLD'.  SWLD is the basis for all
 *				types of Lexicon and Dictionary modules (hence the 'LD').
 *
 * $Id: swld.h,v 1.3 2001/02/09 15:38:51 jansorg Exp $
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

class SWDLLEXPORT SWLD:public SWModule
{
protected:
  char *entkeytxt;
public:
  
    
    SWLD (const char *imodname = 0, const char *imoddesc =
	  0, SWDisplay * idisp = 0);
    virtual ~ SWLD ();
  virtual SWKey *CreateKey ()
  {
    return new StrKey ();
  }
  virtual const char *KeyText (char *ikeytext);
};


#endif
