/******************************************************************************
 *  swmgr.h   - definition of class SWMgr used to interact with an install
 *				base of sword modules.
 *
 * $Id: swmgr.h,v 1.20 2001/02/09 15:38:51 jansorg Exp $
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

#include <sys/types.h>
#include <string>
#include <map>
#include <list>
#include <swmodule.h>
#include <swconfig.h>
#include <swlog.h>

#include <defs.h>

// using namespace std;

typedef map < string, SWModule *, less < string > >ModMap;
typedef list < string > OptionsList;
typedef map < string, SWFilter * >FilterMap;

/**
  * SWmgr manages the installed modules, the filters and global options like footnotes or strong numbers.
  * The class SWMgr is the most important class of Sword. It is used to manage the installed modules.
  * It also manages the filters (Render-, Strip- and Rawfilters).
  *
  * @see AddRawFilters(), AddRenderFilters(), AddStripFilters()
  * @version $Id: swmgr.h,v 1.20 2001/02/09 15:38:51 jansorg Exp $
  */
class SWDLLEXPORT SWMgr
{
protected:
  SWConfig * myconfig;		//made protected because because BibleTime needs it
  SWConfig *mysysconfig;
  void CreateMods ();
  SWModule *CreateMod (string name, string driver, ConfigEntMap & section);
  void DeleteMods ();
  char configType;		// 0 = file; 1 = directory
  FilterMap optionFilters;
  FilterMap cipherFilters;
  SWFilter *gbfplain;
  FilterList cleanupFilters;
  OptionsList options;
  /**
    *
    */
  virtual void init (); // use to initialize before loading modules
	/**
    *
    */
  virtual char AddModToConfig (int conffd, const char *fname);
  /**
    *
    */
  virtual void loadConfigDir (const char *ipath);
  /**
    *
    */
  virtual void AddGlobalOptions (SWModule * module, ConfigEntMap & section,
				 ConfigEntMap::iterator start,
				 ConfigEntMap::iterator end);
  /**
    *
    */
  virtual void AddLocalOptions (SWModule * module, ConfigEntMap & section,
				ConfigEntMap::iterator start,
				ConfigEntMap::iterator end);
  /**
    * Adds the render filters which are defined in "section" to the SWModule object "module".
    * @param module To this module the render filter(s) are added
    * @param section We use this section to get a list of filters we should apply to the module
    */
  virtual void AddRenderFilters (SWModule * module, ConfigEntMap & section);
  /**
    * Adds the strip filters which are defined in "section" to the SWModule object "module".
    * @param module To this module the strip filter(s) are added
    * @param section We use this section to get a list of filters we should apply to the module
    */
  virtual void AddStripFilters (SWModule * module, ConfigEntMap & section);
  /**
    * Adds the raw filters which are defined in "section" to the SWModule object "module".
    * @param module To this module the raw filter(s) are added
    * @param section We use this section to get a list of filters we should apply to the module
    */
  virtual void AddRawFilters (SWModule * module, ConfigEntMap & section);

public:
  /**
    * Set this static bool to true to get more verbose debug messages from SWMgr
    */
  static bool debug;
  /**
    *
    */
  static void findConfig (char *configType, char **prefixPath,
			  char **configPath);

  /**
    *
    */
  SWConfig *config;
  /**
    *
    */
  SWConfig *sysconfig;
  /**
    * This map contains the list of installed modules we use.
    */
  ModMap Modules;
  /**
    *
    */
  char *prefixPath;
  /**
    *
    */
  char *configPath;
  /**
    * Constructor of SWMgr.
    * @param iconfig
    * @param isysconfig
    * @param autoload If this bool is true the constructor starts loading the installed modules. If you reimplemented SWMgr you can set autoload=false to load the modules with your own reimplemented function.
    */
  SWMgr (SWConfig * iconfig = 0, SWConfig * isysconfig = 0, bool autoload = true);
  /**
    *
    */
  SWMgr (const char *iConfigPath, bool autoload = true);
  /**
    * The destructor of SWMgr.
    * This destrutcot cleans up the modules and deletes the created object.
    * Destroy the SWMgr at last in your application.
    */
  virtual ~SWMgr ();
  /**
    *
    */
  virtual void InstallScan (const char *dir);
  /**
    * Load the modules.
    * Reimplement this function to use your own Load function, for example to use your own filters.
    */
  virtual void Load ();
  /**
    *
    */
  virtual void setGlobalOption (const char *option, const char *value);
  /**
    *
    */
  virtual const char *getGlobalOption (const char *option);
  /**
    *
    */
  virtual const char *getGlobalOptionTip (const char *option);
  /**
    * This function returns a list of global options.
    * The list of global options contains the names and the status of the global options.
    */
  virtual OptionsList getGlobalOptions ();
  /**
    *
    */
  virtual OptionsList getGlobalOptionValues (const char *option);
  /**
    * Sets the cipher key for the given module. This function updates the key at runtime,
    * but it does not write to the config file.
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
    * @paran key This is the new unlck key we use for te module.
    */
  virtual char setCipherKey (const char *modName, const char *key);
};
#endif
