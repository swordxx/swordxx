/******************************************************************************
 *  swdisprtf.h   - definition of Class SWDispRTF-- an SWDisplay used to display
 *				a verse in Rich Text Format.
 *
 * $Id$
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
#ifndef SWDispRTFH
#define SWDispRTFH
//---------------------------------------------------------------------------
#include <vcl\SysUtils.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Forms.hpp>
#include <vcl\ComCtrls.hpp>
#include <vcl\StdCtrls.hpp>
#include <swtext.h>
#include "RxRichEditX.h"
//---------------------------------------------------------------------------

#define	defRTFHeader          "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f2\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f3\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f4\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f7\\froman\\fcharset2\\fprq2 Symbol;}{\\f8\\froman\\fcharset2\\fprq2 Symbol;}}{\\colortbl;\\red0\\green0\\blue255;\\red0\\green200\\blue50;\\red0\\green0\\blue255;\\red0\\green200\\blue50;\\red0\\green0\\blue255;\\red255\\green0\\blue0;} "
#define	defRTFHeadMargin	  "{\\fs8\\cf1\\par\\pard} "
#define	defRTFTrailer         "{\\fs%d \\par }}"	// 24

#define	defRTFChapterMarkPre  "\\pard \\qc\\nowidctlpar{\\f1\\cf7\\fs%d\\b Chapter "	// 30
#define	defRTFChapterMarkPost "\\par\\fs%d\\par}"	// 10

#define	defRTFVerseMarkPre    "{\\fs%d\\cf1\\super "	//20
#define	defRTFVerseMarkPost   "}"

#define	defRTFVersePre        "{\\fs%d"	// 24
#define	defRTFVersePost       " }"

class SWDispRTF : public TRxRichEditX {
protected:

  bool FExpandNewLine;
  CHARRANGE FCharRange;
  int fontSize;

  int __fastcall GetMySelStart ();
  void __fastcall SetMySelStart (int iselstart);
  int __fastcall getFontSize ();
  void __fastcall setFontSize (int iFontSize);
protected:
    TMemoryStream * RTFStream;

  virtual void __fastcall Loaded (void);
public:
    __fastcall SWDispRTF (TWinControl *Owner);
    __fastcall ~ SWDispRTF ();

  virtual char Display (SWModule & imodule);
  __property bool ExpandNewLine = { read = FExpandNewLine, write = FExpandNewLine };
};

//---------------------------------------------------------------------------
#endif
