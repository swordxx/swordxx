/******************************************************************************
 *  swmgr.h   - definition of class SWMgr used to interact with an install
 *				base of sword modules.
 *
 * $Id: swmgr.h,v 1.6 1999/09/05 21:42:50 scribe Exp $
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

#ifndef SWMGR_H
#define SWMGR_H

#include <string>
#include <map>
#include <swmodule.h>
#include <swconfig.h>
#include <swlog.h>

// using namespace std;

typedef map <string, SWModule *, less<string> > ModMap;
typedef list<string> OptionsList;
typedef map<string, SWFilter *> FilterMap;

class SWMgr {
private:
	SWConfig *myconfig;
	SWConfig *mysysconfig;

	void SWMgr::CreateMods();
	SWModule *SWMgr::CreateMod(string name, string driver, ConfigEntMap &section);
	void SWMgr::DeleteMods();
	FilterMap optionFilters;
	char configType;	// 0 = file; 1 = directory

protected:
	SWFilter *gbfplain;
	FilterList cleanupFilters;
	OptionsList options;
	virtual char AddModToConfig(int conffd, const char *fname);
	virtual void findConfig();
	virtual void loadConfigDir(const char *ipath);
	virtual void AddGlobalOptions(SWModule *module, ConfigEntMap &section, ConfigEntMap::iterator start, ConfigEntMap::iterator end);
	virtual void AddLocalOptions(SWModule *module, ConfigEntMap &section, ConfigEntMap::iterator start, ConfigEntMap::iterator end);
	virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
	virtual void AddStripFilters(SWModule *module, ConfigEntMap &section);
	virtual void AddRawFilters(SWModule *module, ConfigEntMap &section);
	virtual char existsFile(const char *ipath, const char *ifileName);
	virtual char existsDir(const char *ipath, const char *idirName);

public:
	
	SWConfig *config;
	SWConfig *sysconfig;
	ModMap Modules;
	char *prefixPath;
	char *configPath;

	SWMgr(SWConfig *iconfig = 0, SWConfig *isysconfig = 0, bool autoload = true);
	SWMgr(const char *iConfigPath, bool autoload = true);
	void init();
	virtual ~SWMgr();
	virtual void InstallScan(const char *dir);
	virtual void Load();
	virtual void setGlobalOption(const char *option, const char *value);
	virtual const char *getGlobalOption(const char *option);
	virtual const char *getGlobalOptionTip(const char *option);
	virtual OptionsList getGlobalOptions();
	virtual OptionsList getGlobalOptionValues(const char *option);
};
#endif
