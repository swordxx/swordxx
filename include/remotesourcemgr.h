/******************************************************************************
 *  remotesourcemgr.h	- definition of class SWCacheMgr used to provide an interface for
 *  	caching information of module sources
 *
 * $Id: remotesourcemgr.h,v 1.1 2002/10/23 23:13:35 dglassey Exp $
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
 
#ifndef REMOTESOURCEMGR_H
#define REMOTESOURCEMGR_H
 
 #include <defs.h>
 #include "swsourcemgr.h"

SWORD_NAMESPACE_START

class SWDLLEXPORT RemoteSourceMgr: public SWSourceMgr {
public:	
	RemoteSourceMgr(const char *iHost, const char *iConfigPath);
	char Refresh();
}

SWORD_NAMESPACE_END
#endif
