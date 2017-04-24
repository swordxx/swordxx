/******************************************************************************
 *
 *  swmgr.h -    definition of class SWMgr used to interact with an install
 *        base of Sword++ modules.
 *
 * $Id$
 *
 * Copyright 1997-2014 CrossWire Bible Society (http://www.crosswire.org)
 *    CrossWire Bible Society
 *    P. O. Box 2528
 *    Tempe, AZ  85280-2528
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

/** @mainpage The Sword++ Project - API documentation
 * This is the API documentation for The Sword++ Project.
 * It describes the structure of the Sword++ library and documents the functions of the classes.
 * From time to time this documentation gives programming examples, too.
 *
 * Sword++ provides a simple to use engine for working with many types of texts including Bibles,
 *    commentaries, lexicons, glossaries, daily devotionals, and others.
 *
 * Some main classes:
 *
 * SWMgr gives access to an installed library of modules (books).
 * SWModule represents an individual module
 * SWKey represents a location into a module (e.g. "John 3:16")
 *
 * An API Primer can be found at:
 *
 * http://crosswire.org/sword/develop/swordapi/apiprimer.jsp
 *
 */

#ifndef SWMGR_H
#define SWMGR_H

#include <memory>
#include <map>
#include <list>
#include <string>
#include "defs.h"
#include "normalizedpath.h"
#include "swconfig.h"


namespace swordxx {

class CipherFilter;
class SWModule;
class SWFilter;
class SWOptionFilter;
class SWFilterMgr;
class SWKey;

class FileDesc;
class SWOptionFilter;

/** SWMgr exposes an installed module set
 *
 * SWMgr exposes an installed module set and can be asked to configure the desired
 *    markup and options which modules will produce.
 *
 * @version $Id$
 */
class SWDLLEXPORT SWMgr {

public: /* Types: */

    using ModMap = std::map<std::string, std::unique_ptr<SWModule> >;

private: /* Types: */

    using FilterMap = std::map<std::string, std::shared_ptr<SWFilter> >;

private:
    ModMap m_modules;
    bool const mgrModeMultiMod;
    bool augmentHome = true;
    void init(); // use to initialize before loading modules

protected:
    SWFilterMgr *filterMgr;        //made protected because because BibleTime needs it
    SWConfig * myconfig = nullptr; // made protected because because BibleTime needs it
    SWConfig * mysysconfig = nullptr;
    SWConfig * homeConfig = nullptr;
    void CreateMods(bool multiMod = false);
    virtual std::unique_ptr<SWModule> createModule(
            char const * name,
            char const * driver,
            ConfigEntMap section);
    void DeleteMods();
    char configType = 0;        // 0 = file; 1 = directory
    std::map<std::string, std::shared_ptr<SWOptionFilter> > m_optionFilters;
    std::map<std::string, std::shared_ptr<CipherFilter> > m_cipherFilters;
    std::shared_ptr<SWFilter> m_gbfplain;
    std::shared_ptr<SWFilter> m_thmlplain;
    std::shared_ptr<SWFilter> m_osisplain;
    std::shared_ptr<SWFilter> m_teiplain;
    std::shared_ptr<SWOptionFilter> transliterator;

    FilterMap extraFilters;
    std::list<std::string> options;
    virtual char AddModToConfig(FileDesc *conffd, const char *fname);
    virtual void loadConfigDir(const char *ipath);

    /// \todo Does this really need to be virtual?
    virtual void addGlobalOptions(SWModule & module,
                                  ConfigEntMap const & section,
                                  ConfigEntMap::const_iterator start,
                                  ConfigEntMap::const_iterator end);

    /// \todo Does this really need to be virtual?
    virtual void addLocalOptions(SWModule & module,
                                 ConfigEntMap const & section,
                                 ConfigEntMap::const_iterator start,
                                 ConfigEntMap::const_iterator end);
    std::list<std::string> augPaths;

