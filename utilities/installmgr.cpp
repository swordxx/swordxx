/******************************************************************************
 *
 *  installmgr.cpp -    commandline InstallMgr utility
 *
 * $Id$
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifdef _MSC_VER
    #pragma warning( disable: 4251 )
#endif

#include <cstdio>
#include <iostream>
#include <map>
#include <swordxx/filemgr.h>
#include <swordxx/installmgr.h>
#include <swordxx/remotetrans.h>
#include <swordxx/swlog.h>
#include <swordxx/swmgr.h>
#include <swordxx/swmodule.h>


using namespace swordxx;
using std::cout;
using std::cerr;
using std::cin;
using std::map;

namespace {
SWMgr * mgr = nullptr;
InstallMgr * installMgr = nullptr;
StatusReporter * statusReporter = nullptr;
std::string baseDir;
std::string confPath;

bool isConfirmed;

void usage(char const * progName = nullptr, char const * error = nullptr);

class MyInstallMgr : public InstallMgr {
public:
    MyInstallMgr(const char * privatePath = "./",
                 StatusReporter * sr = nullptr)
        : InstallMgr(privatePath, sr)
    {}

bool isUserDisclaimerConfirmed() const override {
    static bool confirmed = false;

    if (isConfirmed) {
        confirmed = true;
    }
        if (!confirmed) {
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        cout << "                -=+* WARNING *+=- -=+* WARNING *+=-\n\n\n";
        cout << "Although Install Manager provides a convenient way for installing\n";
        cout << "and upgrading Sword++ components, it also uses a systematic method\n";
        cout << "for accessing sites which gives packet sniffers a target to lock\n";
        cout << "into for singling out users. \n\n\n";
        cout << "IF YOU LIVE IN A PERSECUTED COUNTRY AND DO NOT WISH TO RISK DETECTION,\n";
        cout << "YOU SHOULD *NOT* USE INSTALL MANAGER'S REMOTE SOURCE FEATURES.\n\n\n";
        cout << "Also, Remote Sources other than CrossWire may contain less than\n";
        cout << "quality modules, modules with unorthodox content, or even modules\n";
        cout << "which are not legitimately distributable.  Many repositories\n";
        cout << "contain wonderfully useful content.  These repositories simply\n";
        cout << "are not reviewed or maintained by CrossWire and CrossWire\n";
        cout << "cannot be held responsible for their content. CAVEAT EMPTOR.\n\n\n";
        cout << "If you understand this and are willing to enable remote source features\n";
        cout << "then type yes at the prompt\n\n";
        cout << "enable? [no] ";

        std::string prompt;
        std::getline(cin , prompt);
        confirmed = prompt == "yes";
        cout << "\n";
    }
    return confirmed;
}

};

class MyStatusReporter : public StatusReporter {
    int last;

    /// \bug may throw, leading to std::unexpected:
    void update(std::size_t totalBytes, std::size_t completedBytes) noexcept override {
        int p = (totalBytes > 0) ? (int)(74.0 * ((double)completedBytes / (double)totalBytes)) : 0;
        for (;last < p; ++last) {
            if (!last) {
                std::string output(formatted("[ File Bytes: %ld", totalBytes));
                while (output.size() < 75) output += " ";
                output += "]";
                cout << output.c_str() << "\n ";
            }
            cout << "-";
        }
        cout.flush();
    }

    /// \bug may throw, leading to std::unexpected:
    void preStatus(std::size_t totalBytes, std::size_t completedBytes, char const * message) noexcept override {
        std::string output(formatted("[ Total Bytes: %ld; Completed Bytes: %ld", totalBytes, completedBytes));
        while (output.size() < 75) output += " ";
        output += "]";
        cout << "\n" << output.c_str() << "\n ";
        int p = (int)(74.0 * (double)completedBytes/totalBytes);
        for (int i = 0; i < p; ++i) { cout << "="; }
        cout << "\n\n" << message << "\n";
        last = 0;
    }
};


void init() {
    if (!mgr) {
        mgr = new SWMgr();

        if (!mgr->config)
            usage(nullptr,
                  "ERROR: Sword++ configuration not found.  Please configure "
                  "Sword++ before using this program.");

        std::string baseDir_ = SWMgr::getHomeDir();
        if (baseDir_.length() < 1) baseDir_ = ".";
        baseDir_ += "/.swordxx/InstallMgr";
        confPath = baseDir_ + "/InstallMgr.conf";
        statusReporter = new MyStatusReporter();
        installMgr = new MyInstallMgr(baseDir_.c_str(), statusReporter);
    }
}


// clean up and exit if status is 0 or negative error code
void finish(int status) {
    delete statusReporter;
    delete installMgr;
    delete mgr;

    installMgr = nullptr;
    mgr        = nullptr;

    if (status < 1) {
        cout << "\n";
        std::exit(status);
    }
}


void createBasicConfig(bool enableRemote, bool /* addCrossWire */) {

    FileMgr::createParent(confPath.c_str());
    remove(confPath.c_str());

    InstallSource is("FTP");
    is.m_caption = "CrossWire";
    is.m_source = "ftp.crosswire.org";
    is.m_directory = "/pub/sword/raw";

    SWConfig config(confPath.c_str());
    config["General"]["PassiveFTP"] = "true";
    if (enableRemote) {
        config["Sources"]["FTPSource"] = is.getConfEnt();
    }
    config.save();
}


