/******************************************************************************
 *  swdisp.cpp  - code for base class 'swdisp'.  swdisp is the basis for all
 *		  types of displays (e.g. raw textout, curses, xwindow, etc.)
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
