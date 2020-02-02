/******************************************************************************
 *
 *  swmgr.cpp -    used to interact with an install base of Sword++ modules
 *
 * Copyright 1998-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include "swmgr.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#ifndef _MSC_VER
#include <iostream>
#endif
#include <iterator>
#include <sys/stat.h>
#include <tuple>
#include <utility>
#include "DirectoryEnumerator.h"
#include "filemgr.h"
#include "filters/cipherfil.h"
#include "filters/gbffootnotes.h"
#include "filters/gbfheadings.h"
#include "filters/gbfmorph.h"
#include "filters/gbfplain.h"
#include "filters/gbfredletterwords.h"
#include "filters/gbfstrongs.h"
#include "filters/greeklexattribs.h"
#include "filters/osisenum.h"
#include "filters/osisfootnotes.h"
#include "filters/osisglosses.h"
#include "filters/osisheadings.h"
#include "filters/osislemma.h"
#include "filters/osismorph.h"
#include "filters/osismorphsegmentation.h"
#include "filters/osisplain.h"
#include "filters/osisredletterwords.h"
#include "filters/osisreferencelinks.h"
#include "filters/osisscripref.h"
#include "filters/osisstrongs.h"
#include "filters/osisvariants.h"
#include "filters/osisxlit.h"
#include "filters/papyriplain.h"
#include "filters/teiplain.h"
#include "filters/thmlfootnotes.h"
#include "filters/thmlheadings.h"
#include "filters/thmllemma.h"
#include "filters/thmlmorph.h"
#include "filters/thmlplain.h"
#include "filters/thmlscripref.h"
#include "filters/thmlstrongs.h"
#include "filters/thmlvariants.h"
#include "filters/utf8arabicpoints.h"
#include "filters/utf8cantillation.h"
#include "filters/utf8greekaccents.h"
#include "filters/utf8hebrewpoints.h"
#include "modules/comments/hrefcom.h"
#include "modules/comments/rawcom.h"
#include "modules/comments/rawcom4.h"
#include "modules/comments/rawfiles.h"
#include "modules/comments/zcom.h"
#include "modules/comments/zcom4.h"
#include "modules/common/bz2comprs.h"
#include "modules/common/lzsscomprs.h"
#include "modules/common/swcipher.h"
#include "modules/common/xzcomprs.h"
#include "modules/common/zipcomprs.h"
#include "filters/utf8transliterator.h"
#include "modules/genbook/rawgenbook.h"
#include "modules/lexdict/rawld.h"
#include "modules/lexdict/rawld4.h"
#include "modules/lexdict/zld.h"
#include "modules/texts/rawtext.h"
#include "modules/texts/rawtext4.h"
#include "modules/texts/ztext.h"
#include "modules/texts/ztext4.h"
#include "swfiltermgr.h"
#include "swlog.h"
#include "swoptfilter.h"
#include "utilstr.h"


namespace swordxx {
namespace {

void setSystemLogLevel(char const * const logLocation,
                       std::string const & logLevelString)
{
    if (!logLevelString.empty()) {
        auto & logger = *SWLog::getSystemLog();
        SWLog::LogLevel logLevel;
        if (logLevelString == "ERROR") {
            logLevel = SWLog::LOG_ERROR;
        } else if (logLevelString == "WARN") {
            logLevel = SWLog::LOG_WARN;
        } else if (logLevelString == "INFO") {
            logLevel = SWLog::LOG_INFO;
        } else if (logLevelString == "TIMEDINFO") {
            logLevel = SWLog::LOG_TIMEDINFO;
        } else if (logLevelString == "DEBUG") {
            logLevel = SWLog::LOG_DEBUG;
        } else {
            logger.logError("Invalid LogLevel found in %s: LogLevel: %s",
                            logLocation,
                            logLevelString.c_str());
            return;
        }
        logger.setLogLevel(logLevel);
        logger.logInformation("Setting log level from %s to %s",
                              logLocation,
                              logLevelString.c_str());
    }
}

void setSystemLogLevel(std::string const & logLevelString)
{ return setSystemLogLevel("SWORD_LOGLEVEL", logLevelString); }

void setSystemLogLevel(SWConfig const & sysConf) {
    static char const logLocation[] = "[SWORD] section of sword.conf";
    {
        auto const & sections = sysConf.sections();
        auto sectionEntry(sections.find("SWORD"));
        if (sectionEntry != sections.end()) {
            auto const & section = sectionEntry->second;
            auto const entry(section.find("LogLevel"));
            if (entry != section.end())
                return setSystemLogLevel(logLocation, entry->second);
        }
    }
    return setSystemLogLevel(logLocation, std::string());
}

std::string getEnvironmentVariable(char const * const key) {
    auto const value(::getenv(key));
    return value ? value : std::string();
};

} // anonymous namespace

const char *SWMgr::globalConfPath =
        SWORDXX_SYSCONF_INSTALL_PREFIX "/swordxx.conf";

void SWMgr::init() {
    #define ADD_FILTER(mapping,type,...) \
        do { \
            auto filter(std::make_shared<type>()); \
            (mapping).emplace(#type, filter); \
            __VA_ARGS__ \
        } while(false)
    #define ADD_OPTION_FILTER(...) ADD_FILTER(m_optionFilters, __VA_ARGS__)
    #define ADD_EXTRA_FILTER(...) ADD_FILTER(extraFilters, __VA_ARGS__)

    ADD_OPTION_FILTER(ThMLVariants,);
    ADD_OPTION_FILTER(GBFStrongs,);
    ADD_OPTION_FILTER(GBFFootnotes,);
    ADD_OPTION_FILTER(GBFRedLetterWords,);
    ADD_OPTION_FILTER(GBFMorph,);
    ADD_OPTION_FILTER(GBFHeadings,);
    ADD_OPTION_FILTER(OSISHeadings,);
    ADD_OPTION_FILTER(OSISStrongs,);
    ADD_OPTION_FILTER(OSISMorph,);
    ADD_OPTION_FILTER(OSISLemma,);
    ADD_OPTION_FILTER(OSISFootnotes,);
    ADD_OPTION_FILTER(OSISScripref,);
    ADD_OPTION_FILTER(OSISRedLetterWords,);
    ADD_OPTION_FILTER(OSISMorphSegmentation,);
    ADD_OPTION_FILTER(OSISGlosses,
                      m_optionFilters.emplace("OSISRuby", filter););
    ADD_OPTION_FILTER(OSISXlit,);
    ADD_OPTION_FILTER(OSISEnum,);
    ADD_OPTION_FILTER(OSISVariants,);
    ADD_OPTION_FILTER(ThMLStrongs,);
    ADD_OPTION_FILTER(ThMLFootnotes,);
    ADD_OPTION_FILTER(ThMLMorph,);
    ADD_OPTION_FILTER(ThMLHeadings,);
    ADD_OPTION_FILTER(ThMLLemma,);
    ADD_OPTION_FILTER(ThMLScripref,);
    ADD_OPTION_FILTER(UTF8GreekAccents,);
    ADD_OPTION_FILTER(UTF8HebrewPoints,);
    ADD_OPTION_FILTER(UTF8ArabicPoints,);
    ADD_OPTION_FILTER(UTF8Cantillation,);
    ADD_OPTION_FILTER(GreekLexAttribs,);
    ADD_OPTION_FILTER(PapyriPlain,);

// UTF8Transliterator needs to be handled differently because it should always available as an option, for all modules
    ADD_OPTION_FILTER(UTF8Transliterator,
                      transliterator = filter;
                      options.push_back(filter->getOptionName()););

    ADD_EXTRA_FILTER(GBFPlain,  m_gbfplain  = filter;);
    ADD_EXTRA_FILTER(ThMLPlain, m_thmlplain = filter;);
    ADD_EXTRA_FILTER(OSISPlain, m_osisplain = filter;);
    ADD_EXTRA_FILTER(TEIPlain,  m_teiplain  = filter;);

    #undef ADD_EXTRA_FILTER
    #undef ADD_OPTION_FILTER
    #undef ADD_FILTER
}


std::string SWMgr::getHomeDir() {
    // figure out 'home' directory for app data
    std::string homeDir(getEnvironmentVariable("HOME"));
    if (homeDir.empty()) {
        // silly windows
        homeDir = getEnvironmentVariable("APPDATA");
    }
    if (!homeDir.empty())
        addTrailingDirectorySlash(homeDir);

    return homeDir;
}

SWMgr::SWMgr(std::shared_ptr<SWFilterMgr> filterMgr_, bool multiMod)
    : SWMgr(nullptr, nullptr, true, std::move(filterMgr_), multiMod)
{}

SWMgr::SWMgr(SWConfig * iconfig,
             SWConfig * isysconfig,
             bool autoload,
             std::shared_ptr<SWFilterMgr> filterMgr_,
             bool multiMod)
    : mgrModeMultiMod(multiMod)
    , filterMgr(std::move(filterMgr_))
{
    init();

    if (iconfig) {
        config   = iconfig;
        myconfig = nullptr;
    }
    else config = nullptr;
    if (isysconfig) {
        sysConfig   = isysconfig;
        mysysconfig = nullptr;
    }
    else sysConfig = nullptr;

    if (autoload)
        load();
}

SWMgr::SWMgr(char const * iConfigPath,
             bool autoload,
             std::shared_ptr<SWFilterMgr> filterMgr_,
             bool multiMod,
             bool augmentHome_)
    : mgrModeMultiMod(multiMod)
    , augmentHome(augmentHome_)
    , filterMgr(std::move(filterMgr_))
{
    init();

    std::string path;
    if (iConfigPath) {
        path = iConfigPath;
        if (path.empty()) {
            path = '/';
        } else {
            addTrailingDirectorySlash(path);
        }
    } else {
        path = '/';
    }
    if (FileMgr::existsFile(path.c_str(), "mods.conf")) {
        m_prefixPath = path;
        path += "mods.conf";
        m_configPath = path;
    }
    else {
        auto checkPath(path + "mods.d");
        if (FileMgr::isReadable(checkPath)) {
            m_prefixPath = std::move(path);
            m_configPath = std::move(checkPath);
            configType = 1;
        }
    }

    config = nullptr;
    sysConfig = nullptr;

    if (autoload && !m_configPath.empty())
        load();
}

SWMgr::~SWMgr() {
    deleteAllModules();

    delete homeConfig;
    delete mysysconfig;
    delete myconfig;
}


char SWMgr::findConfig(std::string & prefixPath, std::string & configPath, std::list<std::string> *augPaths, SWConfig **providedSysConf) {
    char configType = 0;

    static bool logLevelWasSet = false;
    std::string sysConfPath;

    if (!logLevelWasSet) {
        auto const envLogLevel(getEnvironmentVariable("SWORD_LOGLEVEL"));
        if (!envLogLevel.empty()) {
            setSystemLogLevel(envLogLevel);
            logLevelWasSet = true;
        }
    }

    SWConfig * sysConf = nullptr;
    std::string sysConfDataPath;

    auto const homeDir(getHomeDir());

    // check for a sysConf passed in to us
    SWLog::getSystemLog()->logDebug("Checking for provided SWConfig(\"swordxx.conf\")...");
    if (providedSysConf && *providedSysConf) {
        sysConf = *providedSysConf;
        SWLog::getSystemLog()->logDebug("found.");
        if (!logLevelWasSet) { setSystemLogLevel(*sysConf); logLevelWasSet = true; }
    }

    // if we haven't been given our datapath in a sysconf, we need to track it down
    if (!sysConf) {
        // check working directory
        SWLog::getSystemLog()->logDebug("Checking working directory for swordxx.conf...");
        {
            auto checkPath("./swordxx.conf");
            if (FileMgr::isReadable(checkPath)) {
                SWLog::getSystemLog()->logDebug("Overriding any systemwide or ~/.swordxx/ swordxx.conf with one found in current directory.");
                sysConfPath = std::move(checkPath);
                sysConf = new SWConfig(sysConfPath);
                {
                    auto const & section = sysConf->sections()["Install"];
                    auto const entry(section.find("DataPath"));
                    if (entry != section.end())
                        sysConfDataPath = entry->second;
                }
                if (!logLevelWasSet) { setSystemLogLevel(*sysConf); logLevelWasSet = true; }
                if (providedSysConf) {
                    *providedSysConf = sysConf;
                }
                else {
                    delete sysConf;
                    sysConf = nullptr;
                }
            }
        }
        if (sysConfDataPath.empty()) {
            SWLog::getSystemLog()->logDebug("Checking working directory for mods.conf...");
            {
                auto checkPath("./mods.conf");
                if (FileMgr::isReadable(checkPath)) {
                    SWLog::getSystemLog()->logDebug("found.");
                    prefixPath = "./";
                    configPath = std::move(checkPath);
                    return configType;
                }
            }

            SWLog::getSystemLog()->logDebug("Checking working directory for mods.d...");
            {
                std::string checkPath("./mods.d");
                if (FileMgr::isReadable(checkPath)) {
                    SWLog::getSystemLog()->logDebug("found.");
                    prefixPath = "./";
                    configPath = std::move(checkPath);
                    return 1;
                }
            }

            // check working directory ../library/
            SWLog::getSystemLog()->logDebug("Checking working directory ../library/ for mods.d...");
            {
                std::string checkPath("../library/mods.d");
                if (FileMgr::isReadable(checkPath)) {
                    SWLog::getSystemLog()->logDebug("found.");
                    prefixPath = "../library/";
                    configPath = std::move(checkPath);
                    return 1;
                }
            }

            // check environment variable SWORDXX_PATH
            SWLog::getSystemLog()->logDebug("Checking $SWORDXX_PATH...");
            {
                std::string path(getEnvironmentVariable("SWORDXX_PATH"));
                if (!path.empty()) {

                    SWLog::getSystemLog()->logDebug("found (%s).", path.c_str());
                    addTrailingDirectorySlash(path);

                    SWLog::getSystemLog()->logDebug("Checking $SWORDXX_PATH for mods.conf...");
                    {
                        auto checkPath(path + "mods.conf");
                        if (FileMgr::isReadable(checkPath)) {
                            SWLog::getSystemLog()->logDebug("found.");
                            prefixPath = std::move(path);
                            configPath = std::move(checkPath);
                            return configType;
                        }
                    }

                    SWLog::getSystemLog()->logDebug("Checking $SWORDXX_PATH for mods.d...");
                    {
                        auto checkPath(path + "mods.d");
                        if (FileMgr::isReadable(checkPath)) {
                            SWLog::getSystemLog()->logDebug("found.");
                            prefixPath = std::move(path);
                            configPath = std::move(checkPath);
                            return 1;
                        }
                    }
                }
            }


            // check for systemwide globalConfPath

            SWLog::getSystemLog()->logDebug("Parsing %s...", globalConfPath);
            {
                std::string globPaths(globalConfPath);
                for (std::string gfp = stripPrefix(globPaths, ':');
                     !gfp.empty();
                     gfp = stripPrefix(globPaths, ':'))
                {
                    SWLog::getSystemLog()->logDebug("Checking for %s...",
                                                    gfp.c_str());
                    if (FileMgr::isReadable(gfp)) {
                        SWLog::getSystemLog()->logDebug("found.");
                        sysConfPath = std::move(gfp);
                        break;
                    }
                }
            }

            if (!homeDir.empty()) {
                std::string checkPath(homeDir + ".swordxx/swordxx.conf");
                if (FileMgr::isReadable(checkPath)) {
                    SWLog::getSystemLog()->logDebug("Overriding any systemwide swordxx.conf with one found in users home directory (%s)", checkPath.c_str());
                    sysConfPath = std::move(checkPath);
                } else {
                    std::string checkPath2(homeDir + "swordxx/swordxx.conf");
                    if (FileMgr::isReadable(checkPath2)) {
                        SWLog::getSystemLog()->logDebug("Overriding any systemwide swordxx.conf with one found in users home directory (%s)", checkPath2.c_str());
                        sysConfPath = std::move(checkPath2);
                    }
                }
            }
        }
    }

    if (!sysConf && !sysConfPath.empty()) {
        sysConf = new SWConfig(sysConfPath);
    }

    if (sysConf) {
        if (!logLevelWasSet) { setSystemLogLevel(*sysConf); logLevelWasSet = true; }
        {
            auto const & section = sysConf->sections()["Install"];
            auto const entry(section.find("DataPath"));
            if (entry != section.end())
                sysConfDataPath = entry->second;
        }
        if (!sysConfDataPath.empty()) {
            addTrailingDirectorySlash(sysConfDataPath);

            auto path(sysConfDataPath);
            SWLog::getSystemLog()->logDebug("DataPath in %s is set to %s.", sysConfPath.c_str(), path.c_str());
            SWLog::getSystemLog()->logDebug("Checking for mods.conf in DataPath...");

            if (FileMgr::isReadable(path + "mods.conf")) {
                SWLog::getSystemLog()->logDebug("found.");
                prefixPath = path;
                path += "mods.conf";
                configPath = path;
                configType = 1;
            } else {
                SWLog::getSystemLog()->logDebug("Checking for mods.d in DataPath...");
                auto checkPath(path + "mods.d");
                if (FileMgr::isReadable(checkPath)) {
                    SWLog::getSystemLog()->logDebug("found.");
                    prefixPath = std::move(path);
                    configPath = std::move(checkPath);
                    configType = 1;
                }
            }
        }
    }

    // do some extra processing of sysConf if we have one
    if (sysConf) {
        if (!logLevelWasSet) { setSystemLogLevel(*sysConf); logLevelWasSet = true; }
        if (augPaths) {
            augPaths->clear();
            auto const & section = sysConf->sections()["Install"];
            auto entry(section.lower_bound("AugmentPath"));
            auto const lastEntry(section.upper_bound("AugmentPath"));
            for (; entry != lastEntry; ++entry) {
                auto path(entry->second);
                addTrailingDirectorySlash(path);
                augPaths->emplace_back(std::move(path));
            }
        }
        if (providedSysConf) {
            *providedSysConf = sysConf;
        }
        else delete sysConf;
    }

    if (configType)
        return configType;

    // WE STILL HAVEN'T FOUND A CONFIGURATION.  LET'S LOOK IN SOME OS SPECIFIC
    // LOCATIONS
    //
    // for various flavors of windoze...
    // check %ALLUSERSPROFILE%/Application Data/swordxx/

    SWLog::getSystemLog()->logDebug("Checking $ALLUSERSPROFILE/Application Data/swordxx/...");

    {
        std::string path(getEnvironmentVariable("ALLUSERSPROFILE"));
        if (!path.empty()) {
            SWLog::getSystemLog()->logDebug("found (%s).", path.c_str());
            addTrailingDirectorySlash(path);

            path += "Application Data/swordxx/";
            SWLog::getSystemLog()->logDebug("Checking %s for mods.d...", path.c_str());
            auto checkPath(path + "mods.d");
            if (FileMgr::isReadable(checkPath)) {
                SWLog::getSystemLog()->logDebug("found.");
                prefixPath = std::move(path);
                configPath = std::move(checkPath);
                return 1;
            }
        }
    }

    // for Mac OSX...
    // check $HOME/Library/Application Support/SwordXX/

    SWLog::getSystemLog()->logDebug("Checking $HOME/Library/Application Support/SwordXX/...");

    if (!homeDir.empty()) {
        SWLog::getSystemLog()->logDebug("found (%s).", homeDir.c_str());
        auto path(homeDir);
        addTrailingDirectorySlash(path);

        SWLog::getSystemLog()->logDebug("Checking %s for mods.d...", path.c_str());
        {
            auto checkPath(path + "mods.d");
            if (FileMgr::isReadable(checkPath)) {
                SWLog::getSystemLog()->logDebug("found.");
                prefixPath = std::move(path);
                configPath = std::move(checkPath);
                return 1;
            }
        }
    }

    // FINALLY CHECK PERSONAL HOME DIRECTORY LOCATIONS
    // check ~/.swordxx/

    SWLog::getSystemLog()->logDebug("Checking home directory for ~/.swordxx...");

    if (!homeDir.empty()) {
        auto path(homeDir);
        path += ".swordxx/";
        SWLog::getSystemLog()->logDebug("  Checking for %smods.conf...", path.c_str());
        {
            auto checkPath(path + "mods.conf");
            if (FileMgr::isReadable(checkPath)) {
                SWLog::getSystemLog()->logDebug("found.");
                prefixPath = std::move(path);
                configPath = std::move(checkPath);
                return configType;
            }
        }

        SWLog::getSystemLog()->logDebug("  Checking for %smods.d...", path.c_str());
        {
            auto checkPath(path + "mods.d");
            if (FileMgr::isReadable(checkPath)) {
                SWLog::getSystemLog()->logDebug("found.");
                prefixPath = std::move(path);
                configPath = std::move(checkPath);
                return 2;
            }
        }

        path = homeDir + "swordxx/";
        SWLog::getSystemLog()->logDebug("  Checking for %smods.d...", path.c_str());
        {
            auto checkPath(path + "mods.d");
            if (FileMgr::isReadable(checkPath)) {
                SWLog::getSystemLog()->logDebug("found.");
                prefixPath = std::move(path);
                configPath = std::move(checkPath);
                return 2;
            }
        }
    }
    return configType;
}


void SWMgr::loadConfigDir(const char *ipath)
{
    using namespace std::literals::string_view_literals;

    assert(ipath);
    std::string newmodfile;

    if (auto dir = DirectoryEnumerator(ipath)) {
        while (auto const ent = dir.readEntry()) {
            //check whether it ends with .conf, if it doesn't skip it!
            if (!endsWith(ent, ".conf"sv))
                continue;

            newmodfile = ipath;
            addTrailingDirectorySlash(newmodfile);
            newmodfile += ent;
            if (config)
                config->augment(SWConfig(newmodfile));
            else    config = myconfig = new SWConfig(newmodfile);
        }
        dir.close();

        if (!config) {    // if no .conf file exist yet, create a default
            newmodfile = ipath;
            addTrailingDirectorySlash(newmodfile);
            newmodfile += "globals.conf";
            config = myconfig = new SWConfig(newmodfile);
        }
    }
}


void SWMgr::augmentModules(NormalizedPath const & path, bool multiMod) {
    if (FileMgr::existsDir(path, "mods.d")) {
        SWConfig * saveConfig = nullptr;
        std::string savePrefixPath(m_prefixPath);
        m_prefixPath = path;
        std::string saveConfigPath(m_configPath);
        m_configPath = path.str() + "/mods.d";
        saveConfig = config;
        config = myconfig = nullptr;
        loadConfigDir(m_configPath.c_str());

        if (multiMod) {
            // fix config's Section names to rename modules which are available more than once
            // find out which sections are in both config objects
            // inserting all configs first is not good because that overwrites old keys and new modules would share the same config
            for (auto it = config->sections().cbegin();
                 it != config->sections().cend();)
            {
                if (saveConfig->sections().find( (*it).first ) != saveConfig->sections().end()) { //if the new section is already present rename it
                    ConfigEntMap entMap((*it).second);

                    std::string name;
                    int i = 1;
                    do { //module name already used?
                        name = formatted("%s_%d", (*it).first, i);
                        i++;
                    } while (config->sections().find(name) != config->sections().end());

                    config->sections().emplace(std::move(name),
                                               std::move(entMap));
                    auto const toErase = it++;
                    config->sections().erase(toErase);
                }
                else ++it;
            }
        }

        createAllModules();

        m_prefixPath = std::move(savePrefixPath);
        m_configPath = std::move(saveConfigPath);

        saveConfig->augment(*config);

        homeConfig = myconfig;
        config = myconfig = saveConfig;
    }
}


/***********************************************************************
 * SWMgr::load - loads actual modules
 *
 * RET: status - 0 = ok; -1 no config found; 1 = no modules installed
 *
 */

