/******************************************************************************
 *  swcacher=mgr.h	- definition of class SWCacheMgr used to provide an interface for
 *  	caching information of module sources
 *
 * $Id: swcachemgr.h,v 1.1 2002/10/23 22:11:32 dglassey Exp $
 *
 * Copyright 2002 CrossWire Bible Society (http://www.crosswire.org)
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
 
#ifndef SWCACHEMGR_H
#define SWCACHEMGR_H
 
 #include <defs.h>
 #include "swmgr.h"

SWORD_NAMESPACE_START

class SWDLLEXPORT SWCacheMgr: public SWMgr {
public:	
	SWCacheMgr(const char *iConfigPath, bool autoload = false);
	//SWMgr(const char *iConfigPath, bool autoload = true, SWFilterMgr *filterMgr = 0);
	virtual char Refresh();
}

SWORD_NAMESPACE_END
#endif
