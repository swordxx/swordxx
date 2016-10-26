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

#include <cassert>
#include <cstdio>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <map>
#include <regex>
#include "curlftpt.h"
#include "curlhttpt.h"
#include "filemgr.h"
#include "swlog.h"
#include "swmgr.h"
#include "swmodule.h"
#include "untgz.h"
#include "utilstr.h"
#include SWORDXX_VERSION_H_FILE


using std::map;

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


InstallMgr::InstallMgr(std::string privatePath, StatusReporter *sr, std::string u, std::string p) {
    userDisclaimerConfirmed = false;
    statusReporter = sr;
    this->u = u;
    this->p = p;
    removeTrailingDirectorySlashes(privatePath);
    this->privatePath = privatePath;
    installConf = nullptr;
    confPath = std::move(privatePath += "/InstallMgr.conf");
    FileMgr::createParent(confPath.c_str());

    readInstallConf();
}


InstallMgr::~InstallMgr() {
    delete installConf;
    clearSources();
}


void InstallMgr::clearSources() {
    for (InstallSourceMap::iterator it = sources.begin(); it != sources.end(); ++it) {
        delete it->second;
    }
    sources.clear();
}


void InstallMgr::readInstallConf() {

    delete installConf;

    installConf = new SWConfig(confPath.c_str());

    clearSources();

    setFTPPassive(stricmp((*installConf)["General"]["PassiveFTP"].c_str(), "false") != 0);

    SectionMap::iterator confSection = installConf->sections().find("Sources");
    ConfigEntMap::iterator sourceBegin;
    ConfigEntMap::iterator sourceEnd;

    if (confSection != installConf->sections().end()) {

        sourceBegin = confSection->second.lower_bound("FTPSource");
        sourceEnd = confSection->second.upper_bound("FTPSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("FTP", sourceBegin->second.c_str());
            sources[is->caption] = is;
            std::string parent = privatePath + '/' + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = privatePath + '/' + is->uid;
            sourceBegin++;
        }

#if SWORDXX_CURL_HAS_SFTP
        sourceBegin = confSection->second.lower_bound("SFTPSource");
        sourceEnd   = confSection->second.upper_bound("SFTPSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("SFTP", sourceBegin->second.c_str());
            sources[is->caption] = is;
            std::string parent = privatePath + '/' + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = privatePath + '/' + is->uid;
            sourceBegin++;
        }
#endif // SWORDXX_CURL_HAS_SFTP

        sourceBegin = confSection->second.lower_bound("HTTPSource");
        sourceEnd = confSection->second.upper_bound("HTTPSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("HTTP", sourceBegin->second.c_str());
            sources[is->caption] = is;
            std::string parent = privatePath + '/' + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = privatePath + '/' + is->uid;
            sourceBegin++;
        }

        sourceBegin = confSection->second.lower_bound("HTTPSSource");
        sourceEnd   = confSection->second.upper_bound("HTTPSSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("HTTPS", sourceBegin->second.c_str());
            sources[is->caption] = is;
            std::string parent = privatePath + '/' + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = privatePath + '/' + is->uid;
            sourceBegin++;
        }
    }

    defaultMods.clear();
    confSection = installConf->sections().find("General");
    if (confSection != installConf->sections().end()) {
        sourceBegin = confSection->second.lower_bound("DefaultMod");
        sourceEnd = confSection->second.upper_bound("DefaultMod");

        while (sourceBegin != sourceEnd) {
            defaultMods.insert(sourceBegin->second.c_str());
            sourceBegin++;
        }
    }
}


void InstallMgr::saveInstallConf() {

    installConf->sections()["Sources"].clear();

    for (InstallSourceMap::iterator it = sources.begin(); it != sources.end(); ++it) {
        if (it->second) {
            installConf->sections()["Sources"].insert(ConfigEntMap::value_type(it->second->type + "Source", it->second->getConfEnt().c_str()));
        }
    }
    (*installConf)["General"]["PassiveFTP"] = (isFTPPassive()) ? "true" : "false";

    installConf->save();
}


void InstallMgr::terminate() {
    std::shared_ptr<RemoteTransport> const transportPtr(transport);
    if (transportPtr)
        transportPtr->terminate();
}