signed char SWMgr::load() {
    signed char ret = 0;

    if (!config) {    // If we weren't passed a config object at construction, find a config file
        if (m_configPath.empty()) {    // If we weren't passed a config path at construction...
            SWLog::getSystemLog()->logDebug("LOOKING UP MODULE CONFIGURATION...");
            SWConfig *externalSysConf = sysConfig;    // if we have a sysConf before findConfig, then we were provided one from an external source.
            configType = findConfig(m_prefixPath, m_configPath, &augPaths, &sysConfig);
            if (!externalSysConf) mysysconfig = sysConfig;    // remind us to delete our own sysConfig in d-tor
            SWLog::getSystemLog()->logDebug("LOOKING UP MODULE CONFIGURATION COMPLETE.");
        }
        if (!m_configPath.empty()) {
            if (configType)
                loadConfigDir(m_configPath.c_str());
            else    config = myconfig = new SWConfig(m_configPath);
        }
    }

    if (config) {
        SectionMap::iterator Sectloop, Sectend;
        ConfigEntMap::iterator Entryloop, Entryend;

        deleteAllModules();

        for (Sectloop = config->sections().lower_bound("Globals"), Sectend = config->sections().upper_bound("Globals"); Sectloop != Sectend; Sectloop++) {        // scan thru all 'Globals' sections
            for (Entryloop = (*Sectloop).second.lower_bound("AutoInstall"), Entryend = (*Sectloop).second.upper_bound("AutoInstall"); Entryloop != Entryend; Entryloop++)    // scan thru all AutoInstall entries
                InstallScan((*Entryloop).second.c_str());        // Scan AutoInstall entry directory for new modules and install
        }
        if (configType) {    // force reload on config object because we may have installed new modules
            delete myconfig;
            config = myconfig = nullptr;
            loadConfigDir(m_configPath.c_str());
        }
        else    config->reload();

        createAllModules();

        for (auto const & augPath : augPaths)
            augmentModules(augPath.c_str(), mgrModeMultiMod);
        if (augmentHome) {
            // augment config with ~/.swordxx/mods.d if it exists ---------------------
            std::string homeDir = getHomeDir();
            if (!homeDir.empty() && configType != 2) { // 2 = user only
                std::string path = homeDir;
                path += ".swordxx/";
                augmentModules(path.c_str(), mgrModeMultiMod);
                path = homeDir;
                path += "swordxx/";
                augmentModules(path.c_str(), mgrModeMultiMod);
            }
        }
// -------------------------------------------------------------------------
        if (m_modules.empty()) // config exists, but no modules
            ret = 1;

    }
    else {
        SWLog::getSystemLog()->logError("SWMgr: Can't find 'mods.conf' or 'mods.d'.  Try setting:\n\tSWORDXX_PATH=<directory containing mods.conf>\n\tOr see the README file for a full description of setup options (%s)", !m_configPath.empty() ? m_configPath.c_str() : "<configPath is empty>");
        ret = -1;
    }

    return ret;
}


