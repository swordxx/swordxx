/******************************************************************************
 *  tbdisp.cpp  - code for class 'tbdisp'.  tbdisp writes module output to a
 *			MSWindows TextBox (or any other control that takes a
 *			SetDialogItemText
 *
 * $Id: tbdisp.h,v 1.1 1999/05/04 22:03:36 scribe Exp $
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

#ifndef TBDISP_H
#define TBDISP_H

#include <windows.h>
class SWModule;

class TBDisp: public SWDisplay {
protected:
	HWND	wnd;
	int	ctrlid;
public:
	TBDisp(HWND iwnd, int ictrlid);
	virtual char Display(SWModule &imodule);
};
#endif