int InstallMgr::removeModule(SWMgr *manager, const char *moduleName) {
    assert(manager);
    assert(moduleName);
    SectionMap::iterator module;
    ConfigEntMap::iterator fileBegin;
    ConfigEntMap::iterator fileEnd, entry;

    // save our own copy, cuz when we remove the module from the SWMgr
    // it's likely we'll free the memory passed to us in moduleName
    std::string modName = moduleName;
    module = manager->config->sections().find(modName);

    if (module != manager->config->sections().end()) {
        // to be sure all files are closed
        // this does not remove the .conf information from SWMgr
        manager->deleteModule(modName.c_str());

        fileBegin = module->second.lower_bound("File");
        fileEnd = module->second.upper_bound("File");

        std::string modFile;
        std::string modDir;
        entry = module->second.find("AbsoluteDataPath");
        modDir = entry->second.c_str();
        removeTrailingDirectorySlashes(modDir);
        if (fileBegin != fileEnd) {    // remove each file
            while (fileBegin != fileEnd) {
                modFile = modDir;
                modFile += "/";
                modFile += fileBegin->second.c_str();
                //remove file
                FileMgr::removeFile(modFile.c_str());
                fileBegin++;
            }
        }
        else {    //remove all files in DataPath directory

            DIR *dir;
            struct dirent *ent;
            ConfigEntMap::iterator entry;

            FileMgr::removeDir(modDir.c_str());

            if ((dir = opendir(manager->m_configPath.c_str()))) {    // find and remove .conf file
                rewinddir(dir);
                while ((ent = readdir(dir))) {
                    if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
                        modFile = manager->m_configPath;
                        removeTrailingDirectorySlashes(modFile);
                        modFile += "/";
                        modFile += ent->d_name;
                        SWConfig *config = new SWConfig(modFile.c_str());
                        if (config->sections().find(modName) != config->sections().end()) {
                            delete config;
                            FileMgr::removeFile(modFile.c_str());
                        }
                        else    delete config;
                    }
                }
                closedir(dir);
            }
        }
        return 0;
    }
    return 1;
}


// TODO: rename to netCopy
int InstallMgr::remoteCopy(InstallSource *is, const char *src, const char *dest, bool dirTransfer, const char *suffix) {
SWLog::getSystemLog()->logDebug("remoteCopy: %s, %s, %s, %c, %s", (is?is->source.c_str():"null"), src, (dest?dest:"null"), (dirTransfer?'t':'f'), (suffix?suffix:"null"));

    // assert user disclaimer has been confirmed
    if (!isUserDisclaimerConfirmed()) return -1;

    int retVal = 0;
    RemoteTransport * trans = nullptr;
    if (is->type == "FTP"
#if SWORDXX_CURL_HAS_SFTP
        || is->type == "SFTP"
#endif
        ) {

        auto * const t =
                new CURLFTPTransport(is->source.c_str(), statusReporter);
        t->setPassive(passive);
        trans = t;
    }
    else if (is->type == "HTTP" || is->type == "HTTPS") {
        trans = new CURLHTTPTransport(is->source.c_str(), statusReporter);
    }
    transport.reset(trans); // set classwide current transport for other thread terminate() call
    if (is->u.length()) {
        trans->setUser(is->u.c_str());
        trans->setPasswd(is->p.c_str());
    }
    else {
        trans->setUser(u.c_str());
        trans->setPasswd(p.c_str());
    }

    std::string urlPrefix;
    if (is->type == "HTTP") {
        urlPrefix = "http://";
    }
    else if (is->type == "HTTPS") {
        urlPrefix = "https://";
    }
#if SWORDXX_CURL_HAS_SFTP
    else if (is->type == "SFTP") {
        urlPrefix = "sftp://";
    }
#endif
    else {
        urlPrefix = "ftp://";
    }
    urlPrefix.append(is->source);

    // let's be sure we can connect.  This seems to be necessary but sucks
//    std::string url = urlPrefix + is->directory.c_str() + "/"; //dont forget the final slash
//    if (trans->getURL("swdirlist.tmp", url.c_str())) {
//         SWLog::getSystemLog()->logDebug("FTPCopy: failed to get dir %s\n", url.c_str());
//         return -1;
//    }


    if (dirTransfer) {
        std::string dir(is->directory);
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
            std::string url = urlPrefix + is->directory;
            removeTrailingDirectorySlashes(url);
            (url += '/') + src; //dont forget the final slash
            if (!trans->getUrl(dest, url.c_str())) {
                SWLog::getSystemLog()->logDebug("netCopy: failed to get file %s", url.c_str());
                retVal = -1;
            }
        }
        catch (...) {
            retVal = -1;
        }
    }
    transport.reset();
    return retVal;
}