std::shared_ptr<SWModule> SWMgr::createModule(std::string const & name,
                                              std::string const & driver,
                                              ConfigEntMap section)
{
    using namespace std::literals::string_view_literals;

    auto const getEntry =
            [&section](char const * const key,
                       std::string defValue = std::string())
            {
                auto const it(section.find(key));
                return (it != section.end()) ? it->second : defValue;
            };

    auto datapath(m_prefixPath);
    addTrailingDirectorySlash(datapath);
    auto description(getEntry("Description"));
    auto v11n(VersificationMgr::systemVersificationMgr()->getVersificationSystem(
                  getEntry("Versification", "KJV").c_str()));
    auto lang(getEntry("Lang", "en"));

    // DataPath - relative path to data used by module driver.  May be a directory, may be a File.
    //   Typically not useful by outside world.  See AbsoluteDataPath, PrefixPath, and RelativePrefixPath
    //   below.
    {
        auto const dataPathEntry(getEntry("DataPath"));
        auto const * buf2 = dataPathEntry.c_str();
    //    for (; ((*buf2) && ((*buf2 == '.') || (*buf2 == '/') || (*buf2 == '\\'))); buf2++);
        for (; ((*buf2) && ((*buf2 == '/') || (*buf2 == '\\'))); buf2++);
        if (startsWith(buf2, "./"sv)) { //remove the leading ./ in the module data path to make it look better
            buf2 += 2;
        }
        // PrefixPath - absolute directory path to the repository in which this module was found
        section["PrefixPath"] = datapath;
        if (*buf2)
            datapath += buf2;
    }

    section["AbsoluteDataPath"] = datapath;

    auto const markup(
                [getEntry]() {
                    auto const sourceformat(getEntry("SourceType"));
                    if (caseInsensitiveEquals(sourceformat, "ThML")) {
                        return FMT_THML;
                    } else if (caseInsensitiveEquals(sourceformat, "OSIS")) {
                        return FMT_OSIS;
                    } else if (caseInsensitiveEquals(sourceformat, "TEI")) {
                        return FMT_TEI;
                    } else {
                        return FMT_GBF;
                    }
                }());

    auto const enc(
                [getEntry]() {
                    auto const encoding(getEntry("Encoding"));
                    if (caseInsensitiveEquals(encoding, "UTF-8")) {
                        return ENC_UTF8;
                    } else if (caseInsensitiveEquals(encoding, "SCSU")) {
                        return ENC_SCSU;
                    } else if (caseInsensitiveEquals(encoding, "UTF-16")) {
                        return ENC_UTF16;
                    } else {
                        return ENC_LATIN1;
                    }
                }());

    auto const direction(
                [getEntry]() {
                    auto const dirEntry(getEntry("Direction"));
                    if (caseInsensitiveEquals(dirEntry, "rtol")) {
                        return DIRECTION_RTL;
                    } else if (caseInsensitiveEquals(dirEntry, "bidi")) {
                        return DIRECTION_BIDI;
                    } else {
                        return DIRECTION_LTR;
                    }
                }());

    auto const getCompress =
            [getEntry]() -> std::unique_ptr<SWCompress> {
                auto const compressTypeEntry(getEntry("CompressType", "LZSS"));
                if (caseInsensitiveEquals(compressTypeEntry, "ZIP")) {
                    return std::make_unique<ZipCompress>();
                } else if (caseInsensitiveEquals(compressTypeEntry, "BZIP2")) {
                    return std::make_unique<Bzip2Compress>();
                } else if (caseInsensitiveEquals(compressTypeEntry, "XZ")) {
                    return std::make_unique<XzCompress>();
                } else if (caseInsensitiveEquals(compressTypeEntry, "LZSS")) {
                    return  std::make_unique<LZSSCompress>();
                }
                return nullptr;
            };

    auto const stripAbsolutePathLastComponent =
            [&section]() {
                section["AbsoluteDataPath"] =
                        NormalizedPath(section["AbsoluteDataPath"])
                                .getParentDirectory();
            };

    auto const finalizeModule =
            [&section](std::shared_ptr<SWModule> newModule) {
                // if a specific module type is set in the config, use this
                {
                    auto const it(section.find("Type"));
                    if ((it != section.end()) && !it->second.empty())
                        newModule->setType(it->second);
                }

                newModule->setConfig(std::move(section));
                return newModule;
            };

    if (caseInsensitiveEquals(driver, "zText")
        || caseInsensitiveEquals(driver, "zCom")
        || caseInsensitiveEquals(driver, "zText4")
        || caseInsensitiveEquals(driver, "zCom4"))
    {
        auto const blockType(
                [getEntry]() {
                    auto const blockTypeEntry(getEntry("BlockType", "CHAPTER"));
                    if (caseInsensitiveEquals(blockTypeEntry, "VERSE")) {
                        return VERSEBLOCKS;
                    } else if (caseInsensitiveEquals(blockTypeEntry, "BOOK")) {
                        return BOOKBLOCKS;
                    } else {
                        return CHAPTERBLOCKS;
                    }
                }());
        if (auto compress = getCompress()) {
            if (caseInsensitiveEquals(driver, "zText")) {
                return finalizeModule(std::make_shared<zText>(datapath.c_str(), name.c_str(), description.c_str(), blockType, std::move(compress), enc, direction, markup, lang.c_str(), std::move(v11n)));
            } else if (caseInsensitiveEquals(driver, "zText4")) {
                return finalizeModule(std::make_shared<zText4>(datapath.c_str(), name.c_str(), description.c_str(), blockType, std::move(compress), enc, direction, markup, lang.c_str(), std::move(v11n)));
            } else if (caseInsensitiveEquals(driver, "zCom4")) {
                return finalizeModule(std::make_shared<zCom4>(datapath.c_str(), name.c_str(), description.c_str(), blockType, std::move(compress), enc, direction, markup, lang.c_str(), std::move(v11n)));
            } else {
                return finalizeModule(std::make_shared<zCom>(datapath.c_str(), name.c_str(), description.c_str(), blockType, std::move(compress), enc, direction, markup, lang.c_str(), std::move(v11n)));
            }
        }
    } else if (caseInsensitiveEquals(driver, "RawText")) {
        return finalizeModule(std::make_shared<RawText>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str(), std::move(v11n)));
    } else if (caseInsensitiveEquals(driver, "RawText4")) {
        return finalizeModule(std::make_shared<RawText4>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str(), std::move(v11n)));
    } else if (caseInsensitiveEquals(driver, "RawGBF")) { // backward support old drivers
        return finalizeModule(std::make_shared<RawText>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str()));
    } else if (caseInsensitiveEquals(driver, "RawCom")) {
        return finalizeModule(std::make_shared<RawCom>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str(), std::move(v11n)));
    } else if (caseInsensitiveEquals(driver, "RawCom4")) {
        return finalizeModule(std::make_shared<RawCom4>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str(), std::move(v11n)));
    } else if (caseInsensitiveEquals(driver, "RawFiles")) {
        return finalizeModule(std::make_shared<RawFiles>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str()));
    } else if (caseInsensitiveEquals(driver, "HREFCom")) {
        auto const prefixEntry(getEntry("Prefix"));
        return finalizeModule(std::make_shared<HREFCom>(datapath.c_str(), prefixEntry.c_str(), name.c_str(), description.c_str()));
    } else if (caseInsensitiveEquals(driver, "RawLD")) {
        auto const caseSensitive(getEntry("CaseSensitiveKeys") == "true");
        auto const strongsPadding(getEntry("StrongsPadding", "true") == "true");
        stripAbsolutePathLastComponent();
        return finalizeModule(std::make_shared<RawLD>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str(), caseSensitive, strongsPadding));
    } else if (caseInsensitiveEquals(driver, "RawLD4")) {
        auto const caseSensitive(getEntry("CaseSensitiveKeys") == "true");
        auto const strongsPadding(getEntry("StrongsPadding", "true") == "true");
        stripAbsolutePathLastComponent();
        return finalizeModule(std::make_shared<RawLD4>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str(), caseSensitive, strongsPadding));
    } else if (caseInsensitiveEquals(driver, "zLD")) {
        auto const caseSensitive(getEntry("CaseSensitiveKeys") == "true");
        auto const strongsPadding(getEntry("StrongsPadding", "true") == "true");
        auto const blockCountEntry(getEntry("BlockCount", "200"));
        auto blockCount(std::atoi(blockCountEntry.c_str()));
        if (!blockCount)
            blockCount = 200;
        if (auto compress = getCompress()) {
            stripAbsolutePathLastComponent();
            return finalizeModule(std::make_shared<zLD>(datapath.c_str(), name.c_str(), description.c_str(), blockCount, std::move(compress), enc, direction, markup, lang.c_str(), caseSensitive, strongsPadding));
        }
    } else if (caseInsensitiveEquals(driver, "RawGenBook")) {
        auto const keyTypeEntry(getEntry("KeyType", "TreeKey"));
        return finalizeModule(std::make_shared<RawGenBook>(datapath.c_str(), name.c_str(), description.c_str(), enc, direction, markup, lang.c_str(), keyTypeEntry.c_str()));
    }

    return nullptr;
}

