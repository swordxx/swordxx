/******************************************************************************
 *  swmgr.cpp   - implementaion of class SWMgr used to interact with an install
 *				base of sword modules.
 *
 * $Id: swmgr.cpp,v 1.90 2003/06/17 22:14:18 joachim Exp $
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
#ifndef _MSC_VER
#include <iostream>
#endif
#include <dirent.h>

#include <swmgr.h>
#include <rawtext.h>
#include <rawgenbook.h>
#include <rawcom.h>
#include <hrefcom.h>
#include <rawld.h>
#include <rawld4.h>
#include <utilfuns.h>
#include <gbfplain.h>
#include <thmlplain.h>
#include <osisplain.h>
#include <gbfstrongs.h>
#include <gbffootnotes.h>
#include <gbfheadings.h>
#include <gbfredletterwords.h>
#include <gbfmorph.h>
#include <osisheadings.h>
#include <osisfootnotes.h>
#include <osisstrongs.h>
#include <osismorph.h>
#include <thmlstrongs.h>
#include <thmlfootnotes.h>
#include <thmlheadings.h>
#include <thmlmorph.h>
#include <thmlvariants.h>
#include <thmllemma.h>
#include <thmlscripref.h>
#include <cipherfil.h>
#include <rawfiles.h>
#include <ztext.h>
#include <zld.h>
#include <zcom.h>
#include <lzsscomprs.h>
#include <utf8greekaccents.h>
#include <utf8cantillation.h>
#include <utf8hebrewpoints.h>
#include <greeklexattribs.h>
#include <swfiltermgr.h>
#ifndef EXCLUDEZLIB
#include "zipcomprs.h"
#endif


#ifdef _ICU_
#include <utf8transliterator.h>
#endif

SWORD_NAMESPACE_START

#ifdef _ICU_
bool SWMgr::isICU = true;
#else
bool SWMgr::isICU = false;
#endif


bool SWMgr::debug = false;

#ifdef GLOBCONFPATH
const char *SWMgr::globalConfPath = GLOBCONFPATH;
#else
const char *SWMgr::globalConfPath = "/etc/sword.conf:/usr/local/etc/sword.conf";
#endif

void SWMgr::init() {
	SWFilter *tmpFilter = 0;
	configPath  = 0;
	prefixPath  = 0;
	configType  = 0;
	myconfig    = 0;
	mysysconfig = 0;
	homeConfig = 0;


	cipherFilters.clear();
	optionFilters.clear();
	cleanupFilters.clear();

	tmpFilter = new ThMLVariants();
	optionFilters.insert(FilterMap::value_type("ThMLVariants", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFStrongs();
	optionFilters.insert(FilterMap::value_type("GBFStrongs", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFFootnotes();
	optionFilters.insert(FilterMap::value_type("GBFFootnotes", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFRedLetterWords();
	optionFilters.insert(FilterMap::value_type("GBFRedLetterWords", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFMorph();
	optionFilters.insert(FilterMap::value_type("GBFMorph", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFHeadings();
	optionFilters.insert(FilterMap::value_type("GBFHeadings", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new OSISHeadings();
	optionFilters.insert(FilterMap::value_type("OSISHeadings", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new OSISStrongs();
	optionFilters.insert(FilterMap::value_type("OSISStrongs", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new OSISMorph();
	optionFilters.insert(FilterMap::value_type("OSISMorph", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new OSISFootnotes();
	optionFilters.insert(FilterMap::value_type("OSISFootnotes", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new ThMLStrongs();
	optionFilters.insert(FilterMap::value_type("ThMLStrongs", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new ThMLFootnotes();
	optionFilters.insert(FilterMap::value_type("ThMLFootnotes", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new ThMLMorph();
	optionFilters.insert(FilterMap::value_type("ThMLMorph", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new ThMLHeadings();
	optionFilters.insert(FilterMap::value_type("ThMLHeadings", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new ThMLLemma();
	optionFilters.insert(FilterMap::value_type("ThMLLemma", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new ThMLScripref();
	optionFilters.insert(FilterMap::value_type("ThMLScripref", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new UTF8GreekAccents();
	optionFilters.insert(FilterMap::value_type("UTF8GreekAccents", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new UTF8HebrewPoints();
	optionFilters.insert(FilterMap::value_type("UTF8HebrewPoints", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new UTF8Cantillation();
	optionFilters.insert(FilterMap::value_type("UTF8Cantillation", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GreekLexAttribs();
	optionFilters.insert(FilterMap::value_type("GreekLexAttribs", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

// UTF8Transliterator needs to be handled differently because it should always available as an option, for all modules
#ifdef _ICU_
	transliterator = new UTF8Transliterator();
	optionFilters.insert(FilterMap::value_type("UTF8Transliterator", transliterator));
	options.push_back(transliterator->getOptionName());
	cleanupFilters.push_back(transliterator);
#endif

	gbfplain = new GBFPlain();
	cleanupFilters.push_back(gbfplain);

	thmlplain = new ThMLPlain();
	cleanupFilters.push_back(thmlplain);

	osisplain = new OSISPlain();
	cleanupFilters.push_back(osisplain);
}


SWMgr::SWMgr(SWFilterMgr *filterMgr) {
	commonInit(0, 0, true, filterMgr);
}


SWMgr::SWMgr(SWConfig *iconfig, SWConfig *isysconfig, bool autoload, SWFilterMgr *filterMgr) {
	commonInit(iconfig, isysconfig, autoload, filterMgr);
}


void SWMgr::commonInit(SWConfig * iconfig, SWConfig * isysconfig, bool autoload, SWFilterMgr *filterMgr) {
	this->filterMgr = filterMgr;
	if (filterMgr)
		filterMgr->setParentMgr(this);

	init();
	
	if (iconfig) {
		config   = iconfig;
		myconfig = 0;
	}
	else config = 0;
	if (isysconfig) {
		sysconfig   = isysconfig;
		mysysconfig = 0;
	}
	else sysconfig = 0;

	if (autoload)
		Load();
}


SWMgr::SWMgr(const char *iConfigPath, bool autoload, SWFilterMgr *filterMgr) {

	std::string path;
	
	this->filterMgr = filterMgr;
	if (filterMgr)
		filterMgr->setParentMgr(this);

	init();
	
	path = iConfigPath;
	if ((iConfigPath[strlen(iConfigPath)-1] != '\\') && (iConfigPath[strlen(iConfigPath)-1] != '/'))
		path += "/";
	if (FileMgr::existsFile(path.c_str(), "mods.conf")) {
		stdstr(&prefixPath, path.c_str());
		path += "mods.conf";
		stdstr(&configPath, path.c_str());
	}
	else {
		if (FileMgr::existsDir(path.c_str(), "mods.d")) {
			stdstr(&prefixPath, path.c_str());
			path += "mods.d";
			stdstr(&configPath, path.c_str());
			configType = 1;
		}
	}

	config = 0;
	sysconfig = 0;

	if (autoload && configPath)
		Load();
}


SWMgr::~SWMgr() {

	DeleteMods();

	for (FilterList::iterator it = cleanupFilters.begin(); it != cleanupFilters.end(); it++)
		delete (*it);
			
	if (homeConfig)
		delete homeConfig;

	if (myconfig)
		delete myconfig;

	if (prefixPath)
		delete [] prefixPath;

	if (configPath)
		delete [] configPath;

	if (filterMgr)
		delete filterMgr;
}


void SWMgr::findConfig(char *configType, char **prefixPath, char **configPath, std::list<std::string> *augPaths) {
	std::string path;
	ConfigEntMap::iterator entry;
	ConfigEntMap::iterator lastEntry;

	char *envsworddir = getenv ("SWORD_PATH");
	char *envhomedir  = getenv ("HOME");

	*configType = 0;

#ifndef _MSC_VER
	// check working directory
if (debug)
	std::cerr << "Checking working directory for mods.conf...";
#endif

	if (FileMgr::existsFile(".", "mods.conf")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

		stdstr(prefixPath, "./");
		stdstr(configPath, "./mods.conf");
		return;
	}

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking working directory for mods.d...";
#endif

	if (FileMgr::existsDir(".", "mods.d")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

		stdstr(prefixPath, "./");
		stdstr(configPath, "./mods.d");
		*configType = 1;
		return;
	}


	// check environment variable SWORD_PATH
#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking SWORD_PATH...";
#endif

	if (envsworddir != NULL) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found (" << envsworddir << ")\n";
#endif

		path = envsworddir;
		if ((envsworddir[strlen(envsworddir)-1] != '\\') && (envsworddir[strlen(envsworddir)-1] != '/'))
			path += "/";

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking $SWORD_PATH for mods.conf...";
#endif

		if (FileMgr::existsFile(path.c_str(), "mods.conf")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

			stdstr(prefixPath, path.c_str());
			path += "mods.conf";
			stdstr(configPath, path.c_str());
			return;
		}

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking $SWORD_PATH for mods.d...";
#endif

		if (FileMgr::existsDir(path.c_str(), "mods.d")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

			stdstr(prefixPath, path.c_str());
			path += "mods.d";
			stdstr(configPath, path.c_str());
			*configType = 1;
			return;
		}
	}


	// check for systemwide globalConfPath

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nParsing " << globalConfPath << "...";
#endif

	char *globPaths = 0;
	char *gfp;
	stdstr(&globPaths, globalConfPath);
	for (gfp = strtok(globPaths, ":"); gfp; gfp = strtok(0, ":")) {

	#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking for " << gfp << "...";
#endif

		if (FileMgr::existsFile(gfp))
			break;
	}

	if (gfp) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

		SWConfig etcconf(gfp);
		if ((entry = etcconf.Sections["Install"].find("DataPath")) != etcconf.Sections["Install"].end()) {
			path = (*entry).second;
			if (((*entry).second.c_str()[strlen((*entry).second.c_str())-1] != '\\') && ((*entry).second.c_str()[strlen((*entry).second.c_str())-1] != '/'))
				path += "/";

#ifndef _MSC_VER
if (debug)
	std::cerr << "DataPath in " << gfp << " is set to: " << path;
#endif

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking for mods.conf in DataPath ";
#endif
			if (FileMgr::existsFile(path.c_str(), "mods.conf")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

				stdstr(prefixPath, path.c_str());
				path += "mods.conf";
				stdstr(configPath, path.c_str());
				*configType = 1;
			}

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking for mods.d in DataPath ";
#endif

			if (FileMgr::existsDir(path.c_str(), "mods.d")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

				stdstr(prefixPath, path.c_str());
				path += "mods.d";
				stdstr(configPath, path.c_str());
				*configType = 1;
			}
		}
		if (augPaths) {
			augPaths->clear();
			entry     = etcconf.Sections["Install"].lower_bound("AugmentPath");
			lastEntry = etcconf.Sections["Install"].upper_bound("AugmentPath");
			for (;entry != lastEntry; entry++) {
				path = entry->second;
				if ((entry->second.c_str()[strlen(entry->second.c_str())-1] != '\\') && (entry->second.c_str()[strlen(entry->second.c_str())-1] != '/'))
					path += "/";
				augPaths->push_back(path);
			}
		}
	}

	delete [] globPaths;
	if (*configType)
		return;

	// check ~/.sword/

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking home directory for ~/.sword/mods.conf" << path;
#endif

	if (envhomedir != NULL) {
		path = envhomedir;
		if ((envhomedir[strlen(envhomedir)-1] != '\\') && (envhomedir[strlen(envhomedir)-1] != '/'))
			path += "/";
		path += ".sword/";
		if (FileMgr::existsFile(path.c_str(), "mods.conf")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << " found\n";
#endif

			stdstr(prefixPath, path.c_str());
			path += "mods.conf";
			stdstr(configPath, path.c_str());
			return;
		}

#ifndef _MSC_VER
if (debug)
	std::cerr << "\nChecking home directory for ~/.sword/mods.d" << path;
#endif

		if (FileMgr::existsDir(path.c_str(), "mods.d")) {

#ifndef _MSC_VER
if (debug)
	std::cerr << "found\n";
#endif

			stdstr(prefixPath, path.c_str());
			path += "mods.d";
			stdstr(configPath, path.c_str());
			*configType = 2;
			return;
		}
	}
}


void SWMgr::loadConfigDir(const char *ipath)
{
   DIR *dir;
   struct dirent *ent;
   std::string newmodfile;
 
	if ((dir = opendir(ipath))) {
		rewinddir(dir);
		while ((ent = readdir(dir))) {
			if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
				newmodfile = ipath;
				if ((ipath[strlen(ipath)-1] != '\\') && (ipath[strlen(ipath)-1] != '/'))
					newmodfile += "/";
				newmodfile += ent->d_name;
				if (config) {
					SWConfig tmpConfig(newmodfile.c_str());
					*config += tmpConfig;
				}
				else	config = myconfig = new SWConfig(newmodfile.c_str());
			}
		}
		closedir(dir);
		if (!config) {	// if no .conf file exist yet, create a default
			newmodfile = ipath;
			if ((ipath[strlen(ipath)-1] != '\\') && (ipath[strlen(ipath)-1] != '/'))
				newmodfile += "/";
			newmodfile += "globals.conf";
			config = myconfig = new SWConfig(newmodfile.c_str());
		}
	}
}


void SWMgr::augmentModules(const char *ipath) {
	std::string path = ipath;
	if ((ipath[strlen(ipath)-1] != '\\') && (ipath[strlen(ipath)-1] != '/'))
		path += "/";
	if (FileMgr::existsDir(path.c_str(), "mods.d")) {
		char *savePrefixPath = 0;
		char *saveConfigPath = 0;
		SWConfig *saveConfig = 0;
		stdstr(&savePrefixPath, prefixPath);
		stdstr(&prefixPath, path.c_str());
		path += "mods.d";
		stdstr(&saveConfigPath, configPath);
		stdstr(&configPath, path.c_str());
		saveConfig = config;
		config = myconfig = 0;
		loadConfigDir(configPath);

		CreateMods();

		stdstr(&prefixPath, savePrefixPath);
		delete []savePrefixPath;
		stdstr(&configPath, saveConfigPath);
		delete []saveConfigPath;
		(*saveConfig) += *config;
		homeConfig = myconfig;
		config = myconfig = saveConfig;
	}
}


/***********************************************************************
 * SWMgr::Load - loads actual modules
 *
 * RET: status - 0 = ok; -1 no config found; 1 = no modules installed
 *
 */

