/*****************************************************************************
 *
 *  installmgr.cpp -    InstallMgr functions
 *
 * $Id$
 *
 * Copyright 2002-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include "installmgr.h"

#include <atomic>
#include <cassert>
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <regex>
#include <utility>
#include "curlftpt.h"
#include "curlhttpt.h"
#include "DirectoryEnumerator.h"
#include "filemgr.h"
#include "swlog.h"
#include "swmgr.h"
#include "swmodule.h"
#include "untgz.h"
#include "utilstr.h"
#include SWORDXX_VERSION_H_FILE


namespace swordxx {

namespace {

    static const char *masterRepoList = "masterRepoList.conf";

}


const int InstallMgr::MODSTAT_OLDER            = 0x001;
const int InstallMgr::MODSTAT_SAMEVERSION      = 0x002;
const int InstallMgr::MODSTAT_UPDATED          = 0x004;
const int InstallMgr::MODSTAT_NEW              = 0x008;
const int InstallMgr::MODSTAT_CIPHERED         = 0x010;
const int InstallMgr::MODSTAT_CIPHERKEYPRESENT = 0x020;


InstallMgr::InstallMgr(NormalizedPath const & privatePath,
                       std::shared_ptr<StatusReporter> sr,
                       std::string u,
                       std::string p)
{
    m_statusReporter = sr;
    this->m_u = u;
    this->m_p = p;
    m_privatePath = privatePath.str() + '/';
    m_confPath = m_privatePath + "InstallMgr.conf";
    FileMgr::createParent(m_confPath.c_str());

    readInstallConf();
}


InstallMgr::~InstallMgr() {
    installConf.reset();
    clearSources();
}


void InstallMgr::clearSources() { sources.clear(); }


void InstallMgr::readInstallConf() {
    installConf = std::make_unique<SWConfig>(m_confPath);

    clearSources();

    setFTPPassive(
                !caseInsensitiveEquals(
                    (*installConf)["General"]["PassiveFTP"].c_str(),
                    "false"));

    if (auto const confSection = installConf->sections().find("Sources");
        confSection != installConf->sections().end())
    {
        auto const addSources =
                [this](auto & section,
                       char const * const keyName,
                       char const * const sourceTypeName)
                {
                    auto const e(section.second.upper_bound(keyName));
                    for (auto it = section.second.lower_bound(keyName);
                         it != e;
                         ++it)
                    {
                        auto const is(std::make_shared<InstallSource>(
                                          sourceTypeName,
                                          it->second.c_str()));
                        sources[is->m_caption] = is;
                        auto const parent(m_privatePath + is->m_uid + "/file");
                        FileMgr::createParent(parent.c_str());
                        is->m_localShadow = m_privatePath + is->m_uid;
                    }
                };
        addSources(*confSection, "FTPSource", "FTP");
#if SWORDXX_CURL_HAS_SFTP
        addSources(*confSection, "SFTPSource", "SFTP");
#endif // SWORDXX_CURL_HAS_SFTP
        addSources(*confSection, "HTTPSource", "HTTP");
        addSources(*confSection, "HTTPSSource", "HTTPS");
    }

    m_defaultMods.clear();
    if (auto const confSection = installConf->sections().find("General");
        confSection != installConf->sections().end())
    {
        auto const e(confSection->second.upper_bound("DefaultMod"));
        for (auto it = confSection->second.lower_bound("DefaultMod");
             it != e;
             ++it)
            m_defaultMods.insert(it->second.c_str());

    }
}


void InstallMgr::saveInstallConf() {

    installConf->sections()["Sources"].clear();

    for (auto const & sp : sources)
        if (sp.second)
            installConf->sections()["Sources"].emplace(
                        sp.second->m_type + "Source",
                        sp.second->getConfEnt().c_str());

    (*installConf)["General"]["PassiveFTP"] = (isFTPPassive()) ? "true" : "false";

    installConf->save();
}


void InstallMgr::terminate() {
    if (auto const transportPtr =
                std::atomic_load_explicit(&m_transport,
                                          std::memory_order_acquire))
        transportPtr->terminate();
}


bool InstallMgr::removeModule(SWMgr & manager, std::string const & moduleName) {
    if (auto const module = manager.config->sections().find(moduleName);
        module != manager.config->sections().end())
    {
        /* Save our own copy, cuz when we remove the module from the SWMgr it's
           likely we'll free the memory passed to us in moduleName: */
        auto const modName(moduleName);

        /* To be sure all files are closed (this does not remove the .conf
           information from SWMgr): */
        manager.deleteModule(modName.c_str());

        auto fileBegin(module->second.lower_bound("File"));
        auto const fileEnd(module->second.upper_bound("File"));

        NormalizedPath const modDir(
                    module->second.find("AbsoluteDataPath")->second);
        if (fileBegin != fileEnd) { // Remove each file:
            for (; fileBegin != fileEnd; ++fileBegin)
                FileMgr::removeFile(
                            (modDir.str() + '/' + fileBegin->second).c_str());
        } else { // Remove all files in DataPath directory:
            FileMgr::removeDir(modDir.c_str());

            // Find and remove .conf file:
            if (auto dir = DirectoryEnumerator(manager.m_configPath)) {
                while (auto ent = dir.readEntry()) {
                    std::string modFile(manager.m_configPath);
                    removeTrailingDirectorySlashes(modFile);
                    modFile += "/";
                    modFile += ent;
                    SWConfig config(modFile);
                    auto const & sections = config.sections();
                    if (sections.find(modName) != sections.end())
                        FileMgr::removeFile(modFile.c_str());
                }
            }
        }
        return true;
    }
    return false;
}