void SWMgr::addGlobalOptionFilters(SWModule & module,
                                   ConfigEntMap const & section)
{
    using namespace std::literals::string_view_literals;

    auto start(section.lower_bound("GlobalOptionFilter"));
    auto const end(section.upper_bound("GlobalOptionFilter"));
    for (; start != end; ++start) {
        std::string filterName(start->second);
        decltype(m_optionFilters)::const_iterator it;

        // special cases for filters with parameters
        if (startsWith(filterName, "OSISReferenceLinks"sv)) {
            std::string params = filterName;
            filterName = stripPrefix(params, '|');
            std::string const optionName = stripPrefix(params, '|');
            std::string const optionTip = stripPrefix(params, '|');
            std::string const optionType = stripPrefix(params, '|');
            std::string const optionSubType = stripPrefix(params, '|');
            std::string const optionDefaultValue = stripPrefix(params, '|');
            // we'll key off of type and subtype.
            filterName = filterName + "." + optionType + "." + optionSubType;

            it = m_optionFilters.find(filterName);
            if (it == m_optionFilters.end()) {
                auto filter(std::make_shared<OSISReferenceLinks>(
                                optionName,
                                optionTip,
                                optionType,
                                optionSubType,
                                optionDefaultValue));
                std::tie(it, std::ignore) =
                        m_optionFilters.emplace(filterName, std::move(filter));
            }
        } else {
            it = m_optionFilters.find(filterName);
            if (it == m_optionFilters.end())
                continue;
        }

        // Add filter to module and option as a valid option:
        module.addOptionFilter(it->second);
        if (std::find(options.cbegin(),
                      options.cend(),
                      it->second->getOptionName()) == options.cend())
            options.push_back(it->second->getOptionName());
    }

    if (filterMgr)
        /// \bug Iterator start already invalidated?
        filterMgr->addGlobalOptions(module, section, start, end);
    module.addOptionFilter(transliterator);
}


