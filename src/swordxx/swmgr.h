/******************************************************************************
 *
 *  swmgr.h -    definition of class SWMgr used to interact with an install
 *        base of Sword++ modules.
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

#ifndef SWORDXX_SWMGR_H
#define SWORDXX_SWMGR_H

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
 */
class SWDLLEXPORT SWMgr {

public: /* Types: */

    using ModMap = std::map<std::string, std::shared_ptr<SWModule> >;

private: /* Types: */

    using FilterMap = std::map<std::string, std::shared_ptr<SWFilter> >;

private:
    ModMap m_modules;
    bool const mgrModeMultiMod;
    bool augmentHome = true;
    void init(); // use to initialize before loading modules

    /** The path to main module set and locales. */
    std::string m_prefixPath;
    std::shared_ptr<SWFilterMgr> const filterMgr;

protected:
    SWConfig * myconfig = nullptr; // made protected because because BibleTime needs it
    SWConfig * mysysconfig = nullptr;
    SWConfig * homeConfig = nullptr;
    void createAllModules();
    std::shared_ptr<SWModule> createModule(
            std::string const & name,
            std::string const & driver,
            ConfigEntMap section);
    void deleteAllModules();
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
    virtual char AddModToConfig(FileDesc & conffd, const char *fname);
    virtual void loadConfigDir(const char *ipath);

    /**
      Adds appropriate global option filters to a module. Global option filters
      typically update SourceType markup to turn on and off specific features
      of a text when a user has optionally chosen to show or hide that feature,
      e.g. Strongs, Footnotes, Headings, etc. Global options can also have more
      than On and Off values, but these are the most common. A set of all
      global options included from an entire library of installed modules can
      be obtained from getGlobalOptions and presented to the user.  Values to
      which each global option may be set can be obtain from
      getGlobalOptionValues, and similar.  See that family of methods for more
      information.
      See the module.conf GlobalOptionFilter= entries.
      \param module module to which to add encoding filters
      \param section configuration information for module
     */
    virtual void addGlobalOptionFilters(SWModule & module,
                                        ConfigEntMap const & section);

    /**
      Adds appropriate local option filters to a module. Local options are
      similar to global options in that they may be toggled on or off or set to
      some value from a range of choices but local option.
      See the module.conf LocalOptionFilter= entries.
      \param module module to which to add encoding filters
      \param section configuration information for module
    */
    virtual void addLocalOptionFilters(SWModule & module,
                                       ConfigEntMap const & section);
    std::list<std::string> augPaths;

    /**
      \brief Adds appropriate encoding filters to a module.

      See the module.conf Encoding= entry.
      \param module module to which to add encoding filters
      \param section configuration information for module
    */

    virtual void addEncodingFilters(SWModule & module,
                                    ConfigEntMap const & section);

    /**
      \brief Add appropriate render filters to a module.

      Render filters are used for preparing a text for display and typically
      convert markup from SourceType to desired display markup.
      See the module.conf SourceType= entry.
      \param module module to which to add render filters
      \param section configuration information for module
    */
    virtual void addRenderFilters(SWModule & module,
                                  ConfigEntMap const & section);

    /**
      \brief Adds appropriate strip filters to a module.

      Strip filters are used for preparing text for searching and typically
      strip out all markup and leave only searchable words.
      See the module.conf SourceType= entry.
      \param module module to which to add strip filters
      \param section configuration information for module
    */
    virtual void addStripFilters(SWModule & module,
                                 ConfigEntMap const & section);

    // ones manually specified in .conf file

    /**
      \brief Adds manually specified strip filters specified in module
             configuration as LocalStripFilters.

      These might take care of special cases of preparation for searching, e.g.,
      removing ()[] and underdot symbols from manuscript modules.
      \param module module to which to add local strip filters
      \param section configuration information for module
      \todo Does this really need to be virtual?
    */
    virtual void addLocalStripFilters(SWModule & module,
                                      ConfigEntMap const & section);

    /**
      \brief Add appropriate raw filters to a module.

      Raw filters are used to manipulate a buffer immediately after it has been
      read from storage.  For example, any decryption that might need to be
      done.
      See the module.conf CipherKey= entry.
      \param module module to which to add raw filters
      \param section configuration information for module
    */
    virtual void addRawFilters(SWModule & module, ConfigEntMap const & section);


public:

    static const char *globalConfPath;

    /**
     * Determines where SWORD looks for the user's home folder.  This is
     * typically used as a place to find any additional personal SWORD
     * modules which a user might wish to be added to a system-wide
     * library (e.g., added from ~/.sword/mods.d/ or ~/sword/mods.d/)
     *
     * or if a user or UI wishes to override SWORD system configuration
     * settings (e.g., /etc/sword.conf) with a custom configuration
     * (e.g., ~/.sword/sword.conf)
     */
    static std::string getHomeDir();

    /**
      Performs all the logic to discover a SWORD configuration and libraries on
      a system.
      \returns the type of configuration found.
    */
    static char findConfig(std::string & prefixPath,
                           std::string & configPath,
                           std::list<std::string> * augPaths = nullptr,
                           SWConfig ** providedSysConf = nullptr);

    /**
       The configuration of a loaded library of SWORD modules
       e.g., from /usr/share/sword/mods.d/
            augmented with ~/.sword/mods.d/

       This represents all discovered modules and their configuration
       compiled into a single SWConfig object with each [section]
       representing each module. e.g. [KJV]
    */
    SWConfig *config;

