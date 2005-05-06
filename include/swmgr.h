/******************************************************************************
*  swmgr.h   - definition of class SWMgr used to interact with an install
*				base of sword modules.
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

/** @mainpage The Sword Project - API documentation
* This is the API documentation of the Sword project.
* it describes the structure of the Sword library and documents the functions of the classes.
* From time to time this documentation gives programming examples, too.
*
* Sword provides an interface to different modules (Bibles/Commentaries/Lexicons)
* on disk. The object to work directly with the modules is SWModule.
* Use the class SWMgr to manage the modules.
*
* If you want to write your own frontend for Sword please have a look at the already existing ones.
* Well known frontends are:\n
*	-BibleCS for Windows (the sourcecode is availble in the CVS of crosswire.org)\n
*	-GnomeSword (http://gnomesword.sourceforge.net/)\n
*	-BibleTime (http://www.bibletime.info/)\n
*/

#ifndef SWMGR_H
#define SWMGR_H

#include <sys/types.h>
#include <map>
#include <list>
#include <swmodule.h>
#include <swconfig.h>
#include <swlog.h>
#include <swfiltermgr.h>

#include <defs.h>

SWORD_NAMESPACE_START

typedef std::map < SWBuf, SWModule *, std::less < SWBuf > >ModMap;
typedef std::map < SWBuf, SWFilter * >FilterMap;
typedef std::list < SWBuf >StringList;

/** SWMgr is the main class of the Sword library.
*
* SWmgr manages the installed modules, the filters and global options like footnotes or strong numbers.
* The class SWMgr is the most important class of Sword. It is used to manage the installed modules.
* It also manages the filters (Render-, Strip- and Rawfilters).
*
* To get the SWModule objects of the instalelled modules use @ref Modules for this.
* @see AddRawFilters(), AddRenderFilters(), AddStripFilters()
* @version $Id$
*/

class FileDesc;

class SWDLLEXPORT SWMgr {

private:
	bool mgrModeMultiMod;
	bool augmentHome;
	void commonInit(SWConfig * iconfig, SWConfig * isysconfig, bool autoload, SWFilterMgr *filterMgr, bool multiMod = false);

protected:
	SWFilterMgr *filterMgr;		//made protected because because BibleTime needs it
	SWConfig * myconfig;		//made protected because because BibleTime needs it
	SWConfig *mysysconfig;
	SWConfig *homeConfig;
	void CreateMods(bool multiMod = false);
	SWModule *CreateMod(const char *name, const char *driver, ConfigEntMap & section);
	void DeleteMods();
	char configType;		// 0 = file; 1 = directory
	FilterMap optionFilters;
	FilterMap cipherFilters;
	SWFilter *gbfplain;
	SWFilter *thmlplain;
	SWFilter *osisplain;
	SWFilter *transliterator;
	FilterList cleanupFilters;
	StringList options;
	virtual void init(); // use to initialize before loading modules
	virtual char AddModToConfig(FileDesc *conffd, const char *fname);
	virtual void loadConfigDir(const char *ipath);
	virtual void AddGlobalOptions(SWModule * module, ConfigEntMap & section,
	ConfigEntMap::iterator start,
	ConfigEntMap::iterator end);
	virtual void AddLocalOptions(SWModule * module, ConfigEntMap & section,
	ConfigEntMap::iterator start,
	ConfigEntMap::iterator end);
	StringList augPaths;

	/**
	* Adds the encoding filters which are defined in "section" to the SWModule object "module".
	* @param module To this module the encoding filter(s) are added
	* @param section We use this section to get a list of filters we should apply to the module
	*/
	virtual void AddEncodingFilters(SWModule * module, ConfigEntMap & section);
	/**
	* Adds the render filters which are defined in "section" to the SWModule object "module".
	* @param module To this module the render filter(s) are added
	* @param section We use this section to get a list of filters we should apply to the module
	*/
	virtual void AddRenderFilters(SWModule * module, ConfigEntMap & section);
	/**
	* Adds the strip filters which are defined in "section" to the SWModule object "module".
	* @param module To this module the strip filter(s) are added
	* @param section We use this section to get a list of filters we should apply to the module
	*/
	virtual void AddStripFilters(SWModule * module, ConfigEntMap & section);
	/**
	* Adds the raw filters which are defined in "section" to the SWModule object "module".
	* @param module To this module the raw filter(s) are added
	* @param section We use this section to get a list of filters we should apply to the module
	*/
	virtual void AddRawFilters(SWModule * module, ConfigEntMap & section);


public:
	/** Enable / Disable debug output on runtime
	* Set this to true to get more verbose output of SWMgr at runtime. Set it to false to get no debug output.
	* The default is "false".
	*/
	static bool debug;
	static bool isICU;
	static const char *globalConfPath;
	/**
	*
	*/
	static void findConfig(char *configType, char **prefixPath, char **configPath, StringList *augPaths = 0);
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
	*
	*/
	SWConfig *sysconfig;
	/** The map of available modules.
	* This map contains the list of available modules in Sword.
	* Here's an example how to got through the map and how toc ehck for the module type.
	*
	*@code
	* ModMap::iterator it;
	* SWModule*	curMod = 0;
	*
	* for (it = Modules.begin(); it != Modules.end(); it++) {
	*     curMod = (*it).second;
	*     if (!strcmp(curMod->Type(), "Biblical Texts")) {
	*       //do something with curMod
	*     }
	*     else if (!strcmp(curMod->Type(), "Commentaries")) {
	*       //do something with curMod
	*     }
	*     else if (!strcmp(curMod->Type(), "Lexicons / Dictionaries")) {
	*       //do something with curMod
	*     }
	* }
	* @endcode
	*/
	ModMap Modules;
	SWModule *getModule(const char *modName) { ModMap::iterator it = Modules.find(modName); return ((it != Modules.end()) ? it->second : 0); }
	/** The path to your Sword directory
	*
	*/
	char *prefixPath;
	/**
	*
	*/
	char *configPath;