char SWMgr::filterText(const char *filterName, std::string &text, const SWKey *key, const SWModule *module)
{
    char retVal = -1;
    // why didn't we use find here?
    for (auto const & ofp : m_optionFilters) {
        if (caseInsensitiveEquals(filterName, ofp.second->getOptionName())) {
            retVal = ofp.second->processText(text, key, module);
            break;
        }
    }

    if (retVal == -1) {
        FilterMap::iterator it = extraFilters.find(filterName);
        if (it != extraFilters.end()) {
            retVal = it->second->processText(text, key, module);
        }
    }

    return retVal;
}


void SWMgr::addLocalOptionFilters(SWModule & module,
                                  ConfigEntMap const & section)
{
    auto start(section.lower_bound("LocalOptionFilter"));
    auto const end(section.upper_bound("LocalOptionFilter"));
    for (; start != end; ++start) {
        decltype(m_optionFilters)::iterator const it(
                    m_optionFilters.find(start->second));
        if (it != m_optionFilters.end())
            module.addOptionFilter(it->second);
    }

    if (filterMgr)
        /// \bug Iterator start already invalidated?
        filterMgr->addLocalOptions(module, section, start, end);
}


// manually specified StripFilters for special cases, like Papyri marks and such
void SWMgr::addLocalStripFilters(SWModule & module,
                                 ConfigEntMap const & section)
{
    auto start(section.lower_bound("LocalStripFilter"));
    auto const end(section.upper_bound("LocalStripFilter"));
    for (; start != end; ++start) {
        decltype(m_optionFilters)::const_iterator const it(
                    m_optionFilters.find(start->second));
        if (it != m_optionFilters.end())
            module.addStripFilter(it->second);
    }

    /// \bug Shouldn't we call filterMgr here? (iterators already invalidated)
}


