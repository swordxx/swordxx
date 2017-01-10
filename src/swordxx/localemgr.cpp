/******************************************************************************
 *
 *  localemgr.cpp -    implementation of class LocaleMgr used to interact with
 *            registered locales for a Sword++ installation
 *
 * $Id$
 *
 * Copyright 2000-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "localemgr.h"

#include <cstdio>
#include <cstdlib>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "filemgr.h"
#include "stringmgr.h"
#include "swlog.h"
#include "swmgr.h"
#include "swlocale.h"
#include "utilstr.h"


namespace swordxx {


std::unique_ptr<LocaleMgr> LocaleMgr::systemLocaleMgr;

LocaleMgr *LocaleMgr::getSystemLocaleMgr() {
    if (!systemLocaleMgr)
        setSystemLocaleMgr(new LocaleMgr());
    return systemLocaleMgr.get();
}

void LocaleMgr::setSystemLocaleMgr(LocaleMgr *newLocaleMgr) {
    systemLocaleMgr.reset(newLocaleMgr);
    auto locale(std::make_shared<SWLocale>(nullptr));
    auto localeName(locale->getName());
    systemLocaleMgr->m_locales.emplace(std::move(localeName),
                                       std::move(locale));
}


LocaleMgr::LocaleMgr(const char *iConfigPath) {
    std::string prefixPath;
    std::string configPath;
    SWConfig * sysConf = nullptr;
    char configType = 0;
    std::string path;
    std::list<std::string> augPaths;
    ConfigEntMap::iterator entry;

    defaultLocaleName = nullptr;

    if (!iConfigPath) {
        SWLog::getSystemLog()->logDebug("LOOKING UP LOCALE DIRECTORY...");
        SWMgr::findConfig(&configType, prefixPath, configPath, &augPaths, &sysConf);
        if (sysConf) {
            if ((entry = (*sysConf)["Install"].find("LocalePath")) != (*sysConf)["Install"].end()) {
                configType = 9;    // our own
                prefixPath = entry->second;
                SWLog::getSystemLog()->logDebug("LocalePath provided in sysConfig.");
            }
        }
        SWLog::getSystemLog()->logDebug("LOOKING UP LOCALE DIRECTORY COMPLETE.");
    }
    else {
        loadConfigDir(iConfigPath);
    }

    if (!prefixPath.empty()) {
        switch (configType) {
        case 2: {
            std::size_t i = configPath.size() - 1u;
            while (i > 0u && (configPath[i] != '/') && (configPath[i] != '\\'))
                --i;
            path = configPath.substr(0u, i);
            removeTrailingDirectorySlashes(path);
            path += "/";
            break;
        }
        default:
            path = prefixPath;
            addTrailingDirectorySlash(path);
            break;
        }
        if (FileMgr::existsDir(path.c_str(), "locales.d")) {
            path += "locales.d";
            loadConfigDir(path.c_str());
        }
    }

    if (augPaths.size() && configType != 9) { //load locale files from all augmented paths
        std::list<std::string>::iterator it = augPaths.begin();
        std::list<std::string>::iterator end = augPaths.end();

        for (;it != end; ++it) {
            if (FileMgr::existsDir((*it).c_str(), "locales.d")) {
                std::string path = (*it) + "locales.d";
                loadConfigDir(path.c_str());
            }
        }
    }

    // Locales will be invalidated if you change the StringMgr
    // So only use the default hardcoded locale and let the
    // frontends change the locale if they want
    stdstr(&defaultLocaleName, SWLocale::DEFAULT_LOCALE_NAME);

    delete sysConf;
}


LocaleMgr::~LocaleMgr() {
    delete[] defaultLocaleName;
    deleteLocales();
}


void LocaleMgr::loadConfigDir(const char *ipath) {
    DIR *dir;
    struct dirent *ent;
    std::string newmodfile;
    LocaleMap::iterator it;
    SWLog::getSystemLog()->logInformation("LocaleMgr::loadConfigDir loading %s", ipath);

    if ((dir = opendir(ipath))) {
        rewinddir(dir);
        while ((ent = readdir(dir))) {
            if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
                newmodfile = ipath;
                if ((ipath[strlen(ipath)-1] != '\\') && (ipath[strlen(ipath)-1] != '/'))
                    newmodfile += "/";
                newmodfile += ent->d_name;
                auto locale(std::make_shared<SWLocale>(newmodfile.c_str()));
                auto localeName(locale->getName());
                static_assert(!std::is_reference<decltype(localeName)>::value, "");
                if (!localeName.empty()) {
                    bool supported = false;
                    auto const & localeEncoding = locale->getEncoding();
                    if (StringMgr::hasUTF8Support()) {
                        supported = (localeEncoding == "UTF-8" || localeEncoding == "ASCII");
                    }
                    else {
                        supported = localeEncoding.empty() || localeEncoding != "UTF-8"; //exclude UTF-8 locales
                    }

                    if (!supported)
                        continue;

                    it = m_locales.find(localeName);
                    if (it != m_locales.end()) { // already present
                        *((*it).second) += *locale;
                    } else {
                        m_locales.emplace(std::move(localeName),
                                          std::move(locale));
                    }
                }
            }
        }
        closedir(dir);
    }
}


void LocaleMgr::deleteLocales() { m_locales.clear(); }


SWLocale *LocaleMgr::getLocale(const char *name) {
    LocaleMap::iterator it;

    it = m_locales.find(name);
    if (it != m_locales.end())
        return (*it).second.get();

    SWLog::getSystemLog()->logWarning("LocaleMgr::getLocale failed to find %s\n", name);
    return m_locales[SWLocale::DEFAULT_LOCALE_NAME].get();
}


std::list <std::string> LocaleMgr::getAvailableLocales() {
    std::list <std::string> retVal;
    for (auto const & lp: m_locales)
        if (lp.second->getName() != "locales")
            retVal.push_back(lp.second->getName());
    return retVal;
}


const char *LocaleMgr::translate(const char *text, const char *localeName) {
    if (!localeName)
        localeName = getDefaultLocaleName();
    if (auto target = getLocale(localeName))
        return target->translate(text).c_str();
    return text;
}


const char *LocaleMgr::getDefaultLocaleName() {
    return defaultLocaleName;
}


void LocaleMgr::setDefaultLocaleName(const char *name) {
    char * tmplang = nullptr;
    stdstr(&tmplang, name);
    // discard everything after '.' usually encoding e.g. .UTF-8
    strtok(tmplang, ".");
    // also discard after '@' so e.g. @euro locales are found
    strtok(tmplang, "@");

    stdstr(&defaultLocaleName, tmplang);

    // First check for what we ask for
    if (m_locales.find(tmplang) == m_locales.end()) {
        // check for locale without country
        char * nocntry = nullptr;
        stdstr(&nocntry, tmplang);
        strtok(nocntry, "_");
        if (m_locales.find(nocntry) != m_locales.end()) {
            stdstr(&defaultLocaleName, nocntry);
        }
        delete [] nocntry;
    }
    delete [] tmplang;
}


} /* namespace swordxx */

