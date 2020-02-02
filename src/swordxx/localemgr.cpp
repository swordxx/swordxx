/******************************************************************************
 *
 *  localemgr.cpp -    implementation of class LocaleMgr used to interact with
 *            registered locales for a Sword++ installation
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

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <sys/stat.h>
#include <utility>
#include <vector>
#include "DirectoryEnumerator.h"
#include "filemgr.h"
#include "swlog.h"
#include "swmgr.h"
#include "swlocale.h"
#include "utilstr.h"


namespace swordxx {

std::shared_ptr<LocaleMgr> LocaleMgr::m_systemLocaleMgr;

std::shared_ptr<LocaleMgr> LocaleMgr::getSystemLocaleMgr() {
    if (!m_systemLocaleMgr)
        setSystemLocaleMgr(std::make_shared<LocaleMgr>());
    return m_systemLocaleMgr;
}

void LocaleMgr::setSystemLocaleMgr(std::shared_ptr<LocaleMgr> newLocaleMgr) {
    m_systemLocaleMgr = std::move(newLocaleMgr);
    auto locale(std::make_shared<SWLocale>(nullptr));
    auto localeName(locale->getName());
    m_systemLocaleMgr->m_locales.emplace(std::move(localeName),
                                         std::move(locale));
}

LocaleMgr::LocaleMgr(char const * const iConfigPath)
    /* Locales will be invalidated if you change the StringMgr. So only use the
       default hardcoded locale and let the frontends change the locale if they
       want: */
    : m_defaultLocaleName(SWLocale::DEFAULT_LOCALE_NAME)
{
    std::string prefixPath;
    std::string configPath;
    char configType = 0;
    std::list<std::string> augPaths;

    if (!iConfigPath) {
        SWLog::getSystemLog()->logDebug("LOOKING UP LOCALE DIRECTORY...");
        SWConfig * sysConf = nullptr;
        configType = SWMgr::findConfig(prefixPath, configPath, &augPaths, &sysConf);
        if (sysConf) {
            auto const & section = (*sysConf)["Install"];
            auto const entry(section.find("LocalePath"));
            if (entry != section.end()) {
                configType = 9;    // our own
                prefixPath = entry->second;
                SWLog::getSystemLog()->logDebug("LocalePath provided in sysConfig.");
            }
        }
        delete sysConf;
        SWLog::getSystemLog()->logDebug("LOOKING UP LOCALE DIRECTORY COMPLETE.");
    } else {
        loadConfigDir(iConfigPath);
    }

    if (!prefixPath.empty()) {
        std::string path;
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
        path += "locales.d";
        if (FileMgr::isReadable(path))
            loadConfigDir(path.c_str());
    }

    // Load locale files from all augmented paths:
    if (augPaths.size() && configType != 9) {
        for (auto const & augPath : augPaths) {
            auto const path(augPath + "/locales.d");
            if (FileMgr::isReadable(path))
                loadConfigDir(path.c_str());
        }
    }
}

LocaleMgr::~LocaleMgr() noexcept {}

void LocaleMgr::loadConfigDir(char const * ipath) {
    SWLog::getSystemLog()->logInformation("LocaleMgr::loadConfigDir loading %s",
                                          ipath);
    if (auto dir = DirectoryEnumerator(ipath)) {
        std::vector<std::string> dirEntries;
        while (auto const ent = dir.readEntry())
            dirEntries.emplace_back(ent);
        std::sort(dirEntries.begin(), dirEntries.end());

        for (auto const & ent : dirEntries) {
            auto locale(std::make_shared<SWLocale>(
                            [&ipath,&ent]{
                                std::string newmodfile(ipath);
                                addTrailingDirectorySlash(newmodfile);
                                newmodfile += ent;
                                return newmodfile;

                            }().c_str()));
            auto const & localeName = locale->getName();
            if (!localeName.empty()) {
                auto const it(m_locales.find(localeName));
                if (it != m_locales.end()) { // already present
                    it->second->augment(*locale);
                } else {
                    m_locales.emplace(localeName, std::move(locale));
                }
            }
        }
    }
}

std::shared_ptr<SWLocale> LocaleMgr::getLocale(std::string_view name) {
    auto const it = m_locales.find(name);
    if (it != m_locales.end())
        return it->second;

    SWLog::getSystemLog()->logWarning("LocaleMgr::getLocale failed to find %s\n",
                                      std::string(name).c_str());
    return m_locales[SWLocale::DEFAULT_LOCALE_NAME];
}

std::list<std::string> LocaleMgr::getAvailableLocales() {
    std::list<std::string> retVal;
    for (auto const & lp: m_locales)
        if (lp.second->getName() != "locales")
            retVal.push_back(lp.second->getName());
    return retVal;
}

std::string LocaleMgr::translate(std::string_view text,
                                 std::string_view localeName)
{
    if (auto target = getLocale(localeName.empty()
                                ? m_defaultLocaleName
                                : localeName))
        return target->translateText(text);
    return std::string(text);
}

void LocaleMgr::setDefaultLocaleName(std::string_view defaultLocaleName) {
    static auto const stripStartingFromChar =
            [](std::string_view & str, char const delim) noexcept {
                auto const i = str.find(delim);
                if (i != std::string::npos)
                    str = str.substr(0, i);
            };
    // discard everything after '.' usually encoding e.g. .UTF-8
    stripStartingFromChar(defaultLocaleName, '.');
    // also discard after '@' so e.g. @euro locales are found
    stripStartingFromChar(defaultLocaleName, '@');

    auto toSet(defaultLocaleName);
    // First check for what we ask for
    if (m_locales.find(defaultLocaleName) == m_locales.end()) {
        // check for locale without country
        stripStartingFromChar(defaultLocaleName, '_');
        if (m_locales.find(defaultLocaleName) != m_locales.end())
            toSet = std::move(defaultLocaleName);
    }
    m_defaultLocaleName = std::move(toSet);
}

} /* namespace swordxx */