signed char SWMgr::Load() {
	signed char ret = 0;

	if (!config) {	// If we weren't passed a config object at construction, find a config file
		if (!configPath)	// If we weren't passed a config path at construction...
			findConfig(&configType, &prefixPath, &configPath, &augPaths);
		if (configPath) {
			if (configType)
				loadConfigDir(configPath);
			else	config = myconfig = new SWConfig(configPath);
		}
	}

	if (config) {
		SectionMap::iterator Sectloop, Sectend;
		ConfigEntMap::iterator Entryloop, Entryend;

		DeleteMods();

		for (Sectloop = config->Sections.lower_bound("Globals"), Sectend = config->Sections.upper_bound("Globals"); Sectloop != Sectend; Sectloop++) {		// scan thru all 'Globals' sections
			for (Entryloop = (*Sectloop).second.lower_bound("AutoInstall"), Entryend = (*Sectloop).second.upper_bound("AutoInstall"); Entryloop != Entryend; Entryloop++)	// scan thru all AutoInstall entries
				InstallScan((*Entryloop).second.c_str());		// Scan AutoInstall entry directory for new modules and install
		}		
		if (configType) {	// force reload on config object because we may have installed new modules
			delete myconfig;
			config = myconfig = 0;
			loadConfigDir(configPath);
		}
		else	config->Load();

		CreateMods();

		for (std::list<std::string>::iterator pathIt = augPaths.begin(); pathIt != augPaths.end(); pathIt++) {
			augmentModules(pathIt->c_str());
		}
//	augment config with ~/.sword/mods.d if it exists ---------------------
		char *envhomedir  = getenv ("HOME");
		if (envhomedir != NULL && configType != 2) { // 2 = user only
			std::string path = envhomedir;
			if ((envhomedir[strlen(envhomedir)-1] != '\\') && (envhomedir[strlen(envhomedir)-1] != '/'))
				path += "/";
			path += ".sword/";
			augmentModules(path.c_str());
		}
// -------------------------------------------------------------------------
		if ( !Modules.size() ) // config exists, but no modules
			ret = 1;

	}
	else {
		SWLog::systemlog->LogError("SWMgr: Can't find 'mods.conf' or 'mods.d'.  Try setting:\n\tSWORD_PATH=<directory containing mods.conf>\n\tOr see the README file for a full description of setup options (%s)", (configPath) ? configPath : "<configPath is null>");
		ret = -1;
	}

	return ret;
}