void SWMgr::addRawFilters(SWModule & module, ConfigEntMap const & section) {
    ConfigEntMap::const_iterator const entry(section.find("CipherKey"));
    std::string cipherKey(
                (entry != section.end()) ? entry->second : std::string());
    if (!cipherKey.empty()) {
        auto cipherFilter(std::make_shared<CipherFilter>(cipherKey.c_str()));
        m_cipherFilters.emplace(module.getName(), cipherFilter);
        module.addRawFilter(std::move(cipherFilter));
    }

    if (filterMgr)
        filterMgr->addRawFilters(module, section);
}


void SWMgr::addEncodingFilters(SWModule & module,
                               ConfigEntMap const & section)
{
    if (filterMgr)
        filterMgr->addEncodingFilters(module, section);
}


void SWMgr::addRenderFilters(SWModule & module, ConfigEntMap const & section) {
    if (filterMgr)
        filterMgr->addRenderFilters(module, section);
}


void SWMgr::addStripFilters(SWModule & module, ConfigEntMap const & section) {
    ConfigEntMap::const_iterator entry(section.find("SourceType"));
    std::string sourceformat(
                (entry != section.end()) ? entry->second : std::string());
    // Temporary: To support old module types
    if (sourceformat.empty()) {
        entry = section.find("ModDrv");
        sourceformat = (entry != section.end()) ? entry->second : std::string();
        if (caseInsensitiveEquals(sourceformat, "RawGBF"))
            sourceformat = "GBF";
        else sourceformat = "";
    }

    if (caseInsensitiveEquals(sourceformat, "GBF")) {
        module.addStripFilter(m_gbfplain);
    } else if (caseInsensitiveEquals(sourceformat, "ThML")) {
        module.addStripFilter(m_thmlplain);
    } else if (caseInsensitiveEquals(sourceformat, "OSIS")) {
        module.addStripFilter(m_osisplain);
    } else if (caseInsensitiveEquals(sourceformat, "TEI")) {
        module.addStripFilter(m_teiplain);
    }

    if (filterMgr)
        filterMgr->addStripFilters(module, section);
}


