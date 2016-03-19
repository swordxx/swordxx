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

#include <cassert>
#include "untgz.h"

#include <installmgr.h>
#include <filemgr.h>
#include <utilstr.h>

#include <fcntl.h>

#include <swmgr.h>
#include <swmodule.h>
#include <version.h>
#include <swlog.h>
#include <dirent.h>

#include <stdio.h>
#include <map>

#include <curlftpt.h>
#include <curlhttpt.h>

#include <iostream>
#include <regex>


using std::map;


namespace swordxx {

namespace {

    static void removeTrailingSlash(SWBuf &buf) {
        int len = buf.size();
        if ((buf[len-1] == '/')
         || (buf[len-1] == '\\'))
            buf.size(len-1);
    }

    static const char *masterRepoList = "masterRepoList.conf";

}


const int InstallMgr::MODSTAT_OLDER            = 0x001;
const int InstallMgr::MODSTAT_SAMEVERSION      = 0x002;
const int InstallMgr::MODSTAT_UPDATED          = 0x004;
const int InstallMgr::MODSTAT_NEW              = 0x008;
const int InstallMgr::MODSTAT_CIPHERED         = 0x010;
const int InstallMgr::MODSTAT_CIPHERKEYPRESENT = 0x020;


// override this method and provide your own custom RemoteTransport subclass
// here we try a couple defaults if sword was compiled with support for them.
// see these classes for examples of how to make your own
RemoteTransport *InstallMgr::createFTPTransport(const char *host, StatusReporter *statusReporter) {
    return new CURLFTPTransport(host, statusReporter);
}


RemoteTransport *InstallMgr::createHTTPTransport(const char *host, StatusReporter *statusReporter) {
    return new CURLHTTPTransport(host, statusReporter);
}


InstallMgr::InstallMgr(const char *privatePath, StatusReporter *sr, SWBuf u, SWBuf p) {
    userDisclaimerConfirmed = false;
    statusReporter = sr;
    this->u = u;
    this->p = p;
    this->privatePath = 0;
    installConf = 0;
    stdstr(&(this->privatePath), privatePath);
    if (this->privatePath) {
        int len = strlen(this->privatePath);
        if ((this->privatePath[len-1] == '/')
         || (this->privatePath[len-1] == '\\'))
            this->privatePath[len-1] = 0;
    }
    confPath = (SWBuf)privatePath + "/InstallMgr.conf";
    FileMgr::createParent(confPath.c_str());

    readInstallConf();
}


InstallMgr::~InstallMgr() {
    delete [] privatePath;
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

    if (installConf) delete installConf;

    installConf = new SWConfig(confPath.c_str());

    clearSources();

    setFTPPassive(stricmp((*installConf)["General"]["PassiveFTP"].c_str(), "false") != 0);

    SectionMap::iterator confSection = installConf->Sections.find("Sources");
    ConfigEntMap::iterator sourceBegin;
    ConfigEntMap::iterator sourceEnd;

    if (confSection != installConf->Sections.end()) {

        sourceBegin = confSection->second.lower_bound("FTPSource");
        sourceEnd = confSection->second.upper_bound("FTPSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("FTP", sourceBegin->second.c_str());
            sources[is->caption] = is;
            SWBuf parent = (SWBuf)privatePath + "/" + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = (SWBuf)privatePath + "/" + is->uid;
            sourceBegin++;
        }

#ifdef CURLSFTPAVAILABLE
        sourceBegin = confSection->second.lower_bound("SFTPSource");
        sourceEnd   = confSection->second.upper_bound("SFTPSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("SFTP", sourceBegin->second.c_str());
            sources[is->caption] = is;
            SWBuf parent = (SWBuf)privatePath + "/" + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = (SWBuf)privatePath + "/" + is->uid;
            sourceBegin++;
        }
#endif // CURLSFTPAVAILABLE

        sourceBegin = confSection->second.lower_bound("HTTPSource");
        sourceEnd = confSection->second.upper_bound("HTTPSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("HTTP", sourceBegin->second.c_str());
            sources[is->caption] = is;
            SWBuf parent = (SWBuf)privatePath + "/" + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = (SWBuf)privatePath + "/" + is->uid;
            sourceBegin++;
        }

        sourceBegin = confSection->second.lower_bound("HTTPSSource");
        sourceEnd   = confSection->second.upper_bound("HTTPSSource");

        while (sourceBegin != sourceEnd) {
            InstallSource *is = new InstallSource("HTTPS", sourceBegin->second.c_str());
            sources[is->caption] = is;
            SWBuf parent = (SWBuf)privatePath + "/" + is->uid + "/file";
            FileMgr::createParent(parent.c_str());
            is->localShadow = (SWBuf)privatePath + "/" + is->uid;
            sourceBegin++;
        }
    }

    defaultMods.clear();
    confSection = installConf->Sections.find("General");
    if (confSection != installConf->Sections.end()) {
        sourceBegin = confSection->second.lower_bound("DefaultMod");
        sourceEnd = confSection->second.upper_bound("DefaultMod");

        while (sourceBegin != sourceEnd) {
            defaultMods.insert(sourceBegin->second.c_str());
            sourceBegin++;
        }
    }
}


void InstallMgr::saveInstallConf() {

    installConf->Sections["Sources"].clear();

    for (InstallSourceMap::iterator it = sources.begin(); it != sources.end(); ++it) {
        if (it->second) {
            installConf->Sections["Sources"].insert(ConfigEntMap::value_type(it->second->type + "Source", it->second->getConfEnt().c_str()));
        }
    }
    (*installConf)["General"]["PassiveFTP"] = (isFTPPassive()) ? "true" : "false";

    installConf->Save();
}


void InstallMgr::terminate() {
    std::shared_ptr<RemoteTransport> const transportPtr(transport);
    if (transportPtr)
        transportPtr->terminate();
}


int InstallMgr::removeModule(SWMgr *manager, const char *moduleName) {
    SectionMap::iterator module;
    ConfigEntMap::iterator fileBegin;
    ConfigEntMap::iterator fileEnd, entry;

    // save our own copy, cuz when we remove the module from the SWMgr
    // it's likely we'll free the memory passed to us in moduleName
    SWBuf modName = moduleName;
    module = manager->config->Sections.find(modName);

    if (module != manager->config->Sections.end()) {
        // to be sure all files are closed
        // this does not remove the .conf information from SWMgr
        manager->deleteModule(modName);

        fileBegin = module->second.lower_bound("File");
        fileEnd = module->second.upper_bound("File");

        SWBuf modFile;
        SWBuf modDir;
        entry = module->second.find("AbsoluteDataPath");
        modDir = entry->second.c_str();
        removeTrailingSlash(modDir);
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

            if ((dir = opendir(manager->configPath))) {    // find and remove .conf file
                rewinddir(dir);
                while ((ent = readdir(dir))) {
                    if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
                        modFile = manager->configPath;
                        removeTrailingSlash(modFile);
                        modFile += "/";
                        modFile += ent->d_name;
                        SWConfig *config = new SWConfig(modFile.c_str());
                        if (config->Sections.find(modName) != config->Sections.end()) {
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
    RemoteTransport *trans = 0;
    if (is->type == "FTP"
#ifdef CURLSFTPAVAILABLE
        || is->type == "SFTP"
#endif
        ) {

        trans = createFTPTransport(is->source, statusReporter);
        trans->setPassive(passive);
    }
    else if (is->type == "HTTP" || is->type == "HTTPS") {
        trans = createHTTPTransport(is->source, statusReporter);
    }
    transport.reset(trans); // set classwide current transport for other thread terminate() call
    if (is->u.length()) {
        trans->setUser(is->u);
        trans->setPasswd(is->p);
    }
    else {
        trans->setUser(u);
        trans->setPasswd(p);
    }

    SWBuf urlPrefix;
    if (is->type == "HTTP") {
        urlPrefix = (SWBuf) "http://";
    }
    else if (is->type == "HTTPS") {
        urlPrefix = (SWBuf) "https://";
    }
#ifdef CURLSFTPAVAILABLE
    else if (is->type == "SFTP") {
        urlPrefix = (SWBuf) "sftp://";
    }
#endif
    else {
        urlPrefix = (SWBuf) "ftp://";
    }
    urlPrefix.append(is->source);

    // let's be sure we can connect.  This seems to be necessary but sucks
//    SWBuf url = urlPrefix + is->directory.c_str() + "/"; //dont forget the final slash
//    if (trans->getURL("swdirlist.tmp", url.c_str())) {
//         SWLog::getSystemLog()->logDebug("FTPCopy: failed to get dir %s\n", url.c_str());
//         return -1;
//    }


    if (dirTransfer) {
        SWBuf dir = (SWBuf)is->directory.c_str();
        removeTrailingSlash(dir);
        dir += (SWBuf)"/" + src; //dont forget the final slash
SWLog::getSystemLog()->logDebug("remoteCopy: dirTransfer: %s", dir.c_str());

        retVal = trans->copyDirectory(urlPrefix, dir, dest, suffix);


    }
    else {
        try {
            SWBuf url = urlPrefix + is->directory.c_str();
            removeTrailingSlash(url);
            url += (SWBuf)"/" + src; //dont forget the final slash
            if (trans->getURL(dest, url.c_str())) {
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
    SWBuf sourceDir;
    SWBuf buffer;
    bool aborted = false;
    bool cipher = false;
    DIR *dir;
    struct dirent *ent;
    SWBuf modFile;

    SWLog::getSystemLog()->logDebug("***** InstallMgr::installModule\n");
    if (fromLocation)
        SWLog::getSystemLog()->logDebug("***** fromLocation: %s \n", fromLocation);
    SWLog::getSystemLog()->logDebug("***** modName: %s \n", modName);

    if (is)
        sourceDir = (SWBuf)privatePath + "/" + is->uid;
    else    sourceDir = fromLocation;

    removeTrailingSlash(sourceDir);
    sourceDir += '/';

    SWMgr mgr(sourceDir.c_str());

    module = mgr.config->Sections.find(modName);

    if (module != mgr.config->Sections.end()) {

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
                    SWBuf sourcePath = sourceDir;
                    sourcePath += fileBegin->second.c_str();
                    SWBuf dest = destMgr->prefixPath;
                    removeTrailingSlash(dest);
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
                SWBuf absolutePath = entry->second.c_str();
                SWBuf relativePath = absolutePath;
                entry = module->second.find("PrefixPath");
                if (entry != module->second.end()) {
                    relativePath << strlen(entry->second.c_str());
                }
                else {
                    relativePath << strlen(mgr.prefixPath);
                }
                SWLog::getSystemLog()->logDebug("***** mgr.prefixPath: %s \n", mgr.prefixPath);
                SWLog::getSystemLog()->logDebug("***** destMgr->prefixPath: %s \n", destMgr->prefixPath);
                SWLog::getSystemLog()->logDebug("***** absolutePath: %s \n", absolutePath.c_str());
                SWLog::getSystemLog()->logDebug("***** relativePath: %s \n", relativePath.c_str());

                if (is) {
                    if (remoteCopy(is, relativePath.c_str(), absolutePath.c_str(), true)) {
                        aborted = true;    // user aborted
                    }
                }
                if (!aborted) {
                    SWBuf destPath = (SWBuf)destMgr->prefixPath + relativePath;
                    retVal = FileMgr::copyDir(absolutePath.c_str(), destPath.c_str());
                }
                if (is) {        // delete tmp netCopied files
//                    mgr->deleteModule(modName);
                    FileMgr::removeDir(absolutePath.c_str());
                }
            }
        }
        if (!aborted) {
            SWBuf confDir = sourceDir + "mods.d/";
            if ((dir = opendir(confDir.c_str()))) {    // find and copy .conf file
                rewinddir(dir);
                while ((ent = readdir(dir)) && !retVal) {
                    if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
                        modFile = confDir;
                        modFile += ent->d_name;
                        SWConfig *config = new SWConfig(modFile.c_str());
                        if (config->Sections.find(modName) != config->Sections.end()) {
                            SWBuf targetFile = destMgr->configPath; //"./mods.d/";
                            removeTrailingSlash(targetFile);
                            targetFile += "/";
                            targetFile += ent->d_name;
                            retVal = FileMgr::copyFile(modFile.c_str(), targetFile.c_str());
                            if (cipher) {
                                if (getCipherCode(modName, config)) {
                                    SWMgr newDest(destMgr->prefixPath);
                                    removeModule(&newDest, modName);
                                    aborted = true;
                                }
                                else {
                                    config->Save();
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

    SWBuf root = (SWBuf)privatePath + (SWBuf)"/" + is->uid.c_str();
    removeTrailingSlash(root);
    SWBuf target = root + "/mods.d";
    int errorCode = -1; //0 means successful

    FileMgr::removeDir(target.c_str());

    if (!FileMgr::existsDir(target))
        FileMgr::createPathAndFile(target+"/globals.conf");

    SWBuf archive = root + "/mods.d.tar.gz";

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

        // ##(.##)*
        static auto const parseVersionString = [](char const * a,
                                                  uint32_t & triple) noexcept
        {
            unsigned haveTripleDigits = 0u;
            uint32_t outTriple = 0u;
            assert(a);
            enum { NEEDNUM, NUM_DELIM_OR_END, DELIM_OR_END } state = NEEDNUM;
            unsigned lastDigit = 0u;
            for (;; ++a) {
                switch (*a) {
                case '0':
                    if (state == DELIM_OR_END)
                        return false;
                    if (state == NUM_DELIM_OR_END)
                        lastDigit *= 0x10u;
                    state = DELIM_OR_END;
                    break;
                #define D(ch,val) \
                    case ch: \
                        if (state == DELIM_OR_END) \
                            return false; \
                        if (state == NEEDNUM) { \
                            state = NUM_DELIM_OR_END; \
                            lastDigit = (val); \
                        } else { \
                            state = DELIM_OR_END; \
                            lastDigit = (lastDigit * 0x10u) + (val); \
                        } \
                        break;
                D('1',1u) D('2',2u) D('3',3u) D('4',4u)
                D('5',5u) D('6',6u) D('7',7u) D('8',8u) D('9',9u)
                D('a',10u)D('b',11u)D('c',12u)D('d',13u)D('e',14u)D('f',15u)
                D('A',10u)D('B',11u)D('C',12u)D('D',13u)D('E',14u)D('F',15u)
                #undef D
                case '.':
                    if (state == NEEDNUM)
                        return false;
                    if (haveTripleDigits < 3u) {
                        ++haveTripleDigits;
                        outTriple = (outTriple * 0x100u) + lastDigit;
                        lastDigit = 0u;
                    }
                    state = NEEDNUM;
                    break;
                case '\0':
                    if (state != NEEDNUM) {
                        if (haveTripleDigits < 3u) {
                            ++haveTripleDigits;
                            outTriple = (outTriple * 0x100u) + lastDigit;
                        }
                        while (haveTripleDigits < 3u) {
                            ++haveTripleDigits;
                            outTriple *= 0x100u;
                        }
                        triple = outTriple;
                        return true;
                    }
                    return false;
                default: return false;
                }
            }
        };

        static auto const getVersion = [](char const * const str,
                                          uint32_t const defValue) noexcept
        {
            uint32_t parsed;
            if (str && parseVersionString(str, parsed))
                return parsed;
            return defValue;
        };

        /// \todo Use MinimumVersion key:
        #if 0
        uint32_t const softwareVersion =
                getVersion(mod->second->getConfigEntry("MinimumVersion"),
                           SWORDXX_VERSION);
        #endif

        const SWModule *baseMod = base.getModule(mod->first);
        if (baseMod) {
            uint32_t const sourceVersion =
                    getVersion(mod->second->getConfigEntry("Version"), 0x100u);
            uint32_t const targetVersion =
                    getVersion(baseMod->getConfigEntry("Version"), 0x100u);
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

    SWBuf root = (SWBuf)privatePath;
    removeTrailingSlash(root);
    SWBuf masterRepoListPath = root + "/" + masterRepoList;
    InstallSource is("FTP");
    is.source = "ftp.crosswire.org";
    is.directory = "/pub/sword";
    int errorCode = remoteCopy(&is, masterRepoList, masterRepoListPath.c_str(), false);
    if (!errorCode) { //sucessfully downloaded the repo list
        SWConfig masterList(masterRepoListPath);
        SectionMap::iterator sections = masterList.Sections.find("Repos");
        if (sections != masterList.Sections.end()) {
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
                            it->second = 0;
                        }
                        else {
                            SWBuf key = actions->second.stripPrefix('=');
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
                    SWBuf key = actions->second.stripPrefix('=');
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
    mgr = 0;
    userData = 0;
    if (confEnt) {
        SWBuf buf = confEnt;
        caption   = buf.stripPrefix('|', true);
        source    = buf.stripPrefix('|', true);
        directory = buf.stripPrefix('|', true);
        u         = buf.stripPrefix('|', true);
        p         = buf.stripPrefix('|', true);
        uid       = buf.stripPrefix('|', true);

        if (!uid.length()) uid = source;

        removeTrailingSlash(directory);
    }
}


InstallSource::~InstallSource() {
    if (mgr)
        delete mgr;
}


void InstallSource::flush() {
    if (mgr) {
        delete mgr;
        mgr = 0;
    }
}


SWMgr *InstallSource::getMgr() {
    if (!mgr)
        // ..., false = don't augment ~home directory.
        mgr = new SWMgr(localShadow.c_str(), true, 0, false, false);
    return mgr;
}


} /* namespace swordxx */

