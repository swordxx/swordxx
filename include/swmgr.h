/******************************************************************************
 *  swmgr.h   - definition of class SWMgr used to interact with an install
 *				base of sword modules.
 *
 * $Id: swmgr.h,v 1.34 2001/12/18 04:47:40 chrislit Exp $
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

/** @mainpage The Sword Project 1.5.2 - API documentation
* This page describes the structure of the Sword library.
* Sword provides an interface to different modules (Bibles/Commentaries/Lexicons)
* on disk. The object to work directly with the modules is SWModule.
* Use the class SWMgr to manage the modules.
*
* If you want to write your own frontend for Sword please have a look at the already existing ones.
* Well knwon frontends are:\n
* 	-BibleCS for Windows (the sourcecode is availble in the CVS of crosswire.org)\n
*	-GnomeSword (http://gnomesword.sourceforge.net/)\n
*	-BibleTime (http://www.bibletime.de/)\n
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
#include <swfiltermgr.h>

#include <defs.h>

using namespace std;

typedef map < string, SWModule *, less < string > >ModMap;
typedef list < string > OptionsList;
typedef map < string, SWFilter * >FilterMap;

/** The main class of Sword to handle all other things.
  * SWmgr manages the installed modules, the filters and global options like footnotes or strong numbers.
  * The class SWMgr is the most important class of Sword. It is used to manage the installed modules.
  * It also manages the filters (Render-, Strip- and Rawfilters).
  *
  * @see AddRawFilters(), AddRenderFilters(), AddStripFilters()
  * @version $Id: swmgr.h,v 1.34 2001/12/18 04:47:40 chrislit Exp $
  */
class SWDLLEXPORT SWMgr {

private:
	void commonInit(SWConfig * iconfig, SWConfig * isysconfig, bool autoload, SWFilterMgr *filterMgr);

protected:
  SWFilterMgr *filterMgr;		//made protected because because BibleTime needs it
  SWConfig * myconfig;		//made protected because because BibleTime needs it
  SWConfig *mysysconfig;
  void CreateMods ();
  SWModule *CreateMod (string name, string driver, ConfigEntMap & section);
  void DeleteMods ();
  char configType;		// 0 = file; 1 = directory
  FilterMap optionFilters;
  FilterMap cipherFilters;
  SWFilter *gbfplain;
  SWFilter *thmlplain;
  FilterList cleanupFilters;
  OptionsList options;
  virtual void init (); // use to initialize before loading modules
  virtual char AddModToConfig (int conffd, const char *fname);
  virtual void loadConfigDir (const char *ipath);
  virtual void AddGlobalOptions (SWModule * module, ConfigEntMap & section,
				 ConfigEntMap::iterator start,
				 ConfigEntMap::iterator end);
  virtual void AddLocalOptions (SWModule * module, ConfigEntMap & section,
				ConfigEntMap::iterator start,
				ConfigEntMap::iterator end);

  /**
    * Adds the encoding filters which are defined in "section" to the SWModule object "module".
    * @param module To this module the encoding filter(s) are added
    * @param section We use this section to get a list of filters we should apply to the module
    */
  virtual void AddEncodingFilters (SWModule * module, ConfigEntMap & section);
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
  /** Enable / Disable debug output
    * Set this static bool to true to get more verbose debug messages from SWMgr
    */
  static bool debug;
  /** 
    *
    */
  static void findConfig (char *configType, char **prefixPath,
			  char **configPath);
  /** The global config object.
    * This is the global config object. It contains all items of all modules,
    * so lookups should use this config object.
    * If you want to save a cipher key or other things to the module config file,
    * do NOT use this object, because it would corrupt your configs after config->Save().
    */
  SWConfig *config;
  /**
    *
    */
  SWConfig *sysconfig;
  /** A map of all modules
    * This map contains the list of installed modules we use.
    */
  ModMap Modules;
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
    * @param filterMgr an SWFilterMgr subclass to use to manager filters on modules THIS WILL BE
    *		DELETED BY SWMgr
    */