int InstallMgr::installModule(SWMgr & destMgr,
                              const char * fromLocation,
                              const char * modName,
                              InstallSource * is)
{
    int retVal = 0;
    SectionMap::iterator module, section;
    ConfigEntMap::iterator fileBegin;
    ConfigEntMap::iterator fileEnd;
    ConfigEntMap::iterator entry;
    std::string sourceDir;
    std::string buffer;
    bool aborted = false;
    bool cipher = false;
    std::string modFile;

    SWLog::getSystemLog()->logDebug("***** InstallMgr::installModule\n");
    if (fromLocation)
        SWLog::getSystemLog()->logDebug("***** fromLocation: %s \n", fromLocation);
    SWLog::getSystemLog()->logDebug("***** modName: %s \n", modName);

    if (is)
        sourceDir = m_privatePath + is->m_uid;
    else    sourceDir = fromLocation ? fromLocation : "";

    removeTrailingDirectorySlashes(sourceDir);
    sourceDir += '/';

    SWMgr mgr(sourceDir.c_str());

    module = mgr.config->sections().find(modName);

    if (module != mgr.config->sections().end()) {

        entry = module->second.find("CipherKey");
        if (entry != module->second.end())
            cipher = true;

        //
        // This first check is a method to allow a module to specify each
        // file that needs to be copied
        //
        fileEnd = module->second.upper_bound("File");
        fileBegin = module->second.lower_bound("File");

        if (fileBegin != fileEnd) {    // copy each file
            if (is) {
                while (fileBegin != fileEnd) {    // netCopy each file first
                    buffer = sourceDir + fileBegin->second.c_str();
                    if (remoteCopy(*is, fileBegin->second.c_str(), buffer.c_str())) {
                        aborted = true;
                        break;    // user aborted
                    }
                    fileBegin++;
                }
                fileBegin = module->second.lower_bound("File");
            }

            if (!aborted) {
                // DO THE INSTALL
                while (fileBegin != fileEnd && !retVal) {
                    std::string sourcePath = sourceDir;
                    sourcePath += fileBegin->second.c_str();
                    std::string dest = destMgr.prefixPath();
                    removeTrailingDirectorySlashes(dest);
                    dest += '/';
                    dest += fileBegin->second.c_str();
                    retVal = FileMgr::copyFile(sourcePath.c_str(), dest.c_str());

                    fileBegin++;
                }
            }
            //---------------

            if (is) {
                fileBegin = module->second.lower_bound("File");
                while (fileBegin != fileEnd) {    // delete each tmp netCopied file
                    buffer = sourceDir + fileBegin->second.c_str();
                    FileMgr::removeFile(buffer.c_str());
                    fileBegin++;
                }
            }
        }

        // This is the REAL install code, the above code I don't think has
        // ever been used
        //
        // Copy all files in DataPath directory
        //
        else {
            ConfigEntMap::iterator entry2;

            entry2 = module->second.find("AbsoluteDataPath");
            if (entry2 != module->second.end()) {
                std::string absolutePath(entry2->second);
                std::string relativePath(absolutePath);
                entry2 = module->second.find("PrefixPath");
                if (entry2 != module->second.end()) {
                    relativePath.erase(0u, entry2->second.size());
                }
                else {
                    relativePath.erase(0u, mgr.prefixPath().size());
                }
                SWLog::getSystemLog()->logDebug("***** mgr.prefixPath: %s \n", mgr.prefixPath().c_str());
                SWLog::getSystemLog()->logDebug("***** destMgr->prefixPath: %s \n", destMgr.prefixPath().c_str());
                SWLog::getSystemLog()->logDebug("***** absolutePath: %s \n", absolutePath.c_str());
                SWLog::getSystemLog()->logDebug("***** relativePath: %s \n", relativePath.c_str());

                if (is) {
                    if (remoteCopy(*is, relativePath.c_str(), absolutePath.c_str(), true)) {
                        aborted = true;    // user aborted
                    }
                }
                if (!aborted) {
                    std::string destPath(destMgr.prefixPath() + relativePath);
                    retVal = FileMgr::copyDir(absolutePath.c_str(), destPath.c_str());
                }
                if (is) {        // delete tmp netCopied files
//                    mgr->deleteModule(modName);
                    FileMgr::removeDir(absolutePath.c_str());
                }
            }
        }
        if (!aborted) {
            std::string confDir = sourceDir + "mods.d/";
            if (auto dir = DirectoryEnumerator(confDir)) {    // find and copy .conf file
                while (auto const ent = dir.readEntry()) {
                    if (retVal)
                        break;
                    modFile = confDir;
                    modFile += ent;
                    SWConfig config(modFile);
                    if (config.sections().find(modName) != config.sections().end()) {
                        std::string targetFile = destMgr.m_configPath; //"./mods.d/";
                        removeTrailingDirectorySlashes(targetFile);
                        targetFile += "/";
                        targetFile += ent;
                        retVal = FileMgr::copyFile(modFile.c_str(), targetFile.c_str());
                        if (cipher) {
                            if (getCipherCode(modName, &config)) {
                                SWMgr newDest(destMgr.prefixPath().c_str());
                                removeModule(newDest, modName);
                                aborted = true;
                            }
                            else {
                                config.save();
                                retVal = FileMgr::copyFile(modFile.c_str(), targetFile.c_str());
                            }
                        }
                    }
                }
            }
        }
        return (aborted) ? -9 : retVal;
    }
    return 1;
}