    /**
      \brief Called to add appropriate Encoding Filters to a module.

      Override this to do special actions, if desired. See the module.conf
      Encoding= entry.
      \param module module to which to add Encoding Filters.
      \param section configuration information from module.conf.
      \todo Does this really need to be virtual?
    */
    virtual void addEncodingFilters(SWModule & module,
                                    ConfigEntMap const & section);

    /**
      \brief Called to add appropriate Render Filters to a module.

       Override to do special actions, if desired. See the module.conf
       SourceType= entry.
       \param module module to which to add Render Filters.
       \param section configuration information from module.conf.
       \todo Does this really need to be virtual?
     */
    virtual void addRenderFilters(SWModule & module,
                                  ConfigEntMap const & section);

    /**
      \brief Called to add appropriate Strip Filters to a module.

      Override to do special actions, if desired. See the module.conf
      SourceType= entry.
      \param module module to which to add Strip Filters.
      \param section configuration information from module.conf.
      \todo Does this really need to be virtual?
     */
    virtual void addStripFilters(SWModule & module,
                                 ConfigEntMap const & section);

    // ones manually specified in .conf file
    /// \todo Does this really need to be virtual?
    virtual void addStripFilters(SWModule & module,
                                 ConfigEntMap const & section,
                                 ConfigEntMap::const_iterator start,
                                 ConfigEntMap::const_iterator end);

    /**
      \brief Called to add appropriate Raw Filters to a module.

      Override to do special actions, if desired. See the module.conf
      CipherKey= entry.
      \param module module to which to add Raw Filters.
      \param section configuration information from module.conf.
      \todo Does this really need to be virtual?
     */
    virtual void addRawFilters(SWModule & module, ConfigEntMap const & section);


public:

    static const char *globalConfPath;
    static std::string getHomeDir();

    /**
     *
     */
    static void findConfig(char * configType,
                           std::string & prefixPath,
                           std::string & configPath,
                           std::list<std::string> * augPaths = nullptr,
                           SWConfig ** providedSysConf = nullptr);

    SWConfig *config;
    SWConfig *sysConfig;

    /** The path to main module set and locales
     */
    std::string m_prefixPath;

    /** path to main module set configuration
     */
    std::string m_configPath;

    ModMap const & modules() const noexcept { return m_modules; }

    /** Gets a specific module by name.  e.g. SWModule *kjv = myManager.getModule("KJV");
     * @param modName the name of the module to retrieve
     * @return the module, if found, otherwise 0
     */
    SWModule * getModule(char const * modName) {
        auto const it(m_modules.find(modName));
        return (it != m_modules.end()) ? it->second.get() : nullptr;
    }

    SWModule const * getModule(char const * modName) const {
        auto const it(m_modules.find(modName));
        return (it != m_modules.end()) ? it->second.get() : nullptr;
    }


    /** Constructs an instance of SWMgr
     *
     * @param iconfig manually supply a configuration.  If not supplied, SWMgr will look on the system
     *    using a complex hierarchical search.  See README for detailed specifics.
     * @param isysconfig
     * @param autoload whether or not to immediately load modules on construction of this SWMgr.
     *    If you reimplemented SWMgr you can set this to false and call SWMgr::Load() after you have
     *    completed the contruction and setup of your SWMgr subclass.
     * @param filterMgr an SWFilterMgr subclass to use to manager filters on modules
     *    SWMgr TAKES OWNERSHIP FOR DELETING THIS OBJECT
     *    For example, this will create an SWMgr and cause its modules to produce HTMLHREF formatted output
     *    when asked for renderable text:
     *
     *    SWMgr *myMgr = new SWMgr(0, 0, true, new MarkupFilterMgr(FMT_HTMLHREF));
     */
    SWMgr(SWConfig * iconfig = nullptr,
          SWConfig * isysconfig = nullptr,
          bool autoload = true,
          SWFilterMgr * filterMgr = nullptr,
          bool multiMod = false);

    /**
     */
    SWMgr(SWFilterMgr *filterMgr, bool multiMod = false);