	SWMgr (SWConfig * iconfig = 0, SWConfig * isysconfig = 0, bool autoload = true, SWFilterMgr *filterMgr = 0);


  /**
    *
    * @param filterMgr an SWFilterMgr subclass to use to manager filters on modules THIS WILL BE
    *		DELETED BY SWMgr
    */

	SWMgr (SWFilterMgr *filterMgr);


  /**
   *
   * @param autoload If this bool is true the constructor starts loading the
   * installed modules. If you reimplemented SWMgr you can set autoload=false
   * to load the modules with your own reimplemented function.
   * @param filterMgr an SWFilterMgr subclass to use to manager filters on
   * modules THIS WILL BE DELETED BY SWMgr
   *
   */
  SWMgr (const char *iConfigPath, bool autoload = true, SWFilterMgr *filterMgr = 0);
  /**
    * The destructor of SWMgr.
    * This destrutcot cleans up the modules and deletes the created object.
    * Destroy the SWMgr at last in your application.
    */
  virtual ~SWMgr ();
  /**Installs a scan for modules in the directory givan as parameter.
    * @param dir The directory where new modules should be searched.
    */
  virtual void InstallScan (const char *dir);
  /**  Load the modules.
    * Reimplement this function to use your own Load function, 
    * for example to use your own filters.
    */
  virtual void Load ();

  /** Get the text encoding for a module
    * Gets the native text encoding for a module.
    * This value is important for setting the proper filters for displaying text
    * from a module.
    *
    * @param section The configuration section for the module that we want the source encoding for
    * @return The source encoding for text from the module.
    */
   virtual SWTextEncoding getTextEncoding (ConfigEntMap & section);

  /** Get the text encoding for a module
    * Gets the native text encoding for a module.
    * This value is important for setting the proper filters for displaying text
    * from a module.  Looks up the correct section based on the module name,
    * then calls the sister function getTextEncoding( ConfigEntMap &section ).
    *
    * @param section The name of the module that we want the source encoding for.
    * @return The source encoding for text from the SWModule module.
    */
   virtual SWTextEncoding getTextEncoding (string name);

  /** Get the text format for a module
    * Gets the native text format for a module.
    * This value is important for setting the proper filters for displaying text
    * from a module.  This function is passed a name in addition
    * to a section because it needs the module name in some cases.
    *
    * @param section The configuration section for the module that we want the source format for.
    * @return The source format for text from the SWModule module.
    */
   virtual SWTextMarkup getTextMarkup (ConfigEntMap & section);

  /** Get the text format for a module
    * Gets the native text format for a module.
    * This value is important for setting the proper filters for displaying text
    * from a module.  Looks up the correct section based on the module name,
    * then calls the sister function getTextMarkup( string name, ConfigEntMap &section ).
    *
    * @param module The name of the module for which we want the source format.
    * @return The source format for text from the SWModule module.
    */
   virtual SWTextMarkup getTextMarkup (string name);

  /** Set a global option
   * Set a global option using the parameters. A global option could be for
   * example footnotes.
   * @param option The name of the option, for which you want to change the
   * value. Well known and often used values are "Footnotes" or "Strongs"
   * @param value The value. Common values are "On" and "Off"
   */
  virtual void setGlobalOption (const char *option, const char *value);
  /** Gives the value of the given option
    * @param The option, which should be used to return the value of it
    * @return The value of the given option
    */
  virtual const char *getGlobalOption (const char *option);
  /** Gives a description for the given option
    * @param option The option, which should be used
    * @return A description of the given option
    * @see setGlobalOption, getGlobalOption, getGlobalOptions
    */
  virtual const char *getGlobalOptionTip (const char *option);
  /** A list of all availble options with the currently set  values
    * @return This function returns a list of global options.
    */
  virtual OptionsList getGlobalOptions ();
  /**
    *
    */
  virtual OptionsList getGlobalOptionValues (const char *option);
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
   * @paran key This is the new unlck key we use for te module.
   */
  virtual signed char setCipherKey (const char *modName, const char *key);
  
};
#endif