void initConfig() {
    init();

    bool enable = installMgr->isUserDisclaimerConfirmed();

    createBasicConfig(enable, true);

    cout << "\n\nInitialized basic config file at [" << confPath << "]\n";
    cout << "with remote source features " << ((enable) ? "ENABLED" : "DISABLED") << "\n";
}


void syncConfig() {
    init();

    if (!installMgr->isUserDisclaimerConfirmed()) {  // assert disclaimer is accepted
        cout << "\n\nDisclaimer not accepted.  Aborting.";
        return;
    }

    // be sure we have at least some config file already out there
    if (!FileMgr::existsFile(confPath.c_str())) {
        createBasicConfig(true, false);
        finish(1); // cleanup and don't exit
        init();    // re-init with InstallMgr which uses our new config
    }

    if (!installMgr->refreshRemoteSourceConfiguration())
        cout << "\nSync'd config file with master remote source list.\n";
    else cout << "\nFailed to sync config file with master remote source list.\n";
}


void uninstallModule(const char *modName) {
    init();
    auto const it = mgr->modules().find(modName);
    if (it == mgr->modules().end()) {
        fprintf(stderr, "Couldn't find module [%s] to remove\n", modName);
        finish(-2);
    }
    installMgr->removeModule(*mgr, it->second->getName());
    cout << "Removed module: [" << modName << "]\n";
}


void listRemoteSources() {
    init();
    cout << "Remote Sources:\n\n";
    for (InstallSourceMap::iterator it = installMgr->sources.begin(); it != installMgr->sources.end(); it++) {
        cout << "[" << it->second->m_caption << "]\n";
        cout << "\tType     : " << it->second->m_type << "\n";
        cout << "\tSource   : " << it->second->m_source << "\n";
        cout << "\tDirectory: " << it->second->m_directory << "\n";
    }
}


void refreshRemoteSource(const char *sourceName) {
    init();
    InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
    if (source == installMgr->sources.end()) {
        fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
        finish(-3);
    }

    if (!installMgr->refreshRemoteSource(*source->second))
        cout << "\nRemote Source Refreshed\n";
    else    cerr << "\nError Refreshing Remote Source\n";
}


void listModules(SWMgr * otherMgr = nullptr, bool onlyNewAndUpdates = false) {
    init();
    SWModule *module;
    if (!otherMgr) otherMgr = mgr;
    std::map<SWModule *, int> mods = InstallMgr::getModuleStatus(*mgr, *otherMgr);
    for (std::map<SWModule *, int>::iterator it = mods.begin(); it != mods.end(); it++) {
        module = it->first;
        std::string version = module->getConfigEntry("Version");
        std::string status = " ";
        if (it->second & InstallMgr::MODSTAT_NEW) status = "*";
        if (it->second & InstallMgr::MODSTAT_OLDER) status = "-";
        if (it->second & InstallMgr::MODSTAT_UPDATED) status = "+";

        if (!onlyNewAndUpdates || status == "*" || status == "+") {
            cout << status << "[" << module->getName() << "]  \t(" << version << ")  \t- " << module->getDescription() << "\n";
        }
    }
}