    /**
       The configuration file for SWORD e.g., /etc/sword.conf
    */
    SWConfig *sysConfig;

    /** path to main module set configuration
     */
    std::string m_configPath;

    /** \returns The map of available modules. */
    ModMap const & modules() const noexcept { return m_modules; }

    /** Gets a specific module by name.  e.g. SWModule *kjv = myManager.getModule("KJV");
     * @param modName the name of the module to retrieve
     * @return the module, if found, otherwise 0
     */
    std::shared_ptr<SWModule> getModule(char const * modName) {
        auto const it(m_modules.find(modName));
        return (it != m_modules.end()) ? it->second : nullptr;
    }

    std::shared_ptr<SWModule const> getModule(char const * modName) const {
        auto const it(m_modules.find(modName));
        return (it != m_modules.end()) ? it->second : nullptr;
    }


    /** Constructs an instance of SWMgr
     *
     * @param iconfig manually supply a configuration.  If not supplied, SWMgr will look on the system
     *    using a complex hierarchical search.  See README for detailed specifics.
     * @param isysconfig manually supply a an isysconfig (e.g. /etc/sword.conf)
     * @param autoload whether or not to immediately load modules on construction of this SWMgr.
     *    If you reimplemented SWMgr you can set this to false and call SWMgr::load() after you have
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
          std::shared_ptr<SWFilterMgr> filterMgr = nullptr,
          bool multiMod = false);

    /**
     */
    SWMgr(std::shared_ptr<SWFilterMgr> filterMgr, bool multiMod = false);

    /**
     * @param iConfigPath provide a custom path to use for module set location, instead of
     *    searching the system for it.
     */
    SWMgr(char const * iConfigPath,
          bool autoload = true,
          std::shared_ptr<SWFilterMgr> filterMgr = nullptr,
          bool multiMod = false,
          bool augmentHome = true);

    /** The destructor of SWMgr.
     * This function cleans up the modules and deletes the created object.
     * Destroying the SWMgr causes all retrieved SWModule object to be invalid, so
     *    be sure to destroy only when retrieved objects are no longer needed.
     */
    virtual ~SWMgr();

    /** \returns The path to main module set and locales. */
    std::string const & prefixPath() const noexcept { return m_prefixPath; }

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

    /**
      \brief Loads installed library of SWORD modules.

      Should only be manually called if SWMgr was constructed without autoload;
      otherwise, this will be called on SWMgr construction Reimplement this
      function to supply special functionality when modules are initially
      loaded. This includes discovery of config path with SWMgr::fileconfig,
      loading of composite SWMgr::config,
      and construction of all modules from config using SWMgr::createAllModules
    */
    virtual signed char load();

    /** Change the values of global options (e.g. Footnotes, Strong's Number, etc.)
     * @param option The name of the option, for which you want to change the
     * value. Well known and often used values are "Footnotes" or "Strong's Numbers"
     * @param value new value. Common values are "On" and "Off"
     */
    virtual void setGlobalOption(std::string_view option,
                                 std::string_view value);

    /** Get the current value of the given option
     * @param option the name of the option, who's value is desired
     * @return the value of the given option
     */
    virtual const char *getGlobalOption(std::string_view option);

    /** Gets a brief description for the given option
     * @param option the name of the option, who's tip is desired
     * @return description text
     * @see setGlobalOption, getGlobalOption, getGlobalOptions
     */
    virtual const char *getGlobalOptionTip(std::string_view option);

    /** Gets a list of all available option names
     * @return list of option names
     */
    virtual std::list<std::string> getGlobalOptions();

    /** Gets a list of legal values to which a specific option
     *    may be set
     * @param option the name of the option, who's legal values are desired
     * @return a list of legal values for the given option
     */
    virtual std::list<std::string> getGlobalOptionValues(std::string_view option);

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
     * This method is NOT the recommended means for applying a CipherKey
     * to a module.
     *
     * Typically CipherKey entries and other per module user configuration
     * settings are all saved in a separate localConfig.conf that is updated
     * by a UI or other client of the library. e.g.,
     *
     *
     * [KJV]
     * Font=Arial
     * LocalOptionFilter=SomeSpecialFilterMyUIAppliesToTheKJV
     *
     * [ISV]
     * CipherKey=xyzzy
     *
     * [StrongsGreek]
     * SomeUISetting=false
     *
     *
     * Then these extra config settings in this separate file are applied
     * just before module creation by overriding SWMgr::createAllModules and
     * augmenting SWMgr::config with code like this:

     *
     * @code
     * void createAllModules(bool multiMod) {
     *
     *      // after SWMgr::config is loaded
     *      // see if we have our own local settings
     *      SWBuf myExtraConf = "~/.myapp/localConf.conf";
     *      bool exists = FileMgr::existsFile(extraConf);
     *      if (exists) {
     *              SWConfig addConfig(extraConf);
     *              this->config->augment(addConfig);
     *      }
     *
     *      // now that we've augmented SWMgr::config with our own custom
     *      // settings, proceed on with creating modules
     *
     *      SWMgr::createAllModules(multiMod);
     *
     * }
     * @endcode
     *
     * The above convention is preferred to using this setCipherKey method
     *
     * @param modName For this module we change the unlockKey
     * @param key This is the new unlock key we use for the module.
     */
    virtual bool setCipherKey(std::string const & modName,
                              char const * key);
};

} /* namespace swordxx */

#endif /* SWORDXX_SWMGR_H */