SWModule *SWMgr::CreateMod(std::string name, std::string driver, ConfigEntMap &section)
{
	std::string description, datapath, misc1;
	ConfigEntMap::iterator entry;
	SWModule *newmod = 0;
	std::string lang, sourceformat, encoding;
        signed char direction, enc, markup;

	description  = ((entry = section.find("Description")) != section.end()) ? (*entry).second : (std::string)"";
	lang  = ((entry = section.find("Lang")) != section.end()) ? (*entry).second : (std::string)"en";
 	sourceformat = ((entry = section.find("SourceType"))  != section.end()) ? (*entry).second : (std::string)"";
 	encoding = ((entry = section.find("Encoding"))  != section.end()) ? (*entry).second : (std::string)"";
	datapath = prefixPath;
	if ((prefixPath[strlen(prefixPath)-1] != '\\') && (prefixPath[strlen(prefixPath)-1] != '/'))
		datapath += "/";
	misc1 += ((entry = section.find("DataPath")) != section.end()) ? (*entry).second : (std::string)"";
	char *buf = new char [ strlen(misc1.c_str()) + 1 ];
	char *buf2 = buf;
	strcpy(buf, misc1.c_str());
//	for (; ((*buf2) && ((*buf2 == '.') || (*buf2 == '/') || (*buf2 == '\\'))); buf2++);
	for (; ((*buf2) && ((*buf2 == '/') || (*buf2 == '\\'))); buf2++);
	if (!strncmp(buf2, "./", 2)) { //remove the leading ./ in the module data path to make it look better
		buf2 += 2;
	}
	if (*buf2)
		datapath += buf2;
	delete [] buf;

	section["AbsoluteDataPath"] = datapath;

        if (!stricmp(sourceformat.c_str(), "GBF"))
                markup = FMT_GBF;
        else if (!stricmp(sourceformat.c_str(), "ThML"))
                markup = FMT_THML;
        else if (!stricmp(sourceformat.c_str(), "OSIS"))
                markup = FMT_OSIS;
        else
                markup = FMT_GBF;

	if (!stricmp(encoding.c_str(), "SCSU"))
		enc = ENC_SCSU;
	else if (!stricmp(encoding.c_str(), "UTF-8")) {
		enc = ENC_UTF8;
	}
	else enc = ENC_LATIN1;

	if ((entry = section.find("Direction")) == section.end()) {
                direction = DIRECTION_LTR;
        }
        else if (!stricmp((*entry).second.c_str(), "rtol")) {
                direction = DIRECTION_RTL;
        }
        else if (!stricmp((*entry).second.c_str(), "bidi")) {
                direction = DIRECTION_BIDI;
        }
        else {
                direction = DIRECTION_LTR;
        }

	if ((!stricmp(driver.c_str(), "zText")) || (!stricmp(driver.c_str(), "zCom"))) {
		SWCompress *compress = 0;
		int blockType = CHAPTERBLOCKS;
		misc1 = ((entry = section.find("BlockType")) != section.end()) ? (*entry).second : (std::string)"CHAPTER";
		if (!stricmp(misc1.c_str(), "VERSE"))
			blockType = VERSEBLOCKS;
		else if (!stricmp(misc1.c_str(), "CHAPTER"))
			blockType = CHAPTERBLOCKS;
		else if (!stricmp(misc1.c_str(), "BOOK"))
			blockType = BOOKBLOCKS;

		misc1 = ((entry = section.find("CompressType")) != section.end()) ? (*entry).second : (std::string)"LZSS";
#ifndef EXCLUDEZLIB
		if (!stricmp(misc1.c_str(), "ZIP"))
			compress = new ZipCompress();
		else
#endif
		if (!stricmp(misc1.c_str(), "LZSS"))
			compress = new LZSSCompress();

		if (compress) {
			if (!stricmp(driver.c_str(), "zText"))
				newmod = new zText(datapath.c_str(), name.c_str(), description.c_str(), blockType, compress, 0, enc, direction, markup, lang.c_str());
			else	newmod = new zCom(datapath.c_str(), name.c_str(), description.c_str(), blockType, compress, 0, enc, direction, markup, lang.c_str());
		}
	}

	if (!stricmp(driver.c_str(), "RawText")) {
		newmod = new RawText(datapath.c_str(), name.c_str(), description.c_str(), 0, enc, direction, markup, lang.c_str());
	}

	// backward support old drivers
	if (!stricmp(driver.c_str(), "RawGBF")) {
		newmod = new RawText(datapath.c_str(), name.c_str(), description.c_str(), 0, enc, direction, markup, lang.c_str());
	}

	if (!stricmp(driver.c_str(), "RawCom")) {
		newmod = new RawCom(datapath.c_str(), name.c_str(), description.c_str(), 0, enc, direction, markup, lang.c_str());
	}

	if (!stricmp(driver.c_str(), "RawFiles")) {
		newmod = new RawFiles(datapath.c_str(), name.c_str(), description.c_str(), 0, enc, direction, markup, lang.c_str());
	}

	if (!stricmp(driver.c_str(), "HREFCom")) {
		misc1 = ((entry = section.find("Prefix")) != section.end()) ? (*entry).second : (std::string)"";
		newmod = new HREFCom(datapath.c_str(), misc1.c_str(), name.c_str(), description.c_str());
	}

        string::size_type pos;  //used for position of final / in AbsoluteDataPath, but also set to 1 for modules types that need to strip module name
	if (!stricmp(driver.c_str(), "RawLD")) {
		newmod = new RawLD(datapath.c_str(), name.c_str(), description.c_str(), 0, enc, direction, markup, lang.c_str());
                pos = 1;
        }

	if (!stricmp(driver.c_str(), "RawLD4")) {
		newmod = new RawLD4(datapath.c_str(), name.c_str(), description.c_str(), 0, enc, direction, markup, lang.c_str());
                pos = 1;
        }

	if (!stricmp(driver.c_str(), "zLD")) {
		SWCompress *compress = 0;
		int blockCount;
		misc1 = ((entry = section.find("BlockCount")) != section.end()) ? (*entry).second : (std::string)"200";
		blockCount = atoi(misc1.c_str());
		blockCount = (blockCount) ? blockCount : 200;

		misc1 = ((entry = section.find("CompressType")) != section.end()) ? (*entry).second : (std::string)"LZSS";
#ifndef EXCLUDEZLIB
		if (!stricmp(misc1.c_str(), "ZIP"))
			compress = new ZipCompress();
		else
#endif
		if (!stricmp(misc1.c_str(), "LZSS"))
			compress = new LZSSCompress();

		if (compress) {
			newmod = new zLD(datapath.c_str(), name.c_str(), description.c_str(), blockCount, compress, 0, enc, direction, markup, lang.c_str());
		}
                pos = 1;
	}

	if (!stricmp(driver.c_str(), "RawGenBook")) {
		newmod = new RawGenBook(datapath.c_str(), name.c_str(), description.c_str(), 0, enc, direction, markup, lang.c_str());
                pos = 1;
	}

        if (pos == 1) {
                pos = section["AbsoluteDataPath"].rfind("/");
                section["AbsoluteDataPath"].erase(pos, section["AbsoluteDataPath"].length() - pos);
        }

        // if a specific module type is set in the config, use this
        if ((entry = section.find("Type")) != section.end())
                newmod->Type(entry->second.c_str());

        newmod->setConfig(&section);
                return newmod;
}


