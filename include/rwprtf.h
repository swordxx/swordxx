/******************************************************************************
 *  rwprtf.h   - definition of Class RWPRTF, an SWFilter used to convert
 *				special tags in Robertsons Word Pictures commentary into
 *				Rich Text Format tags
 *
 * $Id: rwprtf.h,v 1.5 2001/02/09 15:38:51 jansorg Exp $
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

#ifndef RWPRTF_H
#define RWPRTF_H

#include <swfilter.h>

#include <defs.h>

class SWDLLEXPORT RWPRTF:public SWFilter
{
public:
  RWPRTF ();
  virtual char ProcessText (char *text, int maxlen, const SWKey * key);
};

#endif
