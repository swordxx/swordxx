/******************************************************************************
 *  localemgr.cpp - implementation of class LocaleMgr used to interact with
 *				registered locales for a sword installation
 *
 * $Id$
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
#include <dirent.h>

#include <swmgr.h>
#include <utilfuns.h>

#include <stringmgr.h>
#include <filemgr.h>
#include <localemgr.h>

SWORD_NAMESPACE_START


LocaleMgr *LocaleMgr::systemLocaleMgr = 0;

class __staticsystemLocaleMgr {
public:
	__staticsystemLocaleMgr() { }
	~__staticsystemLocaleMgr() { delete LocaleMgr::systemLocaleMgr; }
} _staticsystemLocaleMgr;


LocaleMgr *LocaleMgr::getSystemLocaleMgr() {
	if (!systemLocaleMgr)
		systemLocaleMgr = new LocaleMgr();

	return systemLocaleMgr;
}


void LocaleMgr::setSystemLocaleMgr(LocaleMgr *newLocaleMgr) {
	if (systemLocaleMgr)
		delete systemLocaleMgr;
	systemLocaleMgr = newLocaleMgr;
}


LocaleMgr::LocaleMgr(const char *iConfigPath) {
	locales = new LocaleMap();
	char *prefixPath = 0;
	char *configPath = 0;
	char configType = 0;
	SWBuf path;
	std::list<SWBuf> augPaths;
	
	defaultLocaleName = 0;
	
	if (!iConfigPath)
		SWMgr::findConfig(&configType, &prefixPath, &configPath, &augPaths);
	else configPath = (char *)iConfigPath;
	
	if (prefixPath) {
		switch (configType) {
		case 2:
			int i;
			for (i = strlen(configPath)-1; ((i) && (configPath[i] != '/') && (configPath[i] != '\\')); i--);
			configPath[i] = 0;
			path = configPath;
			path += "/";
			break;
		default:
			path = prefixPath;
			if ((prefixPath[strlen(prefixPath)-1] != '\\') && (prefixPath[strlen(prefixPath)-1] != '/'))
				path += "/";

			break;
		}
		if (FileMgr::existsDir(path.c_str(), "locales.d")) {
			path += "locales.d";
			loadConfigDir(path.c_str());
		}
	}
	
	if (augPaths.size()) { //load locale files from all augmented paths
		std::list<SWBuf>::iterator it = augPaths.begin();
		std::list<SWBuf>::iterator end = augPaths.end();
		
		for (;it != end; ++it) {
			if (FileMgr::existsDir((*it).c_str(), "locales.d")) {
				SWBuf path = (*it) + "locales.d";
				loadConfigDir(path.c_str());
			}
		}
	}

	// Locales will be invalidated if you change the StringMgr
	// So only use the default hardcoded locale and let the
	// frontends change the locale if they want
	stdstr(&defaultLocaleName, "en_US");

	if (prefixPath)
		delete [] prefixPath;

	if (configPath)
		delete [] configPath;
}


LocaleMgr::~LocaleMgr() {
	if (defaultLocaleName)
		delete [] defaultLocaleName;
     	deleteLocales();
	delete locales;
}


void LocaleMgr::loadConfigDir(const char *ipath) {
	DIR *dir;
	struct dirent *ent;
	SWBuf newmodfile;
	LocaleMap::iterator it;
	SWLog::getSystemLog()->logInformation("LocaleMgr::loadConfigDir loading %s", ipath);
 
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
					bool supported = false;
					if (StringMgr::hasUTF8Support()) {
						supported = (locale->getEncoding() && (!strcmp(locale->getEncoding(), "UTF-8") || !strcmp(locale->getEncoding(), "ASCII")) );
					}
					else {
						supported = !locale->getEncoding() || (locale->getEncoding() && (strcmp(locale->getEncoding(), "UTF-8") != 0)); //exclude UTF-8 locales
					}
					
					if (!supported) { //not supported
						delete locale;
						continue;
					}
				
					it = locales->find(locale->getName());
					if (it != locales->end()) { // already present
						*((*it).second) += *locale;
						delete locale;
					}
					else locales->insert(LocaleMap::value_type(locale->getName(), locale));
				}
				else	delete locale;
			}
		}
		closedir(dir);
	}
}


void LocaleMgr::deleteLocales() {

	LocaleMap::iterator it;

	for (it = locales->begin(); it != locales->end(); it++)
		delete (*it).second;

	locales->erase(locales->begin(), locales->end());
}


SWLocale *LocaleMgr::getLocale(const char *name) {
	LocaleMap::iterator it;

	it = locales->find(name);
	if (it != locales->end())
		return (*it).second;

	SWLog::getSystemLog()->logWarning("LocaleMgr::getLocale failed to find %s\n", name);
	return 0;
}


std::list <SWBuf> LocaleMgr::getAvailableLocales() {
	std::list <SWBuf> retVal;
	for (LocaleMap::iterator it = locales->begin(); it != locales->end(); it++) 
		retVal.push_back((*it).second->getName());

	return retVal;
}


const char *LocaleMgr::translate(const char *text, const char *localeName) {
	SWLocale *target;
	if (!localeName) {
		localeName = getDefaultLocaleName();
	}
	target = getLocale(localeName);
	if (target)
		return target->translate(text);
	return text;
}


const char *LocaleMgr::getDefaultLocaleName() {
	return defaultLocaleName;
}


void LocaleMgr::setDefaultLocaleName(const char *name) {
	char *tmplang=0;
	stdstr(&tmplang, name);
	// discard everything after '.' usually encoding e.g. .UTF-8
	strtok(tmplang, ".");
	// also discard after '@' so e.g. @euro locales are found
	strtok(tmplang, "@");
	// Before we set it we want to verify that it is an existing locale
	// First check for what we ask for
	if (getLocale(tmplang)) {
		stdstr(&defaultLocaleName, tmplang);
	}
	else {
		// check for locale without country
		char *nocntry=0;
		stdstr(&nocntry, tmplang);
		strtok(nocntry, "_");
		if (getLocale(nocntry)) {
			stdstr(&defaultLocaleName, nocntry);
		}
		else {
			if (!defaultLocaleName) {
				// make sure we have a default locale name
				// even though the one asked for doesn't exist
				stdstr(&defaultLocaleName, "en_US");
			}

		}
		delete [] nocntry;
	}
	delete [] tmplang;
}

SWORD_NAMESPACE_END