void SWMgr::AddGlobalOptions(SWModule *module, ConfigEntMap &section, ConfigEntMap::iterator start, ConfigEntMap::iterator end) {
	for (;start != end; start++) {
		FilterMap::iterator it;
		it = optionFilters.find((*start).second);
		if (it != optionFilters.end()) {
			module->AddOptionFilter((*it).second);	// add filter to module and option as a valid option
			OptionsList::iterator loop;
			for (loop = options.begin(); loop != options.end(); loop++) {
				if (!strcmp((*loop).c_str(), (*it).second->getOptionName()))
					break;
			}
			if (loop == options.end())	// if we have not yet included the option
				options.push_back((*it).second->getOptionName());
		}
	}
	if (filterMgr)
		filterMgr->AddGlobalOptions(module, section, start, end);
#ifdef _ICU_
	   module->AddOptionFilter(transliterator);
#endif
}


void SWMgr::AddLocalOptions(SWModule *module, ConfigEntMap &section, ConfigEntMap::iterator start, ConfigEntMap::iterator end)
{
	for (;start != end; start++) {
		FilterMap::iterator it;
		it = optionFilters.find((*start).second);
		if (it != optionFilters.end()) {
			module->AddOptionFilter((*it).second);	// add filter to module
		}
	}

	if (filterMgr)
		filterMgr->AddLocalOptions(module, section, start, end);
}


