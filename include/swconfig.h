/******************************************************************************
 *  swconfig.h   - definition of Class SWConfig used for saving and retrieval
 *				of configuration information
 *
 * $Id: swconfig.h,v 1.17 2002/07/28 01:48:38 scribe Exp $
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

#ifndef SWCONFIG_H
#define SWCONFIG_H

#include <stdio.h>

#include <string>
#include <map>

#include <defs.h>
#include <multimapwdef.h>

using namespace std;



typedef multimapwithdefault < string, string, less < string > >ConfigEntMap;
typedef map < string, ConfigEntMap, less < string > >SectionMap;

/** The class to read and save settings using a file on disk.
*
*/
class SWDLLEXPORT SWConfig {
private:
	char getline(FILE * fp, string & line);
public:
	/** The filename used by this SWConfig object
	*
	*/
	string filename;
	/** Map of available sections
	* The map of available sections.
	*/
	SectionMap Sections;

	/** Constructor of SWConfig
	* @param ifilename The file, which should be used for this config.
	*/
	SWConfig(const char *ifilename);
	virtual ~SWConfig();

	/** Load from disk
	* Load the contzent from disk.
	*/
	virtual void Load();

	/** Save to disk
	* Save the content of this config object to disk.
	*/
	virtual void Save();

	/** Merges the values of addFrom
	* @param The config which values should be merged to this config object. Already existing values will be overwritten.
	*/
	virtual void augment(SWConfig &addFrom);
	virtual SWConfig & operator +=(SWConfig &addFrom) { augment(addFrom); return *this; }

	/** Get a section
	* This is an easy way to get and store config values.
	* The following will work:\n
	*
	* @code
	* SWConfig config("/home/user/.setttings");
	* config["Colors"]["Background"] = "red";
	* @endcode
	*/
	virtual ConfigEntMap & operator [](const char *section);
	};

#endif
