/******************************************************************************
 *  swdisprtfchap.h	- definition of class SWDispRTFChap, an SWDisplay used to
 *				display an entire chapter in Rich Text Format
 *
 * $Id: swdisprtfchap.h,v 1.3 2001/11/08 13:20:24 chrislit Exp $
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
#ifndef swdisprtfchapH
#define swdisprtfchapH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\StdCtrls.hpp>
#include <swtext.h>
#include <swdisprtf.h>
//---------------------------------------------------------------------------

class SWDispRTFChap:public SWDispRTF
{
protected:

  bool FCurVerseMrk;
  bool FCurVersePos;
  bool FMarkCurrentVerse;
  bool FAutoPosition;
  
  char platformID;

  virtual void __fastcall Loaded (void);
public:
    __fastcall SWDispRTFChap (TComponent * Owner);
    __fastcall ~ SWDispRTFChap ();

  virtual char Display (SWModule & imodule);
    __published:__property bool CurVersePos =
  {
  read = FCurVersePos, write = FCurVersePos, default = true};
    __property bool CurVerseMrk = { read = FCurVerseMrk, write =
      FCurVerseMrk, default = true };
  __property bool MarkCurrentVerse = { read = FMarkCurrentVerse, write =
      FMarkCurrentVerse, default = true };
  __property bool AutoPosition = { read = FAutoPosition, write =
      FAutoPosition, default = true };
};

//---------------------------------------------------------------------------
#endif