void SWMgr::AddRawFilters(SWModule *module, ConfigEntMap &section) {
	std::string sourceformat, cipherKey;
	ConfigEntMap::iterator entry;

	cipherKey = ((entry = section.find("CipherKey")) != section.end()) ? (*entry).second : (std::string)"";
	if (!cipherKey.empty()) {
		SWFilter *cipherFilter = new CipherFilter(cipherKey.c_str());
		cipherFilters.insert(FilterMap::value_type(module->Name(), cipherFilter));
		cleanupFilters.push_back(cipherFilter);
		module->AddRawFilter(cipherFilter);
	}

	if (filterMgr)
		filterMgr->AddRawFilters(module, section);
}


void SWMgr::AddEncodingFilters(SWModule *module, ConfigEntMap &section) {

	if (filterMgr)
		filterMgr->AddEncodingFilters(module, section);
}


void SWMgr::AddRenderFilters(SWModule *module, ConfigEntMap &section) {
	std::string sourceformat;
	ConfigEntMap::iterator entry;

	sourceformat = ((entry = section.find("SourceType")) != section.end()) ? (*entry).second : (std::string)"";

	// Temporary: To support old module types
        // TODO: Remove at 1.6.0 release?
	if (sourceformat.empty()) {
		sourceformat = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (std::string)"";
		if (!stricmp(sourceformat.c_str(), "RawGBF"))
			sourceformat = "GBF";
		else sourceformat = "";
	}

// process module	- eg. follows
//	if (!stricmp(sourceformat.c_str(), "GBF")) {
//		module->AddRenderFilter(gbftortf);
//	}

	if (filterMgr)
		filterMgr->AddRenderFilters(module, section);

}


