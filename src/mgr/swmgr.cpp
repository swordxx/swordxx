/******************************************************************************
 *  swmgr.cpp   - implementaion of class SWMgr used to interact with an install
 *				base of sword modules.
 *
 * $Id: swmgr.cpp,v 1.40 2001/06/12 06:27:24 scribe Exp $
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
#ifndef __VISUALC__
#include <iostream.h>
#endif
#include <swmgr.h>
#include <rawtext.h>
#include <rawgbf.h>
#include <rawcom.h>
#include <hrefcom.h>
#include <rawld.h>
#include <rawld4.h>
#include <utilfuns.h>
#include <gbfplain.h>
#include <thmlplain.h>
#include <gbfstrongs.h>
#include <gbffootnotes.h>
#include <gbfheadings.h>
#include <gbfmorph.h>
#include <cipherfil.h>
#include <rawfiles.h>
#include <ztext.h>
#include <zcom.h>
#include <lzsscomprs.h>

#ifndef EXCLUDEZLIB
#include <zipcomprs.h>
#endif

bool SWMgr::debug = false;

void SWMgr::init() {
	SWFilter *tmpFilter = 0;
	configPath  = 0;
	prefixPath  = 0;
	configType  = 0;
	myconfig    = 0;
	mysysconfig = 0;

	cipherFilters.clear();
	optionFilters.clear();
	cleanupFilters.clear();

	tmpFilter = new GBFStrongs();
	optionFilters.insert(FilterMap::value_type("GBFStrongs", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFFootnotes();
	optionFilters.insert(FilterMap::value_type("GBFFootnotes", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFMorph();
	optionFilters.insert(FilterMap::value_type("GBFMorph", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	tmpFilter = new GBFHeadings();
	optionFilters.insert(FilterMap::value_type("GBFHeadings", tmpFilter));
	cleanupFilters.push_back(tmpFilter);

	gbfplain = new GBFPlain();
	cleanupFilters.push_back(gbfplain);

	thmlplain = new ThMLPlain();
	cleanupFilters.push_back(thmlplain);
}


SWMgr::SWMgr(SWConfig *iconfig, SWConfig *isysconfig, bool autoload) {

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


SWMgr::SWMgr(const char *iConfigPath, bool autoload) {

	string path;
	
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
			
	if (myconfig)
		delete myconfig;

	if (prefixPath)
		delete [] prefixPath;

	if (configPath)
		delete [] configPath;
}


void SWMgr::findConfig(char *configType, char **prefixPath, char **configPath) {
	string path;
	ConfigEntMap::iterator entry;

	char *envsworddir = getenv ("SWORD_PATH");
	char *envhomedir  = getenv ("HOME");

	*configType = 0;

#ifndef __VISUALC__
	// check working directory
if (debug)
	cerr << "Checking working directory for mods.conf...";
#endif

	if (FileMgr::existsFile(".", "mods.conf")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

		stdstr(prefixPath, "./");
		stdstr(configPath, "./mods.conf");
		return;
	}

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking working directory for mods.d...";
#endif

	if (FileMgr::existsDir(".", "mods.d")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

		stdstr(prefixPath, "./");
		stdstr(configPath, "./mods.d");
		*configType = 1;
		return;
	}


	// check environment variable SWORD_PATH
#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking SWORD_PATH...";
#endif

	if (envsworddir != NULL) {

#ifndef __VISUALC__
if (debug)
	cerr << "found (" << envsworddir << ")\n";
#endif

		path = envsworddir;
		if ((envsworddir[strlen(envsworddir)-1] != '\\') && (envsworddir[strlen(envsworddir)-1] != '/'))
			path += "/";

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking $SWORD_PATH for mods.conf...";
#endif

		if (FileMgr::existsFile(path.c_str(), "mods.conf")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

			stdstr(prefixPath, path.c_str());
			path += "mods.conf";
			stdstr(configPath, path.c_str());
			return;
		}

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking $SWORD_PATH for mods.d...";
#endif

		if (FileMgr::existsDir(path.c_str(), "mods.d")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

			stdstr(prefixPath, path.c_str());
			path += "mods.d";
			stdstr(configPath, path.c_str());
			*configType = 1;
			return;
		}
	}


	// check for systemwide /etc/sword.conf

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking for /etc/sword.conf...";
#endif

	if (!::access("/etc/sword.conf", 04)) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

		SWConfig etcconf("/etc/sword.conf");
		if ((entry = etcconf.Sections["Install"].find("DataPath")) != etcconf.Sections["Install"].end()) {
			path = (*entry).second;
			if (((*entry).second.c_str()[strlen((*entry).second.c_str())-1] != '\\') && ((*entry).second.c_str()[strlen((*entry).second.c_str())-1] != '/'))
				path += "/";

#ifndef __VISUALC__
if (debug)
	cerr << "DataPath in /etc/sword.conf is set to: " << path;
#endif

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking for mods.conf in DataPath ";
#endif
			if (FileMgr::existsFile(path.c_str(), "mods.conf")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

				stdstr(prefixPath, path.c_str());
				path += "mods.conf";
				stdstr(configPath, path.c_str());
				return;
			}

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking for mods.d in DataPath ";
#endif

			if (FileMgr::existsDir(path.c_str(), "mods.d")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

				stdstr(prefixPath, path.c_str());
				path += "mods.d";
				stdstr(configPath, path.c_str());
				*configType = 1;
				return;
			}
		}
	}


	// check ~/.sword/

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking home directory for ~/.sword/mods.conf" << path;
#endif

	if (envhomedir != NULL) {
		path = envhomedir;
		if ((envhomedir[strlen(envhomedir)-1] != '\\') && (envhomedir[strlen(envhomedir)-1] != '/'))
			path += "/";
		path += ".sword/";
		if (FileMgr::existsFile(path.c_str(), "mods.conf")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

			stdstr(prefixPath, "");
			path += "mods.conf";
			stdstr(configPath, path.c_str());
			return;
		}

#ifndef __VISUALC__
if (debug)
	cerr << "\nChecking home directory for ~/.sword/mods.d" << path;
#endif

		if (FileMgr::existsDir(path.c_str(), "mods.d")) {

#ifndef __VISUALC__
if (debug)
	cerr << "found\n";
#endif

			stdstr(prefixPath, "");
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
   string newmodfile;
 
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


void SWMgr::Load() {
	if (!config) {	// If we weren't passed a config object at construction, find a config file
		if (!configPath)	// If we weren't passed a config path at construction...
			findConfig(&configType, &prefixPath, &configPath);
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

//	augment config with ~/.sword/mods.d if it exists ---------------------
			char *envhomedir  = getenv ("HOME");
			if (envhomedir != NULL && configType != 2) { // 2 = user only
				string path = envhomedir;
				if ((envhomedir[strlen(envhomedir)-1] != '\\') && (envhomedir[strlen(envhomedir)-1] != '/'))
					path += "/";
				path += ".sword/";
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
					delete myconfig;
					config = myconfig = saveConfig;
				}
			}
// -------------------------------------------------------------------------

	}
	else {
		SWLog::systemlog->LogError("SWMgr: Can't find 'mods.conf' or 'mods.d'.  Try setting:\n\tSWORD_PATH=<directory containing mods.conf>\n\tOr see the README file for a full description of setup options (%s)", (configPath) ? configPath : "<configPath is null>");
#ifndef WIN32
		throw "SWMgr: Can't find 'mods.conf' or 'mods.d' .  Try setting:\n\tSWORD_PATH=<directory containing mods.conf>\n\tOr see the README file for a full description of setup options";
#else
		exit(-1);
#endif
	}
}


SWModule *SWMgr::CreateMod(string name, string driver, ConfigEntMap &section)
{
	string description, datapath, misc1;
	ConfigEntMap::iterator entry;
	SWModule *newmod = 0;
 	string sourceformat;
 	string encoding;
     bool unicode = false;

	sourceformat = ((entry = section.find("SourceType"))  != section.end()) ? (*entry).second : (string)"";
	encoding = ((entry = section.find("Encoding"))  != section.end()) ? (*entry).second : (string)"";
	description  = ((entry = section.find("Description")) != section.end()) ? (*entry).second : (string)"";
	datapath = prefixPath;
	if ((prefixPath[strlen(prefixPath)-1] != '\\') && (prefixPath[strlen(prefixPath)-1] != '/'))
		datapath += "/";
	misc1 += ((entry = section.find("DataPath")) != section.end()) ? (*entry).second : (string)"";
	char *buf = new char [ strlen(misc1.c_str()) + 1 ];
	char *buf2 = buf;
	strcpy(buf, misc1.c_str());
//	for (; ((*buf2) && ((*buf2 == '.') || (*buf2 == '/') || (*buf2 == '\\'))); buf2++);
	for (; ((*buf2) && ((*buf2 == '/') || (*buf2 == '\\'))); buf2++);
	if (*buf2)
		datapath += buf2;
	delete [] buf;
	
    	if ((!stricmp(sourceformat.c_str(), "PlainUnicode")) || (!stricmp(encoding.c_str(), "UTF-8")))
     	unicode = true;

	if ((!stricmp(driver.c_str(), "zText")) || (!stricmp(driver.c_str(), "zCom"))) {
		SWCompress *compress = 0;
		int blockType = CHAPTERBLOCKS;
		misc1 = ((entry = section.find("BlockType")) != section.end()) ? (*entry).second : (string)"CHAPTER";
		if (!stricmp(misc1.c_str(), "VERSE"))
			blockType = VERSEBLOCKS;
		else if (!stricmp(misc1.c_str(), "CHAPTER"))
			blockType = CHAPTERBLOCKS;
		else if (!stricmp(misc1.c_str(), "BOOK"))
			blockType = BOOKBLOCKS;

		misc1 = ((entry = section.find("CompressType")) != section.end()) ? (*entry).second : (string)"LZSS";
#ifndef EXCLUDEZLIB
		if (!stricmp(misc1.c_str(), "ZIP"))
			compress = new ZipCompress();
		else
#endif
		if (!stricmp(misc1.c_str(), "LZSS"))
			compress = new LZSSCompress();
		
		if (compress) {
			if (!stricmp(driver.c_str(), "zText"))
				newmod = new zText(datapath.c_str(), name.c_str(), description.c_str(), blockType, compress);
			else	newmod = new zCom(datapath.c_str(), name.c_str(), description.c_str(), blockType, compress);
		}
	}
	
	if (!stricmp(driver.c_str(), "RawText")) {
		newmod = new RawText(datapath.c_str(), name.c_str(), description.c_str(), 0, unicode);
	}
	
	// backward support old drivers
	if (!stricmp(driver.c_str(), "RawGBF")) {
		newmod = new RawText(datapath.c_str(), name.c_str(), description.c_str());
	}

	if (!stricmp(driver.c_str(), "RawCom")) {
		newmod = new RawCom(datapath.c_str(), name.c_str(), description.c_str());
	}
				
	if (!stricmp(driver.c_str(), "RawFiles")) {
		newmod = new RawFiles(datapath.c_str(), name.c_str(), description.c_str());
	}
				
	if (!stricmp(driver.c_str(), "HREFCom")) {
		misc1 = ((entry = section.find("Prefix")) != section.end()) ? (*entry).second : (string)"";
		newmod = new HREFCom(datapath.c_str(), misc1.c_str(), name.c_str(), description.c_str());
	}
				
	if (!stricmp(driver.c_str(), "RawLD"))
		newmod = new RawLD(datapath.c_str(), name.c_str(), description.c_str());

	if (!stricmp(driver.c_str(), "RawLD4"))
		newmod = new RawLD4(datapath.c_str(), name.c_str(), description.c_str());

    // if a specific module type is set in the config, use this
    if ((entry = section.find("Type")) != section.end()) 
        newmod->Type(entry->second.c_str());

	return newmod;
}


void SWMgr::AddGlobalOptions(SWModule *module, ConfigEntMap &section, ConfigEntMap::iterator start, ConfigEntMap::iterator end)
{
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
}


void SWMgr::AddLocalOptions(SWModule *module, ConfigEntMap &section, ConfigEntMap::iterator start, ConfigEntMap::iterator end)
{
	for (;start != end; start++) {
		printf("%s:%s\n", module->Name(), (*start).second.c_str());
	}
}


void SWMgr::AddRawFilters(SWModule *module, ConfigEntMap &section) {
	string sourceformat, cipherKey;
	ConfigEntMap::iterator entry;

	cipherKey = ((entry = section.find("CipherKey")) != section.end()) ? (*entry).second : (string)"";
	if (!cipherKey.empty()) {
		SWFilter *cipherFilter = new CipherFilter(cipherKey.c_str());
		cipherFilters.insert(FilterMap::value_type(module->Name(), cipherFilter));
		cleanupFilters.push_back(cipherFilter);
		module->AddRawFilter(cipherFilter);
	}
}


void SWMgr::AddRenderFilters(SWModule *module, ConfigEntMap &section) {
	string sourceformat;
	ConfigEntMap::iterator entry;

	sourceformat = ((entry = section.find("SourceType")) != section.end()) ? (*entry).second : (string)"";

	// Temporary: To support old module types
	if (sourceformat.empty()) {
		sourceformat = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (string)"";
		if (!stricmp(sourceformat.c_str(), "RawGBF"))
			sourceformat = "GBF";
		else sourceformat = "";
	}

// process module	- eg. follows
//	if (!stricmp(sourceformat.c_str(), "GBF")) {
//		module->AddRenderFilter(gbftortf);
//	}

}


void SWMgr::AddStripFilters(SWModule *module, ConfigEntMap &section)
{
	string sourceformat;
	ConfigEntMap::iterator entry;

	sourceformat = ((entry = section.find("SourceType")) != section.end()) ? (*entry).second : (string)"";
	// Temporary: To support old module types
	if (sourceformat.empty()) {
		sourceformat = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (string)"";
		if (!stricmp(sourceformat.c_str(), "RawGBF"))
			sourceformat = "GBF";
		else sourceformat = "";
	}
	
	if (!stricmp(sourceformat.c_str(), "GBF")) {
		module->AddStripFilter(gbfplain);
	}
	if (!stricmp(sourceformat.c_str(), "ThML")) {
		module->AddStripFilter(thmlplain);
	}
}


void SWMgr::CreateMods() {
	SectionMap::iterator it;
	ConfigEntMap::iterator start;
	ConfigEntMap::iterator end;
	ConfigEntMap::iterator entry;
	SWModule *newmod;
	string driver, misc1;
	for (it = config->Sections.begin(); it != config->Sections.end(); it++) {
		ConfigEntMap &section = (*it).second;
		newmod = 0;
		
		driver = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (string)"";
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
				
				Modules.insert(ModMap::value_type(newmod->Name(), newmod));
			}
		}
	}
}


void SWMgr::DeleteMods() {

	ModMap::iterator it;

	for (it = Modules.begin(); it != Modules.end(); it++)
		delete (*it).second;

	Modules.erase(Modules.begin(), Modules.end());
}


void SWMgr::InstallScan(const char *dirname)
{
   DIR *dir;
   struct dirent *ent;
   int conffd = 0;
   string newmodfile;
   string targetName;
 
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
		if (!stricmp(option, (*it).second->getOptionName()))
			(*it).second->setOptionValue(value);
	}
}


const char *SWMgr::getGlobalOption(const char *option)
{
	for (FilterMap::iterator it = optionFilters.begin(); it != optionFilters.end(); it++) {
		if (!stricmp(option, (*it).second->getOptionName()))
			return (*it).second->getOptionValue();
	}
	return 0;
}


const char *SWMgr::getGlobalOptionTip(const char *option)
{
	for (FilterMap::iterator it = optionFilters.begin(); it != optionFilters.end(); it++) {
		if (!stricmp(option, (*it).second->getOptionName()))
			return (*it).second->getOptionTip();
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
		if (!stricmp(option, (*it).second->getOptionName())) {
			options = (*it).second->getOptionValues();
			break;	// just find the first one.  All option filters with the same option name should expect the same values
		}
	}
	return options;
}


char SWMgr::setCipherKey(const char *modName, const char *key) {
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