void remoteListModules(const char *sourceName, bool onlyNewAndUpdated = false) {
    init();
    cout << "Available Modules:\n(be sure to refresh remote source (-r) first for most current list)\n\n";
    InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
    if (source == installMgr->sources.end()) {
        fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
        finish(-3);
    }
    listModules(source->second->getMgr().get(), onlyNewAndUpdated);
}


void remoteDescribeModule(char const * const sourceName,
                          char const * const modName)
{
    init();
    auto const source(installMgr->sources.find(sourceName));
    if (source == installMgr->sources.end()) {
        std::cerr << "Couldn't find remote source [" << sourceName << ']'
                  << std::endl;
        finish(-3);
    }
    auto const m(source->second->getMgr()->getModule(modName));
    if (!m) {
        std::cerr << "Couldn't find module [" << modName << "] in source ["
                  << sourceName << ']' << std::endl;
        finish(-3);
    }
    cout << "Module Description\n\n";
    for (auto const & vp : m->getConfig())
        cout << "[" << vp.first << "]:" << vp.second << "\n";
}


void localDirListModules(const char *dir) {
    cout << "Available Modules:\n\n";
    SWMgr mgr_(dir);
    listModules(&mgr_);
}


void remoteInstallModule(const char *sourceName, const char *modName) {
    init();
    InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
    if (source == installMgr->sources.end()) {
        fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
        finish(-3);
    }
    auto & is = *source->second;
    auto const rmgr(is.getMgr());
    auto const it = rmgr->modules().find(modName);
    if (it == rmgr->modules().end()) {
        fprintf(stderr, "Remote source [%s] does not make available module [%s]\n", sourceName, modName);
        finish(-4);
    }
    SWModule const & module = *it->second;

    int error = installMgr->installModule(*mgr, nullptr, module.getName().c_str(), &is);
    if (error) {
        cout << "\nError installing module: [" << module.getName() << "] (write permissions?)\n";
    } else cout << "\nInstalled module: [" << module.getName() << "]\n";
}


void localDirInstallModule(const char *dir, const char *modName) {
    init();
    SWMgr lmgr(dir);
    auto const it = lmgr.modules().find(modName);
    if (it == lmgr.modules().end()) {
        fprintf(stderr, "Module [%s] not available at path [%s]\n", modName, dir);
        finish(-4);
    }
    SWModule const & module = *it->second;
    int error = installMgr->installModule(*mgr, dir, module.getName().c_str());
    if (error) {
        cout << "\nError installing module: [" << module.getName() << "] (write permissions?)\n";
    } else cout << "\nInstalled module: [" << module.getName() << "]\n";
}