void SWMgr::AddStripFilters(SWModule *module, ConfigEntMap &section)
{
	std::string sourceformat;
	ConfigEntMap::iterator entry;

	sourceformat = ((entry = section.find("SourceType")) != section.end()) ? (*entry).second : (std::string)"";
	// Temporary: To support old module types
	if (sourceformat.empty()) {
		sourceformat = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (std::string)"";
		if (!stricmp(sourceformat.c_str(), "RawGBF"))
			sourceformat = "GBF";
		else sourceformat = "";
	}
	
	if (!stricmp(sourceformat.c_str(), "GBF")) {
		module->AddStripFilter(gbfplain);
	}
	else if (!stricmp(sourceformat.c_str(), "ThML")) {
		module->AddStripFilter(thmlplain);
	}
	else if (!stricmp(sourceformat.c_str(), "OSIS")) {
		module->AddStripFilter(osisplain);
	}

	if (filterMgr)
		filterMgr->AddStripFilters(module, section);

}


void SWMgr::CreateMods() {
	SectionMap::iterator it;
	ConfigEntMap::iterator start;
	ConfigEntMap::iterator end;
	ConfigEntMap::iterator entry;
	SWModule *newmod;
	std::string driver, misc1;
	for (it = config->Sections.begin(); it != config->Sections.end(); it++) {
		ConfigEntMap &section = (*it).second;
		newmod = 0;
		
		driver = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (std::string)"";
		if (!driver.empty()) {
			newmod = CreateMod((*it).first, driver, section);
			if (newmod) {
				start = (*it).second.lower_bound("GlobalOptionFilter");
				end   = (*it).second.upper_bound("GlobalOptionFilter");
				AddGlobalOptions(newmod, section, start, end);

				start = (*it).second.lower_bound("LocalOptionFilter");
				end   = (*it).second.upper_bound("LocalOptionFilter");
				AddLocalOptions(newmod, section, start, end);

				AddRawFilters(newmod, section);
				AddStripFilters(newmod, section);
				AddRenderFilters(newmod, section);
				AddEncodingFilters(newmod, section);
				
				Modules.insert(ModMap::value_type(newmod->Name(), newmod));
			}
		}
	}
}