int InstallMgr::refreshRemoteSource(InstallSource & is) {
    NormalizedPath const root(m_privatePath + is.m_uid);
    std::string const target(root.str() + "/mods.d");
    int errorCode = -1; //0 means successful

    FileMgr::removeDir(target.c_str());

    if (!FileMgr::exists(target))
        FileMgr::createPathAndFile((target+"/globals.conf").c_str());

    std::string archive = target + ".tar.gz";

    errorCode = remoteCopy(is, "mods.d.tar.gz", archive.c_str(), false);
    if (!errorCode) { //sucessfully downloaded the tar,gz of module configs
        auto const fd(FileMgr::getSystemFileMgr()->open(archive.c_str(), FileMgr::RDONLY));
        untargz(fd->getFd(), root.c_str());
    }
    else
    errorCode = remoteCopy(is, "mods.d", target.c_str(), true, ".conf"); //copy the whole directory

    is.flush();
    return errorCode;
}


bool InstallMgr::isDefaultModule(const char *modName) {
    return m_defaultMods.count(modName);
}

/************************************************************************
 * getModuleStatus - compare the modules of two SWMgrs and return a
 *     vector describing the status of each.  See MODSTAT_*
 */
std::map<SWModule *, int> InstallMgr::getModuleStatus(const SWMgr &base, const SWMgr &other) {
    std::map<SWModule *, int> retVal;
    bool cipher;
    bool keyPresent;
    int modStat;

    for (auto const & mp : other.modules()) {
        modStat = 0;

        cipher = false;
        keyPresent = false;

        const char *v = mp.second->getConfigEntry("CipherKey");
        if (v) {
            cipher = true;
            keyPresent = *v;
        }

        static auto const getVersion = [](char const * const str,
                                          char const * const defValue) noexcept
        { return isValidSwordVersion(str) ? str : defValue; };

        /// \todo Use MinimumVersion key:
        #if 0
        SwordxxVersionType const softwareVersion =
                getVersion(mod->second->getConfigEntry("MinimumVersion"),
                           SWORDXX_VERSION);
        #endif

        if (auto const baseMod = base.getModule(mp.first.c_str())) {
            auto const sourceVersion(
                    getVersion(mp.second->getConfigEntry("Version"), "1"));
            auto const targetVersion(
                    getVersion(baseMod->getConfigEntry("Version"), "1"));
            int const c = compareVersions(sourceVersion, targetVersion);
            modStat |= ((c == 0)
                        ? MODSTAT_SAMEVERSION
                        : ((c > 0) ? MODSTAT_UPDATED : MODSTAT_OLDER));
        }
        else modStat |= MODSTAT_NEW;

        if (cipher) modStat |= MODSTAT_CIPHERED;
        if (keyPresent) modStat |= MODSTAT_CIPHERKEYPRESENT;
        retVal[mp.second.get()] = modStat;
    }
    return retVal;
}