	/** Constructor of SWMgr.
	*
	* @param iconfig
	* @param isysconfig
	* @param autoload If this bool is true the constructor starts loading the installed modules. If you reimplemented SWMgr you can set autoload=false to load the modules with your own reimplemented function.
	* @param filterMgr an SWFilterMgr subclass to use to manager filters on modules THIS WILL BE DELETED BY SWMgr
	*/
	SWMgr(SWConfig * iconfig = 0, SWConfig * isysconfig = 0, bool autoload = true, SWFilterMgr *filterMgr = 0, bool multiMod = false);

	/**
	*
	* @param filterMgr an SWFilterMgr subclass to use to manager filters on modules THIS WILL BE DELETED BY SWMgr
	*/
	SWMgr(SWFilterMgr *filterMgr, bool multiMod = false);

	/**
	*
	* @param iConfigPath Path to config files.
	* @param autoload If this bool is true the constructor starts loading the
	* installed modules. If you reimplemented SWMgr you can set autoload=false
	* to load the modules with your own reimplemented function.
	* @param iConfigPath explicit path to use where modules exist
	* @param filterMgr an SWFilterMgr subclass to use to manager filters on
	* 		modules THIS WILL BE DELETED BY SWMgr
	* @param augmentHome whether or not to augment ~/.sword personal modules
	*		default is to augment modules,
	*
	*/
	SWMgr(const char *iConfigPath, bool autoload = true, SWFilterMgr *filterMgr = 0, bool multiMod = false, bool augmentHome = true);

	/** The destructor of SWMgr.
	* This function cleans up the modules and deletes the created object.
	* Destroy the SWMgr at last object in your application, because otherwise you may experience crashes
	* because the SWModule objects become invalid.
	*/
	virtual ~SWMgr();

	/**
	* Adds books from a new path to the library
	* @param path the path in which to search for books
	* @param multiMod whether or not to keep multiple copies of the same book if found in different paths
	*		default - false, uses last found version of the book
	*/
	virtual void augmentModules(const char *path, bool multiMod = false);
	void deleteModule(const char *);

	/**Installs a scan for modules in the directory givan as parameter.
	* @param dir The directory where new modules should be searched.
	*/
	virtual void InstallScan(const char *dir);
	/**  Load the modules.
	* Reimplement this function to use your own Load function,
	* for example to use your own filters.
	*/
	virtual signed char Load();
	/** Set a global option
	* Set a global option using the parameters. A global option could be for
	* example footnotes.
	* @param option The name of the option, for which you want to change the
	* value. Well known and often used values are "Footnotes" or "Strongs"
	* @param value The value. Common values are "On" and "Off"
	*/
	virtual void setGlobalOption(const char *option, const char *value);
	/** Gives the value of the given option
	* @param option The option, which should be used to return the value of it
	* @return The value of the given option
	*/
	virtual const char *getGlobalOption(const char *option);
	/** Gives a description for the given option
	* @param option The option, which should be used
	* @return A description of the given option
	* @see setGlobalOption, getGlobalOption, getGlobalOptions
	*/
	virtual const char *getGlobalOptionTip(const char *option);
	/** A list of all availble options with the currently set  values
	* @return This function returns a list of global options.
	*/
	virtual StringList getGlobalOptions();
	/**
	*
	*/
	virtual StringList getGlobalOptionValues(const char *option);
	/**
	* Sets the cipher key for the given module. This function updates the key
	* at runtime, but it does not write to the config file.
	* To write the new unlock key to the config file use code like this:
	*
	* @code
	* SectionMap::iterator section;
	* ConfigEntMap::iterator entry;
	* DIR *dir = opendir(configPath);
	* struct dirent *ent;
	* char* modFile;
	* if (dir) {    // find and update .conf file
	*   rewinddir(dir);
	*   while ((ent = readdir(dir)))
	*   {
	*     if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, "..")))
	*     {
	*       modFile = m_backend->configPath;
	*       modFile += "/";
	*       modFile += ent->d_name;
	*       SWConfig *myConfig = new SWConfig( modFile );
	*       section = myConfig->Sections.find( m_module->Name() );
	*       if ( section != myConfig->Sections.end() )
	*       {
	*         entry = section->second.find("CipherKey");
	*         if (entry != section->second.end())
	*         {
	*           entry->second = unlockKey;//set cipher key
	*           myConfig->Save();//save config file
	*         }
	*       }
	*       delete myConfig;
	*     }
	*   }
	* }
	* closedir(dir);
	* @endcode
	*
	* @param modName For this module we change the unlockKey
	* @param key This is the new unlck key we use for te module.
	*/
	virtual signed char setCipherKey(const char *modName, const char *key);
};

SWORD_NAMESPACE_END
#endif