void SWMgr::createAllModules() {
    ConfigEntMap::iterator entry;
    std::string driver;
    for (auto & sp : config->sections()) {
        ConfigEntMap & section = sp.second;

        driver = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : std::string();
        if (!driver.empty()) {
            if (auto newmod = createModule(sp.first, driver, section)) {
                // Filters to add for this module and globally announce as an option to the user
                // e.g. translit, strongs, redletterwords, etc, so users can turn these on and off globally
                addGlobalOptionFilters(*newmod, section);

                // Only add the option to the module, don't announce it's availability
                // These are useful for like: filters that parse special entryAttribs in a text
                // or whatever you might want to happen on entry lookup
                addLocalOptionFilters(*newmod, section);

                //STRIP FILTERS

                // add all basic strip filters for the modtype
                addStripFilters(*newmod, section);

                // Any module-specific processing specified in module config as
                // entries LocalStripFilter= e.g. for papyri, removed all []().
                // notation
                addLocalStripFilters(*newmod, section);

                addRawFilters(*newmod, section);
                addRenderFilters(*newmod, section);
                addEncodingFilters(*newmod, section);

                // place our module in module container, removing first if one
                // already exists by our same name:
                m_modules[newmod->getName()] = std::move(newmod);
            }
        }
    }
}


void SWMgr::deleteAllModules() { m_modules.clear(); }


