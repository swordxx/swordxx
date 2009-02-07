/******************************************************************************
 *  swdisp.cpp  - code for base class 'swdisp'.  swdisp is the basis for all
 *		  types of displays (e.g. raw textout, curses, xwindow, etc.)
 *
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
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


#ifndef	_WIN32_WCE
#include <iostream>
#endif
#include <swmodule.h>
#include <swdisp.h>

SWORD_NAMESPACE_START

static const char *classes[] = {"SWDisplay", "SWObject", 0};
SWClass SWDisplay::classdef(classes);

/******************************************************************************
 * SWDisplay::Display - casts a module to a character pointer and displays it to
 *			raw output (overriden for different display types and
 *			module types if necessary)
 *
 * ENT:	imodule - module to display
 *
 * RET:	error status
 */

char SWDisplay::Display(SWModule &imodule)
{
#ifndef	_WIN32_WCE
	std::cout << (const char *)imodule;
#endif
	return 0;
}

SWORD_NAMESPACE_END
