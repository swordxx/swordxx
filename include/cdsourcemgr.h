/******************************************************************************
 *  cdsourcemgr.h	- definition of class SWCacheMgr used to provide an interface for
 *  	caching information of module sources
 *
 * $Id: cdsourcemgr.h,v 1.1 2002/10/23 23:13:35 dglassey Exp $
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
 
#ifndef CDSOURCEMGR_H
#define CDSOURCEMGR_H
 
 #include <defs.h>
 #include "swsourcemgr.h"

SWORD_NAMESPACE_START

class SWDLLEXPORT CDSourceMgr: public SWSourceMgr {
public:	
	CDSourceMgr(const char *iConfigPath);
	char Refresh();
}

CDSourceMgr *DetectCD(const char *iCDPath);

SWORD_NAMESPACE_END
#endif
