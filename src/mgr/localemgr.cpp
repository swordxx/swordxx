/******************************************************************************
 *  localemgr.cpp - implementation of class LocaleMgr used to interact with
 *				registered locales for a sword installation
 *
 * $Id: localemgr.cpp,v 1.5 2000/03/15 07:25:09 scribe Exp $
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

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#include <unixstr.h>
#endif
#include <sys/stat.h>

#include <swmgr.h>
#include <utilfuns.h>

#include <localemgr.h>


LocaleMgr LocaleMgr::systemLocaleMgr;


LocaleMgr::LocaleMgr(const char *iConfigPath) {
	char *prefixPath = 0;
	char *configPath = 0;
	char configType = 0;
	string path;
	
	defaultLocaleName = 0;
	
	char *lang = getenv ("LANG");
	if (lang) {
		if (strlen(lang) > 0)
			setDefaultLocaleName(lang);
		else setDefaultLocaleName("en");
	}
	else setDefaultLocaleName("en");

	if (!iConfigPath)
		SWMgr::findConfig(&configType, &configPath, &prefixPath);
	else configPath = (char *)iConfigPath;

	if (configPath) {
		path = configPath;
		if ((configPath[strlen(configPath)-1] != '\\') && (configPath[strlen(configPath)-1] != '/'))
			path += "/";

		if (SWMgr::existsDir(path.c_str(), "locales.d")) {
			path += "locales.d";
			loadConfigDir(path.c_str());
		}
	}
}


LocaleMgr::~LocaleMgr() {
	if (defaultLocaleName)
		delete [] defaultLocaleName;
}


void LocaleMgr::loadConfigDir(const char *ipath) {
	DIR *dir;
	struct dirent *ent;
	string newmodfile;
	LocaleMap::iterator it;
 
	if ((dir = opendir(ipath))) {
		rewinddir(dir);
		while ((ent = readdir(dir))) {
			if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
				newmodfile = ipath;
				if ((ipath[strlen(ipath)-1] != '\\') && (ipath[strlen(ipath)-1] != '/'))
					newmodfile += "/";
				newmodfile += ent->d_name;
				SWLocale *locale = new SWLocale(newmodfile.c_str());
				if (locale->getName()) {
					it = locales.find(locale->getName());
					if (it != locales.end()) {
						*((*it).second) += *locale;
						delete locale;
					}
					else locales.insert(LocaleMap::value_type(locale->getName(), locale));
				}
			}
		}
		closedir(dir);
	}
}


void LocaleMgr::deleteLocales() {

	LocaleMap::iterator it;

	for (it = locales.begin(); it != locales.end(); it++)
		delete (*it).second;

	locales.erase(locales.begin(), locales.end());
}


SWLocale *LocaleMgr::getLocale(const char *name) {
	LocaleMap::iterator it;

	it = locales.find(name);
	if (it != locales.end())
		return (*it).second;

	return 0;
}


list <string> LocaleMgr::getAvailableLocales() {
	list <string> retVal;
	for (LocaleMap::iterator it = locales.begin(); it != locales.end(); it++) 
		retVal.push_back((*it).second->getName());

	return retVal;
}


const char *LocaleMgr::translate(const char *name, const char *text) {
	SWLocale *target;
	target = getLocale(name);
	if (target)
		return target->translate(text);
	return text;
}


const char *LocaleMgr::getDefaultLocaleName() {
	return defaultLocaleName;
}


void LocaleMgr::setDefaultLocaleName(const char *name) {
	stdstr(&defaultLocaleName, name);
}