int InstallMgr::installModule(SWMgr *destMgr, const char *fromLocation, const char *modName, InstallSource *is) {
    int retVal = 0;
    SectionMap::iterator module, section;
    ConfigEntMap::iterator fileBegin;
    ConfigEntMap::iterator fileEnd;
    ConfigEntMap::iterator entry;
    std::string sourceDir;
    std::string buffer;
    bool aborted = false;
    bool cipher = false;
    DIR *dir;
    struct dirent *ent;
    std::string modFile;

    SWLog::getSystemLog()->logDebug("***** InstallMgr::installModule\n");
    if (fromLocation)
        SWLog::getSystemLog()->logDebug("***** fromLocation: %s \n", fromLocation);
    SWLog::getSystemLog()->logDebug("***** modName: %s \n", modName);

    if (is)
        sourceDir = privatePath + '/' + is->uid;
    else    sourceDir = fromLocation;

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
                    if (remoteCopy(is, fileBegin->second.c_str(), buffer.c_str())) {
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
                    std::string dest = destMgr->m_prefixPath;
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
            ConfigEntMap::iterator entry;

            entry = module->second.find("AbsoluteDataPath");
            if (entry != module->second.end()) {
                std::string absolutePath(entry->second);
                std::string relativePath(absolutePath);
                entry = module->second.find("PrefixPath");
                if (entry != module->second.end()) {
                    relativePath.erase(0u, entry->second.size());
                }
                else {
                    relativePath.erase(0u, mgr.m_prefixPath.size());
                }
                SWLog::getSystemLog()->logDebug("***** mgr.prefixPath: %s \n", mgr.m_prefixPath.c_str());
                SWLog::getSystemLog()->logDebug("***** destMgr->prefixPath: %s \n", destMgr->m_prefixPath.c_str());
                SWLog::getSystemLog()->logDebug("***** absolutePath: %s \n", absolutePath.c_str());
                SWLog::getSystemLog()->logDebug("***** relativePath: %s \n", relativePath.c_str());

                if (is) {
                    if (remoteCopy(is, relativePath.c_str(), absolutePath.c_str(), true)) {
                        aborted = true;    // user aborted
                    }
                }
                if (!aborted) {
                    std::string destPath(std::string(destMgr->m_prefixPath) += relativePath);
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
            if ((dir = opendir(confDir.c_str()))) {    // find and copy .conf file
                rewinddir(dir);
                while ((ent = readdir(dir)) && !retVal) {
                    if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
                        modFile = confDir;
                        modFile += ent->d_name;
                        SWConfig *config = new SWConfig(modFile.c_str());
                        if (config->sections().find(modName) != config->sections().end()) {
                            std::string targetFile = destMgr->m_configPath; //"./mods.d/";
                            removeTrailingDirectorySlashes(targetFile);
                            targetFile += "/";
                            targetFile += ent->d_name;
                            retVal = FileMgr::copyFile(modFile.c_str(), targetFile.c_str());
                            if (cipher) {
                                if (getCipherCode(modName, config)) {
                                    SWMgr newDest(destMgr->m_prefixPath.c_str());
                                    removeModule(&newDest, modName);
                                    aborted = true;
                                }
                                else {
                                    config->save();
                                    retVal = FileMgr::copyFile(modFile.c_str(), targetFile.c_str());
                                }
                            }
                        }
                        delete config;
                    }
                }
                closedir(dir);
            }
        }
        return (aborted) ? -9 : retVal;
    }
    return 1;
}


int InstallMgr::refreshRemoteSource(InstallSource *is) {

    // assert user disclaimer has been confirmed
    if (!isUserDisclaimerConfirmed()) return -1;

    std::string root(privatePath + '/' + is->uid);
    removeTrailingDirectorySlashes(root);
    std::string target = root + "/mods.d";
    int errorCode = -1; //0 means successful

    FileMgr::removeDir(target.c_str());

    if (!FileMgr::existsDir(target.c_str()))
        FileMgr::createPathAndFile((target+"/globals.conf").c_str());

    std::string archive = root + "/mods.d.tar.gz";

    errorCode = remoteCopy(is, "mods.d.tar.gz", archive.c_str(), false);
    if (!errorCode) { //sucessfully downloaded the tar,gz of module configs
        FileDesc *fd = FileMgr::getSystemFileMgr()->open(archive.c_str(), FileMgr::RDONLY);
        untargz(fd->getFd(), root.c_str());
        FileMgr::getSystemFileMgr()->close(fd);
    }
    else
    errorCode = remoteCopy(is, "mods.d", target.c_str(), true, ".conf"); //copy the whole directory

    is->flush();
    return errorCode;
}


bool InstallMgr::isDefaultModule(const char *modName) {
    return defaultMods.count(modName);
}

/************************************************************************
 * getModuleStatus - compare the modules of two SWMgrs and return a
 *     vector describing the status of each.  See MODSTAT_*
 */
