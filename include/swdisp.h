/******************************************************************************
 *  swdisp.h  - code for base class 'swdisp'.  swdisp is the basis for all
 *		types of displays (e.g. raw textout, curses, xwindow, etc.)
 *
 * $Id: swdisp.h,v 1.3 2001/02/09 15:38:51 jansorg Exp $
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

#ifndef SWDISP_H
#define SWDISP_H

#include <defs.h>


class SWModule;

class SWDLLEXPORT SWDisplay
{
public:
  SWDisplay ()
  {
  };
  virtual char Display (SWModule & imodule);
};
#endif