void usage(const char *progName, const char *error) {

    if (error) fprintf(stderr, "\n%s: %s\n", (progName ? progName : "installmgr"), error);

    fprintf(stderr, "\nusage: %s [--allow...] <command> [command ...]\n"
        "\n\t --allow-internet-access-and-risk-tracing-and-jail-or-martyrdom \n"
        "\n  This aptly named option will allow the program to connect to the internet without asking for user confirmation\n"
        "  In many places this may well be a risky or even foolish undertaking.\n"
        "  Please take special care before you use this option in scripts, particularly in scripts you want to offer for public download.\n"
        "  What may appear to be safe for you, may well not be safe for someone else, who uses your scripts. \n"
        "\n  Commands (run in order they are passed):\n\n"
        "\t-init\t\t\t\tcreate a basic user config file.\n"
        "\t\t\t\t\t\tWARNING: overwrites existing.\n"
        "\t-sc\t\t\t\tsync config with known remote repo list\n"
        "\t\t\t\t\t\tNOTE: also creates if none exists\n"
        "\t-s\t\t\t\tlist remote sources\n"
        "\t-r  <remoteSrcName>\t\trefresh remote source\n"
        "\t-rl <remoteSrcName>\t\tlist available modules from remote source\n"
        "\t-rd <remoteSrcName>\t\tlist new/updated modules from remote source\n"
        "\t-rdesc <remoteSrcName> <modName>\tdescribe module from remote source\n"
        "\t-ri <remoteSrcName> <modName>\tinstall module from remote source\n"
        "\t-l\t\t\t\tlist installed modules\n"
        "\t-u <modName>\t\t\tuninstall module\n"
        "\t-ll <path>\t\t\tlist available modules at local path\n"
        "\t-li <path> <modName>\t\tinstall module from local path\n"
        "\t-d\t\t\t\tturn debug output on\n"
        , (progName ? progName : "installmgr"));
    finish(-1);
}

} // anonymous namespace

int main(int argc, char **argv) {

    isConfirmed = false;

    if (argc < 2) usage(*argv);

    for (int i = 1; i < argc; i++) {
        if (!std::strcmp(argv[i], "-d")) {
            SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);
        }
        else if (!std::strcmp(argv[i], "--allow-internet-access-and-risk-tracing-and-jail-or-martyrdom")) {
            isConfirmed = true;
        }
        else if (!std::strcmp(argv[i], "-init")) {
            initConfig();
        }
        else if (!std::strcmp(argv[i], "-l")) {    // list installed modules
            cout << "Installed Modules:\n\n";
            listModules();
        }
        else if (!std::strcmp(argv[i], "-ll")) {    // list from local directory
            if (i+1 < argc) localDirListModules(argv[++i]);
            else usage(*argv, "-ll requires <path>");
        }
        else if (!std::strcmp(argv[i], "-li")) {    // install from local directory
            if (i+2 < argc) {
                const char *path = argv[++i];
                const char *modName = argv[++i];
                localDirInstallModule(path, modName);
            }
            else usage(*argv, "-li requires <path> <modName>");
        }
        else if (!std::strcmp(argv[i], "-u")) {    // uninstall module
            if (i+1 < argc) uninstallModule(argv[++i]);
            else usage(*argv, "-u requires <modName>");
        }
        else if (!std::strcmp(argv[i], "-s")) {    // list sources
            listRemoteSources();
        }
        else if (!std::strcmp(argv[i], "-sc")) {    // sync config with master
            syncConfig();
        }
        else if (!std::strcmp(argv[i], "-r")) {    // refresh remote source
            if (i+1 < argc) refreshRemoteSource(argv[++i]);
            else usage(*argv, "-r requires <remoteSrcName>");
        }
        else if (!std::strcmp(argv[i], "-rl")) {    // list remote modules
            if (i+1 < argc) remoteListModules(argv[++i]);
            else usage(*argv, "-rl requires <remoteSrcName>");
        }
        else if (!std::strcmp(argv[i], "-rd")) {    // list differences between remote source and installed modules
            if (i+1 < argc) remoteListModules(argv[++i], true);
            else usage(*argv, "-rd requires <remoteSrcName>");
        }
        else if (!std::strcmp(argv[i], "-rdesc")) { // describe remove module
            if (i + 2 < argc) {
                char const * const source = argv[++i];
                char const * const modName = argv[++i];
                remoteDescribeModule(source, modName);
            } else {
                usage(*argv, "-rdesc requires <remoteSrcName> <modName>");
            }
        }
        else if (!std::strcmp(argv[i], "-ri")) {    // install from remote directory
            if (i+2 < argc) {
                const char *source = argv[++i];
                const char *modName = argv[++i];
                remoteInstallModule(source, modName);
            }
            else usage(*argv, "-ri requires <remoteSrcName> <modName>");
        }
        else usage(*argv, (((std::string)"Unknown argument: ")+ argv[i]).c_str());
    }

    finish(0);

    return 0;
}