void SWMgr::DeleteMods() {

	ModMap::iterator it;

	for (it = Modules.begin(); it != Modules.end(); it++)
		delete (*it).second;

	Modules.clear();
}


void SWMgr::InstallScan(const char *dirname)
{
   DIR *dir;
   struct dirent *ent;
   int conffd = 0;
   std::string newmodfile;
   std::string targetName;
 
	if (!access(dirname, 04)) {
          if ((dir = opendir(dirname))) {
               rewinddir(dir);
               while ((ent = readdir(dir))) {
                    if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
                         newmodfile = dirname;
                         if ((dirname[strlen(dirname)-1] != '\\') && (dirname[strlen(dirname)-1] != '/'))
                              newmodfile += "/";
                         newmodfile += ent->d_name;
                         if (configType) {
                              if (config > 0)
                                   close(conffd);
                              targetName = configPath;
                              if ((configPath[strlen(configPath)-1] != '\\') && (configPath[strlen(configPath)-1] != '/'))
                                   targetName += "/";
                              targetName += ent->d_name;
                              conffd = open(targetName.c_str(), O_WRONLY|O_CREAT, S_IREAD|S_IWRITE);
                         }
                         else {
                              if (conffd < 1) {
                                   conffd = open(config->filename.c_str(), O_WRONLY|O_APPEND);
                                   if (conffd > 0)
                                        lseek(conffd, 0L, SEEK_END);
                              }
                         }
                         AddModToConfig(conffd, newmodfile.c_str());
                         unlink(newmodfile.c_str());
                    }
               }
               if (conffd > 0)
                    close(conffd);
               closedir(dir);
          }
     }
}


