/******************************************************************************
 *  swwinlog.h   - definition of SWWinLog-- a derivitive of SWLog used to
 *			handle logging in the MSWindows environment
 *
 * $Id: swwinlog.h,v 1.3 2002/10/03 04:26:46 scribe Exp $
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

//---------------------------------------------------------------------------
#ifndef swwinlogH
#define swwinlogH
//---------------------------------------------------------------------------

#include <swlog.h>

SWORD_NAMESPACE_START

class SWWinLog:public SWLog
{
  HWND parent;
public:
    SWWinLog (HWND iparent);
  virtual void LogWarning (char *fmt, ...);
  virtual void LogError (char *fmt, ...);
  virtual void LogTimedInformation (char *fmt, ...);
  virtual void LogInformation (char *fmt, ...);
};

SWORD_NAMESPACE_END
#endif
