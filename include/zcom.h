/******************************************************************************
 *  zcom.h   - code for class 'zCom'- a module that reads compressed text
 *				files: ot and nt using indexs ??.vss
 *
 * $Id: zcom.h,v 1.3 2001/02/09 15:38:51 jansorg Exp $
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

#ifndef ZCOM_H
#define ZCOM_H

#include <rawverse.h>
#include <swcom.h>
#include <swcomprs.h>

#include <defs.h>

class SWDLLEXPORT zCom:public RawVerse, public SWCom, public SWCompress
{
public:
  zCom (char *ipath, char *iname = 0, char *idesc = 0, SWDisplay * idisp = 0);
    virtual ~ zCom ();
  virtual operator char *();
};


#endif
