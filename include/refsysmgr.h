/******************************************************************************
 *  refsysmgr.h   - definition of class RefSysMgr used to interact with
 *				registered biblical reference systems for a sword installation
 *
 * $Id$
 *
 * Copyright 2004 CrossWire Bible Society (http://www.crosswire.org)
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

#ifndef REFSYSMGR_H
#define REFSYSMGR_H


#include <map>
#include <list>
//#include <swconfig.h>

#include <defs.h>

SWORD_NAMESPACE_START

class RefSys;
class VerseKey2;
#if 0
struct RefDetails 
{
	char *name;
	int cpssize;
};
#endif
typedef std::pair <RefSys *, int> RefDetails;
typedef std::list<SWBuf> StringList;
typedef std::map < SWBuf, RefDetails, std::less < SWBuf > > RefSysMap;
/**
* The RefSysMgr class handles all the different locales of Sword.
* It provides functions to get a list of all available locales,
* to get the default locale name and to get it.
* The other functions are not interesting for frontend programmers.
*
* To get the default locale name use @see getDefaultLocaleName
* To set the default locale name use @see setDefaultLocaleName
* To get the locale for a language name use @see getLocale
* To get a list of availble locales use @see getAvailableLocales
*/


class SWDLLEXPORT RefSysMgr {
private:
	void deleteRefSys();
	char *defaultRefSysName;
	RefSysMgr(const RefSysMgr &);
	friend class __staticsystemRefSysMgr;
	RefSys *loadRefSys(const char *name, int cpssize);
	SWBuf refpath;
protected:
	RefSysMap *m_reflist;
	static RefSysMgr *systemRefSysMgr;

public:

	/** Default constructor of  RefSysMgr
	* You do normally not need this constructor, use RefSysMgr::getSystemRefSysMgr() instead.
	*/
	RefSysMgr(const char *iConfigPath = 0);

	/**
	* Default destructor of RefSysMgr
	*/
	virtual ~RefSysMgr();

	/** Get the locale connected with the name "name".
	*
	* @param name The name of the locale you want to have. For example use getLocale("de") to get the locale for the German language.
	* @return Returns the locale object if the locale with the name given as parameter was found. If it wasn't found return NULL.
	*/
	virtual RefSys *getRefSys(const char *name);
	virtual VerseKey2 *getVerseKey(const char *name, const char *key);

	/** Get the list of available locales.
	*
	* @return Returns a list of strings, which contains the names of the available locales.
	*/
	virtual StringList getAvailableRefSys();
	/** Get the default locale name. To set it use @see setDefaultLocaleName
	*
	* @return Returns the default locale name
	*/
	virtual const char *getDefaultRefSysName();

	/** Set the new standard locale of Sword.
	*
	* @param name The name of the new default locale  
	*/
	virtual void setDefaultRefSysName(const char *name);

	/** The RefSysMgr object used globally in the Sword world.
	* Do not create your own RefSysMgr, use this static object instead.
	*/
	static RefSysMgr *getSystemRefSysMgr();
	static void setSystemRefSysMgr(RefSysMgr *newRefSysMgr);

	/** Augment this localmgr with all locale.conf files in a directory
	*/
	virtual void loadConfigDir(const char *ipath);

};

SWORD_NAMESPACE_END

#endif
