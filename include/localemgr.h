/******************************************************************************
 *  localemgr.h   - definition of class LocaleMgr used to interact with
 *				registered locales for a sword installation
 *
 * $Id: localemgr.h,v 1.3 2000/03/13 09:36:02 scribe Exp $
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
#include <list>
#include <swconfig.h>
#include <swlocale.h>

// using namespace std;

typedef map <string, SWLocale *, less<string> > LocaleMap;

class LocaleMgr {
private:
	void deleteLocales();
	char *defaultLocaleName;

protected:
	LocaleMap locales;
	virtual void loadConfigDir(const char *ipath);

public:
	
	LocaleMgr(const char *iConfigPath = 0);
	virtual ~LocaleMgr();
	virtual SWLocale *getLocale(const char *name);
	virtual list<string> getAvailableLocales();
	virtual const char *translate(const char *name, const char *text);
	virtual const char *LocaleMgr::getDefaultLocaleName();
	virtual void setDefaultLocaleName(const char *name);

	static LocaleMgr systemLocaleMgr;
};
#endif