char SWMgr::AddModToConfig(int conffd, const char *fname)
{
	int modfd;
	char ch;

	SWLog::systemlog->LogTimedInformation("Found new module [%s]. Installing...", fname);
	modfd = open(fname, O_RDONLY);
	ch = '\n';
	write(conffd, &ch, 1);
	while (read(modfd, &ch, 1) == 1)
		write(conffd, &ch, 1);
	ch = '\n';
	write(conffd, &ch, 1);
	close(modfd);
	return 0;
}


void SWMgr::setGlobalOption(const char *option, const char *value)
{
	for (FilterMap::iterator it = optionFilters.begin(); it != optionFilters.end(); it++) {
		if ((*it).second->getOptionName()) {
			if (!stricmp(option, (*it).second->getOptionName()))
				(*it).second->setOptionValue(value);
		}
	}
}


const char *SWMgr::getGlobalOption(const char *option)
{
	for (FilterMap::iterator it = optionFilters.begin(); it != optionFilters.end(); it++) {
		if ((*it).second->getOptionName()) {
			if (!stricmp(option, (*it).second->getOptionName()))
				return (*it).second->getOptionValue();
		}
	}
	return 0;
}


const char *SWMgr::getGlobalOptionTip(const char *option)
{
	for (FilterMap::iterator it = optionFilters.begin(); it != optionFilters.end(); it++) {
		if ((*it).second->getOptionName()) {
			if (!stricmp(option, (*it).second->getOptionName()))
				return (*it).second->getOptionTip();
		}
	}
	return 0;
}


OptionsList SWMgr::getGlobalOptions()
{
	return options;
}


OptionsList SWMgr::getGlobalOptionValues(const char *option)
{
	OptionsList options;
	for (FilterMap::iterator it = optionFilters.begin(); it != optionFilters.end(); it++) {
		if ((*it).second->getOptionName()) {
			if (!stricmp(option, (*it).second->getOptionName())) {
				options = (*it).second->getOptionValues();
				break;	// just find the first one.  All option filters with the same option name should expect the same values
			}
		}
	}
	return options;
}


signed char SWMgr::setCipherKey(const char *modName, const char *key) {
	FilterMap::iterator it;
	ModMap::iterator it2;

	// check for filter that already exists
	it = cipherFilters.find(modName);
	if (it != cipherFilters.end()) {
		((CipherFilter *)(*it).second)->getCipher()->setCipherKey(key);
		return 0;
	}
	// check if module exists
	else {
		it2 = Modules.find(modName);
		if (it2 != Modules.end()) {
			SWFilter *cipherFilter = new CipherFilter(key);
			cipherFilters.insert(FilterMap::value_type(modName, cipherFilter));
			cleanupFilters.push_back(cipherFilter);
			(*it2).second->AddRawFilter(cipherFilter);
			return 0;
		}
	}
	return -1;
}

SWORD_NAMESPACE_END
