/******************************************************************************
 *  swdisprtf.h   - definition of Class SWDispRTF-- an SWDisplay used to display
 *				a verse in Rich Text Format.
 *
 * $Id: swdisprtf.h,v 1.2 1999/09/30 20:51:36 scribe Exp $
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
//---------------------------------------------------------------------------

#define	defRTFHeader          "{\\rtf1\\ansi{\\fonttbl{\\f0\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f1\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f2\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f3\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f4\\froman\\fcharset0\\fprq2 Times New Roman;}{\\f7\\froman\\fcharset2\\fprq2 Symbol;}{\\f8\\froman\\fcharset2\\fprq2 Symbol;}}{\\colortbl;\\red0\\green0\\blue255;\\red0\\green200\\blue50;\\red0\\green0\\blue255;\\red0\\green200\\blue50;\\red0\\green0\\blue255;\\red255\\green0\\blue0;} "
#define	defRTFHeadMargin	  "{\\fs8\\cf1\\par\\pard} "
#define	defRTFTrailer         "{\\fs%d \\par }}"	// 24

#define	defRTFChapterMarkPre  "\\pard \\qc\\nowidctlpar{\\f1\\fs%d\\b Chapter " // 30
#define	defRTFChapterMarkPost "\\par\\fs%d\\par}"	// 10

#define	defRTFVerseMarkPre    "{\\fs%d\\cf1\\super "	//20
#define	defRTFVerseMarkPost   "}"

#define	defRTFVersePre        "{\\fs%d"	// 24
#define	defRTFVersePost       " }"


class SWDispRTF : public TRichEdit {
protected:

		System::AnsiString FRTFHeader;
		System::AnsiString FRTFHeadMargin;
		System::AnsiString FRTFTrailer;
		System::AnsiString FRTFChapterMarkPre;
		System::AnsiString FRTFChapterMarkPost;
		System::AnsiString FRTFVerseMarkPre;
		System::AnsiString FRTFVerseMarkPost;
		System::AnsiString FRTFVersePre;
		System::AnsiString FRTFVersePost;
		bool FExpandNewLine;
		CHARRANGE FCharRange;
		int fontSize;
		
		int __fastcall GetMySelStart();
		void __fastcall SetMySelStart(int iselstart);
		int __fastcall getFontSize();
		void __fastcall setFontSize(int iFontSize);
		void recalcHeaders();
protected:
		TMemoryStream *RTFStream;
		
		virtual void __fastcall Loaded(void);
public:
	__fastcall SWDispRTF(TComponent* Owner);
	__fastcall ~SWDispRTF();

	virtual char Display(SWModule &imodule);
__published:
		__property System::AnsiString RTFHeader          = {read=FRTFHeader,           write=FRTFHeader};
		__property System::AnsiString RTFHeadMargin      = {read=FRTFHeadMargin,       write=FRTFHeadMargin};
		__property System::AnsiString RTFChapterMarkPre  = {read=FRTFChapterMarkPre,   write=FRTFChapterMarkPre};
		__property System::AnsiString RTFChapterMarkPost = {read=FRTFChapterMarkPost,  write=FRTFChapterMarkPost};
		__property System::AnsiString RTFVerseMarkPre    = {read=FRTFVerseMarkPre,     write=FRTFVerseMarkPre};
		__property System::AnsiString RTFVerseMarkPost   = {read=FRTFVerseMarkPost,    write=FRTFVerseMarkPost};
		__property System::AnsiString RTFVersePre        = {read=FRTFVersePre,         write=FRTFVersePre};
		__property System::AnsiString RTFVersePost       = {read=FRTFVersePost,        write=FRTFVersePost};
		__property System::AnsiString RTFTrailer         = {read=FRTFTrailer,          write=FRTFTrailer};
		__property bool               ExpandNewLine      = {read=FExpandNewLine,       write=FExpandNewLine};
		__property int                FontSize           = {read=getFontSize,          write=setFontSize};
};
//---------------------------------------------------------------------------
#endif

