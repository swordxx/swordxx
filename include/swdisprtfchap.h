/******************************************************************************
 *  swdisprtfchap.h	- definition of class SWDispRTFChap, an SWDisplay used to
 *				display an entire chapter in Rich Text Format
 *
 * $Id: swdisprtfchap.h,v 1.5 2002/07/30 11:33:17 scribe Exp $
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
class SWDispRTFChap_init;
class SWDispRTFChap:public SWDispRTF {
protected:
	virtual void __fastcall Loaded (void);
public:
	static char platformID;
	static SWDispRTFChap_init _init;
	
    __fastcall SWDispRTFChap(TWinControl *Owner);
    __fastcall ~ SWDispRTFChap();

	virtual char Display(SWModule & imodule);
};

//---------------------------------------------------------------------------
#endif