/************************************************************************
 * refreshRemoteSourceConfiguration - grab master list of know remote
 *     sources and integrate it with our configurations.
 */
int InstallMgr::refreshRemoteSourceConfiguration() {
    std::string const masterRepoListPath(
                NormalizedPath(m_privatePath).str() + "/" + masterRepoList);
    InstallSource is("FTP");
    is.m_source = "ftp.crosswire.org";
    is.m_directory = "/pub/sword";
    int errorCode = remoteCopy(is, masterRepoList, masterRepoListPath.c_str(), false);
    if (!errorCode) { //sucessfully downloaded the repo list
        SWConfig masterList(masterRepoListPath);
        SectionMap::iterator sections = masterList.sections().find("Repos");
        if (sections != masterList.sections().end()) {
            for (auto & ap : sections->second) {
                // Search through our current sources and see if we have a matching UID
                for (auto & sp : sources) {
                    // is this our UID?
                    if (sp.second && sp.second->m_uid == ap.first) {
                        if (ap.second == "REMOVE") {
                            // be sure to call save/reload after this
                            // or this could be dangerous
                            sp.second.reset();
                        }
                        else {
                            std::string key(stripPrefix(ap.second, '='));
                            if (key == "FTPSource") {
                                // we might consider instantiating a temp IS
                                // from our config string and then copy only
                                // some entries.  This would allow the use to
                                // change some fields and not have them overwritten
                                // but it seems like we might want to change any
                                // of the current fields so we don't do this now
                                // InstallSource i("FTP", actions->second);
                                sp.second =
                                        std::make_shared<InstallSource>(
                                            "FTP",
                                            ap.second.c_str());
                                sp.second->m_uid = ap.first;
                            }
                        }
                        goto foundIt;
                    }
                }
                // didn't find our UID, let's add it
                {
                    std::string key(stripPrefix(ap.second, '='));
                    if (key == "FTPSource") {
                        if (ap.second != "REMOVE") {
                            auto is2(std::make_shared<InstallSource>(
                                         "FTP",
                                         ap.second.c_str()));
                            is2->m_uid = ap.first;
                            sources[is2->m_caption] = std::move(is2);
                        }
                    }
                }

                foundIt: (void) 0;
            }

            // persist and re-read
            saveInstallConf();
            readInstallConf();

            return 0;
        }
    }
    return -1;
}

bool InstallMgr::getCipherCode(char const *, SWConfig *) { return false; }


