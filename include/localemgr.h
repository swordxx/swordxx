/******************************************************************************
 *  localemgr.h   - definition of class LocaleMgr used to interact with
 *				registered locales for a sword installation
 *
 * $Id: localemgr.h,v 1.1 2000/03/12 01:09:43 scribe Exp $
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

#ifndef LOCALEMGR_H
#define LOCALEMGR_H

#include <string>
#include <map>
#include <swmodule.h>
#include <swconfig.h>
#include <swlog.h>

// using namespace std;

typedef map <string, SWLocale *, less<string> > LocaleMap;

class LocaleMgr {
private:

protected:
	LocaleMap locales;
	virtual void findConfig();
	virtual void loadConfigDir(const char *ipath);

public:
	
	LocaleMgr(const char *iConfigPath = 0);
	virtual ~LocaleMgr();
	virtual SWLocale *getLocale(const char *symbol);
	virtual List<string> getAvailableLocales();
	virtual const char *translate(const char *symbol, const char *text);
};
#endif
