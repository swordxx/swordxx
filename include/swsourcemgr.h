/******************************************************************************
 *  swsourcemgr.h	- definition of class SWSourceMgr used to provide an interface for
 *  	caching information of module sources
 *
 * $Id$
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
 
#ifndef SWSOURCEMGR_H
#define SWSOURCEMGR_H
 
 #include <defs.h>
 
SWORD_NAMESPACE_START

class SWConfig;

class SWDLLEXPORT SWSourceMgr {
private:
	/** The path to the source's directory
	*
	*/
	char *prefixPath;
	/**
	* The path to the source's config
	*/
	char *configPath;
	/**
	* The type of config 0 = file; 1 = directory
	*/
	char configType;
	/** The global config object.
	* This is the global config object. It contains all items of all modules,
	* so lookups of entries should use this config object.
	* If you want to save a cipher key or other things to the module config file,
	* DO NOT USE this object, because it would corrupt your config files after config->Save().
	*
	* If you want to write to the modules config file read the informtaion of @ref setCipherKey() for an example of this.
	*/
	SWConfig *config;
	/**
	* ?
	*/
	SWConfig * myconfig;

public:	
	SWSourceMgr(const char *iConfigPath);
	//SWMgr(const char *iConfigPath, bool autoload = true, SWFilterMgr *filterMgr = 0);
	virtual char Refresh() = 0;
	void loadConfigDir(const char *ipath);
};

SWORD_NAMESPACE_END
#endif