InstallSource::InstallSource(const char *type, const char *confEnt) {
    this->m_type = type;
    if (confEnt) {
        std::string buf = confEnt;
        m_caption   = stripPrefix(buf, '|');
        m_source    = stripPrefix(buf, '|');
        m_directory = stripPrefix(buf, '|');
        m_u         = stripPrefix(buf, '|');
        m_p         = stripPrefix(buf, '|');
        m_uid       = stripPrefix(buf, '|');

        if (!m_uid.length()) m_uid = m_source;

        removeTrailingDirectorySlashes(m_directory);
    }
}


InstallSource::~InstallSource() noexcept = default;


void InstallSource::flush() { m_mgr.reset(); }


std::shared_ptr<SWMgr> const & InstallSource::getMgr() {
    if (!m_mgr)
        m_mgr = std::make_shared<SWMgr>(m_localShadow.c_str(),
                                        true,
                                        nullptr,
                                        false,
                                        false); // augmentHome
    return m_mgr;
}


int InstallMgr::remoteCopy(InstallSource & is,
                           const char * src,
                           const char * dest,
                           bool dirTransfer,
                           const char * suffix)
{
    SWLog::getSystemLog()->logDebug("remoteCopy: %s, %s, %s, %c, %s",
                                    is.m_source.c_str(),
                                    src,
                                    (dest ? dest : "null"),
                                    (dirTransfer ? 't' : 'f'),
                                    (suffix ? suffix : "null"));

    int retVal = 0;
    std::shared_ptr<RemoteTransport> trans;
    if (is.m_type == "FTP"
#if SWORDXX_CURL_HAS_SFTP
        || is.m_type == "SFTP"
#endif
        ) {

        auto t(std::make_shared<CURLFTPTransport>(is.m_source.c_str(),
                                                  m_statusReporter));
        t->setPassive(m_passive);
        trans = std::move(t);
    }
    else if (is.m_type == "HTTP" || is.m_type == "HTTPS") {
        trans = std::make_shared<CURLHTTPTransport>(is.m_source.c_str(),
                                                    m_statusReporter);
    }

    // set classwide current transport for other thread terminate() call:
    std::atomic_store_explicit(&m_transport, trans, std::memory_order_release);

    if (is.m_u.length()) {
        trans->setUser(is.m_u.c_str());
        trans->setPasswd(is.m_p.c_str());
    }
    else {
        trans->setUser(m_u.c_str());
        trans->setPasswd(m_p.c_str());
    }

    std::string urlPrefix;
    if (is.m_type == "HTTP") {
        urlPrefix = "http://";
    }
    else if (is.m_type == "HTTPS") {
        urlPrefix = "https://";
    }
#if SWORDXX_CURL_HAS_SFTP
    else if (is.m_type == "SFTP") {
        urlPrefix = "sftp://";
    }
#endif
    else {
        urlPrefix = "ftp://";
    }
    urlPrefix.append(is.m_source);

    // let's be sure we can connect.  This seems to be necessary but sucks
//    std::string url = urlPrefix + is->directory.c_str() + "/"; //dont forget the final slash
//    if (trans->getURL("swdirlist.tmp", url.c_str())) {
//         SWLog::getSystemLog()->logDebug("FTPCopy: failed to get dir %s\n", url.c_str());
//         return -1;
//    }


    if (dirTransfer) {
        std::string dir(is.m_directory);
        removeTrailingDirectorySlashes(dir);
        (dir += '/') += src; //dont forget the final slash
SWLog::getSystemLog()->logDebug("remoteCopy: dirTransfer: %s", dir.c_str());

        retVal = trans->copyDirectory(urlPrefix.c_str(),
                                      dir.c_str(),
                                      dest,
                                      suffix);


    }
    else {
        try {
            std::string url = urlPrefix + is.m_directory;
            removeTrailingDirectorySlashes(url);
            (url += '/') += src; // Dont forget the final slash
            if (!trans->getUrl(dest, url.c_str())) {
                SWLog::getSystemLog()->logDebug("netCopy: failed to get file %s", url.c_str());
                retVal = -1;
            }
        }
        catch (...) {
            retVal = -1;
        }
    }
    std::atomic_store_explicit(&m_transport,
                               std::shared_ptr<RemoteTransport>(),
                               std::memory_order_release);
    return retVal;
}

} /* namespace swordxx */