void SWMgr::deleteModule(const char *modName) { m_modules.erase(modName); }


void SWMgr::InstallScan(const char *dirname)
{
   std::string newmodfile;
   std::string targetName;

    if (FileMgr::existsDir(dirname)) {
        if (auto dir = DirectoryEnumerator(dirname)) {
            std::shared_ptr<FileDesc> conffd;
            auto const fileMgr(FileMgr::getSystemFileMgr());
            while (auto const ent = dir.readEntry()) {
                newmodfile = dirname;
                addTrailingDirectorySlash(newmodfile);
                newmodfile += ent;

                // mods.d
                if (configType) {
                    conffd.reset();
                    targetName = m_configPath;
                    addTrailingDirectorySlash(targetName);
                    targetName += ent;
                    conffd = fileMgr->open(targetName.c_str(), FileMgr::WRONLY|FileMgr::CREAT, FileMgr::IREAD|FileMgr::IWRITE);
                }

                // mods.conf
                else {
                    if (!conffd) {
                        conffd = fileMgr->open(config->filename().c_str(), FileMgr::WRONLY|FileMgr::APPEND);
                        if (conffd && conffd->getFd() >= 0)
                            conffd->seek(0L, SEEK_END);
                        else {
                            conffd.reset();
                        }
                    }
                }
                AddModToConfig(*conffd, newmodfile.c_str());
                FileMgr::removeFile(newmodfile.c_str());
            }
            dir.close();
        }
    }
}


char SWMgr::AddModToConfig(FileDesc & conffd, const char *fname)
{
    char ch;

    SWLog::getSystemLog()->logTimedInformation("Found new module [%s]. Installing...", fname);
    auto const modfd(FileMgr::getSystemFileMgr()->open(fname, FileMgr::RDONLY));
    ch = '\n';
    conffd.write(&ch, 1);
    while (modfd->read(&ch, 1) == 1)
        conffd.write(&ch, 1);
    ch = '\n';
    conffd.write(&ch, 1);
    return 0;
}


void SWMgr::setGlobalOption(std::string_view option,
                            std::string_view value)
{
    for (auto const & ofp : m_optionFilters)
        if (caseInsensitiveEquals(option, ofp.second->getOptionName()))
            ofp.second->setOptionValue(value);
}


const char * SWMgr::getGlobalOption(std::string_view option) {
    for (auto const & ofp : m_optionFilters)
        if (caseInsensitiveEquals(option, ofp.second->getOptionName()))
            return ofp.second->getSelectedOptionValue().c_str();
    return nullptr;
}


const char * SWMgr::getGlobalOptionTip(std::string_view option) {
    for (auto const & ofp : m_optionFilters)
        if (caseInsensitiveEquals(option, ofp.second->getOptionName()))
            return ofp.second->getOptionTip().c_str();
    return nullptr;
}


std::list<std::string> SWMgr::getGlobalOptions() { return options; }


std::list<std::string> SWMgr::getGlobalOptionValues(std::string_view option) {
    /* Just find the first one. All option filters with the same option name
       should expect the same values. */
    for (auto const & ofp : m_optionFilters) {
        if (caseInsensitiveEquals(option, ofp.second->getOptionName())) {
            auto const & vals = ofp.second->getOptionValues();
            return {vals.begin(), vals.end()};
        }
    }
    return std::list<std::string>();
}


bool SWMgr::setCipherKey(std::string const & modName, char const * key) {
    { // check for filter that already exists
        auto const it = m_cipherFilters.find(modName);
        if (it != m_cipherFilters.end()) {
            it->second->getCipher().setCipherKey(key);
            return true;
        }
    }

    // check if module exists
    auto const it = m_modules.find(modName);
    if (it == m_modules.end())
        return false;

    auto cipherFilter(std::make_shared<CipherFilter>(key));
    m_cipherFilters.emplace(modName, cipherFilter);
    it->second->addRawFilter(std::move(cipherFilter));
    return true;
}


} /* namespace swordxx */