    /**
     * @param iConfigPath provide a custom path to use for module set location, instead of
     *    searching the system for it.
     */
    SWMgr(char const * iConfigPath,
          bool autoload = true,
          SWFilterMgr * filterMgr = nullptr,
          bool multiMod = false,
          bool augmentHome = true);

    /** The destructor of SWMgr.
     * This function cleans up the modules and deletes the created object.
     * Destroying the SWMgr causes all retrieved SWModule object to be invalid, so
     *    be sure to destroy only when retrieved objects are no longer needed.
     */
    virtual ~SWMgr();

    /**
     * Adds books from a new path to the library
     * @param path the path in which to search for books
     * @param multiMod whether or not to keep multiple copies of the same book if found in different paths
     *        default - false, uses last found version of the book
     */
    virtual void augmentModules(NormalizedPath const & path, bool multiMod = false);

    void deleteModule(const char *);

    /** Looks for any newly installed module.conf file in the path provided,
     *    displays the copyright information to the user, and then copies the
     *    module.conf to the main module set's mods.d directory
     * @param dir where to search for new modules
     */
    virtual void InstallScan(const char *dir);

    /** Load all modules.  Should only be manually called if SWMgr was constructed
     *    without autoload; otherwise, this will be called on SWMgr construction
     * Reimplement this function to supply special functionality when modules are
     * initially loaded.
     */
    virtual signed char Load();

    /** Change the values of global options (e.g. Footnotes, Strong's Number, etc.)
     * @param option The name of the option, for which you want to change the
     * value. Well known and often used values are "Footnotes" or "Strong's Numbers"
     * @param value new value. Common values are "On" and "Off"
     */
    virtual void setGlobalOption(const char *option, const char *value);

    /** Get the current value of the given option
     * @param option the name of the option, who's value is desired
     * @return the value of the given option
     */
    virtual const char *getGlobalOption(const char *option);

    /** Gets a brief description for the given option
     * @param option the name of the option, who's tip is desired
     * @return description text
     * @see setGlobalOption, getGlobalOption, getGlobalOptions
     */
    virtual const char *getGlobalOptionTip(const char *option);

    /** Gets a list of all available option names
     * @return list of option names
     */
    virtual std::list<std::string> getGlobalOptions();

    /** Gets a list of legal values to which a specific option
     *    may be set
     * @param option the name of the option, who's legal values are desired
     * @return a list of legal values for the given option
     */
    virtual std::list<std::string> getGlobalOptionValues(const char * option);

    /** Filters a buffer thru a named filter
     * @param filterName
     * @param text buffer to filter
     * @param key context key if filter needs this for processing
     * @param module context module if filter needs this for processing
     * @return error status
     */
    virtual char filterText(char const * filterName,
                            std::string & text,
                            SWKey const * key = nullptr,
                            SWModule const * module = nullptr);

    /**
     * Sets the cipher key for the given module. This function updates the key
     * at runtime, but it does not write to the config file.
     * To write the new unlock key to the config file use code like this:
     *
     * @code
     * if (auto dir = opendir(configPath)) { // Find and update .conf file
     *   rewinddir(dir);
     *   struct dirent * ent;
     *   while ((ent = readdir(dir))) {
     *     if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
     *       std::string modFile(m_backend->configPath);
     *       modFile.push_back('/');
     *       modFile.append(ent->d_name);
     *       auto myConfig(std::make_unique<SWConfig>(modFile));
     *       auto section(myConfig->Sections.find(m_module->Name()));
     *       if (section != myConfig->Sections.end()) {
     *         auto entry(section->second.find("CipherKey"));
     *         if (entry != section->second.end()) {
     *           entry->second = unlockKey; // Set cipher key
     *           myConfig->Save(); // Save config file
     *         }
     *       }
     *     }
     *   }
     *   closedir(dir);
     * }
     * @endcode
     *
     * @param modName For this module we change the unlockKey
     * @param key This is the new unlck key we use for te module.
     */
    virtual bool setCipherKey(std::string const & modName,
                              char const * key);
};

} /* namespace swordxx */
#endif