map<SWModule *, int> InstallMgr::getModuleStatus(const SWMgr &base, const SWMgr &other) {
    map<SWModule *, int> retVal;
    bool cipher;
    bool keyPresent;
    int modStat;

    for (ModMap::const_iterator mod = other.Modules.begin(); mod != other.Modules.end(); mod++) {

        modStat = 0;

        cipher = false;
        keyPresent = false;

        const char *v = mod->second->getConfigEntry("CipherKey");
        if (v) {
            cipher = true;
            keyPresent = *v;
        }

        static auto const getVersion = [](char const * const str,
                                          Version defValue) noexcept
        {
            if (str) {
                auto r(parseVersion(str));
                if (r.first)
                    return r.second;
            }
            return defValue;
        };

        /// \todo Use MinimumVersion key:
        #if 0
        SwordxxVersionType const softwareVersion =
                getVersion(mod->second->getConfigEntry("MinimumVersion"),
                           SWORDXX_VERSION);
        #endif

        const SWModule *baseMod = base.getModule(mod->first.c_str());
        if (baseMod) {
            auto const sourceVersion(
                    getVersion(mod->second->getConfigEntry("Version"), 0x1u));
            auto const targetVersion(
                    getVersion(baseMod->getConfigEntry("Version"), 0x1u));
            modStat |=
                    (sourceVersion > targetVersion)
                     ? MODSTAT_UPDATED
                     : (sourceVersion < targetVersion)
                       ? MODSTAT_OLDER
                       : MODSTAT_SAMEVERSION;
        }
        else modStat |= MODSTAT_NEW;

        if (cipher) modStat |= MODSTAT_CIPHERED;
        if (keyPresent) modStat |= MODSTAT_CIPHERKEYPRESENT;
        retVal[mod->second] = modStat;
    }
    return retVal;
}


/************************************************************************
 * refreshRemoteSourceConfiguration - grab master list of know remote
 *     sources and integrate it with our configurations.
 */
int InstallMgr::refreshRemoteSourceConfiguration() {

    // assert user disclaimer has been confirmed
    if (!isUserDisclaimerConfirmed()) return -1;

    std::string root(privatePath);
    removeTrailingDirectorySlashes(root);
    std::string masterRepoListPath = root + "/" + masterRepoList;
    InstallSource is("FTP");
    is.source = "ftp.crosswire.org";
    is.directory = "/pub/sword";
    int errorCode = remoteCopy(&is, masterRepoList, masterRepoListPath.c_str(), false);
    if (!errorCode) { //sucessfully downloaded the repo list
        SWConfig masterList(masterRepoListPath.c_str());
        SectionMap::iterator sections = masterList.sections().find("Repos");
        if (sections != masterList.sections().end()) {
            for (ConfigEntMap::iterator actions = sections->second.begin(); actions != sections->second.end(); actions++) {
                // Search through our current sources and see if we have a matching UID
                InstallSourceMap::iterator it;
                for (it = sources.begin(); it != sources.end(); ++it) {
                    // is this our UID?
                    if ((it->second) && (it->second->uid == actions->first)) {
                        if (actions->second == "REMOVE") {
                            // be sure to call save/reload after this
                            // or this could be dangerous
                            delete it->second;
                            it->second = nullptr;
                        }
                        else {
                            std::string key(stripPrefix(actions->second, '='));
                            if (key == "FTPSource") {
                                // we might consider instantiating a temp IS
                                // from our config string and then copy only
                                // some entries.  This would allow the use to
                                // change some fields and not have them overwritten
                                // but it seems like we might want to change any
                                // of the current fields so we don't do this now
                                // InstallSource i("FTP", actions->second);
                                delete it->second;
                                it->second = new InstallSource("FTP", actions->second.c_str());
                                it->second->uid = actions->first;
                            }
                        }
                        break;
                    }
                }
                // didn't find our UID, let's add it
                if (it == sources.end()) {
                    std::string key(stripPrefix(actions->second, '='));
                    if (key == "FTPSource") {
                        if (actions->second != "REMOVE") {
                            InstallSource *is = new InstallSource("FTP", actions->second.c_str());
                            is->uid = actions->first;
                            sources[is->caption] = is;
                        }
                    }
                }
            }

            // persist and re-read
            saveInstallConf();
            readInstallConf();

            return 0;
        }
    }
    return -1;
}


InstallSource::InstallSource(const char *type, const char *confEnt) {
    this->type = type;
    mgr = nullptr;
    userData = nullptr;
    if (confEnt) {
        std::string buf = confEnt;
        caption   = stripPrefix(buf, '|');
        source    = stripPrefix(buf, '|');
        directory = stripPrefix(buf, '|');
        u         = stripPrefix(buf, '|');
        p         = stripPrefix(buf, '|');
        uid       = stripPrefix(buf, '|');

        if (!uid.length()) uid = source;

        removeTrailingDirectorySlashes(directory);
    }
}


InstallSource::~InstallSource() { delete mgr; }


void InstallSource::flush() {
    delete mgr;
    mgr = nullptr;
}


SWMgr *InstallSource::getMgr() {
    if (!mgr)
        // ..., false = don't augment ~home directory.
        mgr = new SWMgr(localShadow.c_str(), true, nullptr, false, false);
    return mgr;
}


} /* namespace swordxx */

