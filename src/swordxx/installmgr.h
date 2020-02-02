/******************************************************************************
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

#ifndef SWORDXX_INSTALLMGR_H
#define SWORDXX_INSTALLMGR_H

#include <map>
#include <memory>
#include <set>
#include <string>
#include "defs.h"
#include "normalizedpath.h"
#include "remotetrans.h"


namespace swordxx {

class SWMgr;
class SWModule;
class SWConfig;
class StatusReporter;

/** A remote installation source configuration
*/
class SWDLLEXPORT InstallSource {

    std::shared_ptr<SWMgr> m_mgr;

public:
    InstallSource(char const * type, char const * confEnt = nullptr);
    virtual ~InstallSource() noexcept;
    std::string getConfEnt() {
        return m_caption +"|" + m_source + "|" + m_directory + "|" + m_u + "|" + m_p + "|" + m_uid;
    }
    std::string m_caption;
    std::string m_source;
    std::string m_directory;
    std::string m_u;
    std::string m_p;
    std::string m_uid;

    std::string m_type;
    std::string m_localShadow;
    std::shared_ptr<SWMgr> const & getMgr();
    void flush();
};

/** A standard map of remote install sources.
 */
typedef std::map<std::string, std::shared_ptr<InstallSource> > InstallSourceMap;

/** Class to handle installation and maintenance of a Sword++ library of books.
 */
class SWDLLEXPORT InstallMgr {

protected:
    std::set<std::string> m_defaultMods;
    std::string m_privatePath; /**< \note Includes trailing slash. */
    std::string m_confPath;
    std::shared_ptr<StatusReporter> m_statusReporter;
    bool m_passive;
    std::string m_u, m_p;

private:

    /** we have a transport member to set as current running transport so we
     *  can ask it to terminate below, if user requests
         */
    std::shared_ptr<RemoteTransport> m_transport;

public:

    static const int MODSTAT_OLDER;
    static const int MODSTAT_SAMEVERSION;
    static const int MODSTAT_UPDATED;
    static const int MODSTAT_NEW;
    static const int MODSTAT_CIPHERED;
    static const int MODSTAT_CIPHERKEYPRESENT;

    std::unique_ptr<SWConfig> installConf;

        /** all remote sources configured for this installmgr.  Use this to gain access
         *  to individual remote sources.
         */
    InstallSourceMap sources;

    /** Username and Password supplied here can be used to identify your frontend
     *  by supplying a valid anon password like installmgr@macsword.com
     *  This will get overridden if a password is required and provided in an indivual
     *  source configuration.
         */
    InstallMgr(NormalizedPath const & privatePath = "./",
               std::shared_ptr<StatusReporter> statusReporter = nullptr,
               std::string u = "ftp",
               std::string p = "installmgr@user.com");
    virtual ~InstallMgr();

    std::shared_ptr<StatusReporter> const & statusReporter() const noexcept
    { return m_statusReporter; }

    /** Call to re-read InstallMgr.conf
         */
    void readInstallConf();

    /** Call to dump sources and other settings to InstallMgr.conf
         */
    void saveInstallConf();

    /** Removes all configured sources from memory.  Call saveInstallConf() to persist
         */
    void clearSources();

        /** call to delete all files of a locally installed module.
         */
    bool removeModule(SWMgr & manager, std::string const & modName);

        /** call to install a module from a local path (fromLocation) or remote InstallSource (is) (leave the other 0)
         */
    int installModule(SWMgr & destMgr,
                      char const * fromLocation,
                      char const * modName,
                      InstallSource * is = nullptr);

        /** call to obtain and locally cache the available content list of the remote source
         */
    int refreshRemoteSource(InstallSource & is);

        /** call to populate installmgr configuration with all known
         *  remote sources from the master list at CrossWire
         */
    virtual int refreshRemoteSourceConfiguration();

    /** override this and provide an input mechanism to allow your users
     * to enter the decipher code for a module.
     * return true you added the cipher code to the config.
     * default to return 'aborted'

A sample implementation, roughly taken from the windows installmgr:

    SectionMap::iterator section;
    ConfigEntMap::iterator entry;
    std::string tmpBuf;
    section = config->Sections.find(modName);
    if (section != config->Sections.end()) {
        entry = section->second.find("CipherKey");
        if (entry != section->second.end()) {
            entry->second = GET_USER_INPUT();
            config->Save();

            // LET'S SHOW THE USER SOME SAMPLE TEXT FROM THE MODULE
            SWMgr mgr;
            auto const mod(mgr.getModule(modName));
            mod->setKey("Ipet 2:12");
            tmpBuf = mod->StripText();
            mod->setKey("gen 1:10");
            tmpBuf += "\n\n";
            tmpBuf += mod->StripText();
            SOME_DIALOG_CONTROL->SETTEXT(tmpBuf.c_str());

            // if USER CLICKS OK means we should return true
            return true;
        }
    }
    return false;
}
    */
    virtual bool getCipherCode(const char *modName, SWConfig *config);



    /** whether or not to use passive mode when doing ftp transfers
         */
    void setFTPPassive(bool passive) { this->m_passive = passive; }
    bool isFTPPassive() { return m_passive; }

        /** call from another thread to terminate the installation process
         */
    void terminate();

    /************************************************************************
     * getModuleStatus - compare the modules of two SWMgrs and return a
     *     vector describing the status of each.  See MODSTAT_*
     */
    static std::map<SWModule *, int> getModuleStatus(const SWMgr &base, const SWMgr &other);

    /************************************************************************
     * isDefaultModule - allows an installation to provide a set of modules
     *   in installMgr.conf like:
     *     [General]
     *     DefaultMod=KJV
     *     DefaultMod=StrongsGreek
     *     DefaultMod=Personal
     *   This method allows a user interface to ask if a module is specified
     *   as a default in the above way.  The logic is, if no modules are
     *   installed then all default modules should be automatically selected for install
     *   to help the user select a basic foundation of useful modules
     */
    bool isDefaultModule(const char *modName);

private: /* Methods: */

    /** remote download from a remote source to a local destination */
    int remoteCopy(InstallSource & is,
                   const char * src,
                   const char * dest,
                   bool dirTransfer = false,
                   const char * suffix = "");
};


} /* namespace swordxx */

#endif /* SWORDXX_INSTALLMGR_H */
