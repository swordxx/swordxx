/*
 * Copyright 2009 CrossWire Bible Society (http://www.crosswire.org)
 *	CrossWire Bible Society
 *	P. O. Box 2528
 *	Tempe, AZ  85280-2528
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

#include <swmgr.h>
#include <installmgr.h>
#include <filemgr.h>
#include <iostream>
#include <map>
#include <swmodule.h>
#include <stdio.h>
#include <swlog.h>

using namespace sword;
using std::cout;
using std::cerr;
using std::cin;
using std::map;


SWMgr *mgr;
InstallMgr *installMgr;
SWBuf baseDir;
SWBuf confPath;


class MyInstallMgr : public InstallMgr {
public:
	MyInstallMgr(const char *privatePath = "./") : InstallMgr(privatePath) {}

virtual bool isUserDisclaimerConfirmed() const {
	static bool confirmed = false;
        if (!confirmed) {
		cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
		cout << "                -=+* WARNING *+=- -=+* WARNING *+=-\n\n\n";
		cout << "Although Install Manager provides a convenient way for installing\n";
		cout << "and upgrading SWORD components, it also uses a systematic method\n";
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

		char prompt[10];
		fgets(prompt, 9, stdin);
		confirmed = (!strcmp(prompt, "yes\n"));
	}
	return confirmed;
}

};


void init() {
	mgr = new SWMgr();
	SWBuf baseDir = mgr->getHomeDir();
	if (baseDir.length() < 1) baseDir = ".";
	baseDir += "/.sword/InstallMgr";
	confPath = baseDir + "/InstallMgr.conf";
	installMgr = new MyInstallMgr(baseDir);
}


// clean up and exit if status is 0 or negative error code
void finish(int status) {
	delete installMgr;
	delete mgr;
	if (status < 1) {
		cout << "\n";
		exit(status);
	}
}


void usage(const char *progName) {
	fprintf(stderr, "usage: %s <option>\nOptions:\n"
		"\t-init\t\t\t\tcreate a basic user config file.\n"
		"\t-sc\t\t\t\tsync config with known remote repo list\n"
		"\t\t\t\t\t\tWARNING: overwrites existing.\n"
		"\t-l\t\t\t\tlist installed modules\n"
		"\t-u <modName>\t\t\tuninstall module\n"
		"\t-s\t\t\t\tlist remote sources\n"
		"\t-r  <remoteSrcName>\t\trefresh remote source\n"
		"\t-rl <remoteSrcName>\t\tlist available modules from remote source\n"
		"\t-rd <remoteSrcName>\t\tlist new/updated modules from remote source\n"
		"\t-ri <remoteSrcName> <modName>\tinstall module from remote source\n"
		"\t-ll <path>\t\t\tlist available modules at local path\n"
		"\t-li <path> <modName>\t\tinstall module from local path\n"
		, progName);
	finish(-1);
}


void createBasicConfig(bool enableRemote, bool addCrossWire) {

	FileMgr::createParent(confPath.c_str());
	remove(confPath.c_str());

	InstallSource is("FTP");
	is.caption = "CrossWire";
	is.source = "ftp.crosswire.org";
	is.directory = "/pub/sword/raw";

	SWConfig config(confPath.c_str());
	config["General"]["PassiveFTP"] = "true";
	if (enableRemote) {
		config["Sources"]["FTPSource"] = is.getConfEnt();
	}
	config.Save();
}


void initConfig() {

	bool enable = installMgr->isUserDisclaimerConfirmed();

	createBasicConfig(enable, true);

	cout << "\n\nInitialized basic config file at [" << confPath << "]\n";
	cout << "with remote source features " << ((enable) ? "ENABLED" : "DISABLED") << "\n";
}


void syncConfig() {

	if (!installMgr->isUserDisclaimerConfirmed()) {  // assert disclaimer is accepted
		cout << "\n\nDisclaimer not accepted.  Aborting.";
		return;
	}

	if (!FileMgr::existsFile(confPath.c_str())) {
		createBasicConfig(true, false);
		finish(1); // cleanup and don't exit
		init();    // re-init with InstallMgr which uses our new config
	}

	if (!installMgr->refreshRemoteSourceConfiguration()) 
		cout << "\nSync'd config file with master remote source list.\n";
	else cout << "\nFailed to sync config file with master remote source list.\n";
}


void listModules(SWMgr *otherMgr) {
	cout << "Installed Modules:\n\n";
	SWModule *module;
	std::map<SWModule *, int> mods = InstallMgr::getModuleStatus(*mgr, *otherMgr);
	for (std::map<SWModule *, int>::iterator it = mods.begin(); it != mods.end(); it++) {
		module = it->first;
		SWBuf version = module->getConfigEntry("Version");
		SWBuf status = " ";
		if (it->second & InstallMgr::MODSTAT_NEW) status = "*";
		if (it->second & InstallMgr::MODSTAT_OLDER) status = "-";
		if (it->second & InstallMgr::MODSTAT_UPDATED) status = "+";

		cout << status << "[" << module->Name() << "]  \t(" << version << ")  \t- " << module->Description() << "\n";
	}
}


void uninstallModule(const char *modName) {
	SWModule *module;
	ModMap::iterator it = mgr->Modules.find(modName);
	if (it == mgr->Modules.end()) {
		fprintf(stderr, "Couldn't find module [%s] to remove\n", modName);
		finish(-2);
	}
	module = it->second;
	installMgr->removeModule(mgr, module->Name());
	cout << "Removed module: [" << modName << "]\n";
}


void listRemoteSources() {
	cout << "Remote Sources:\n\n";
	for (InstallSourceMap::iterator it = installMgr->sources.begin(); it != installMgr->sources.end(); it++) {
		cout << "[" << it->second->caption << "]\n";
		cout << "\tType     : " << it->second->type << "\n";
		cout << "\tSource   : " << it->second->source << "\n";
		cout << "\tDirectory: " << it->second->directory << "\n";
	}
}


void refreshRemoteSource(const char *sourceName) {
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
		fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
		finish(-3);
	}

	if (!installMgr->refreshRemoteSource(source->second))
		cout << "Remote Source Refreshed\n";
	else	cerr << "Error Refreshing Remote Source\n";
}


void remoteNewModules(const SWMgr *base, const char *sourceName) {
	cout << "Updated and New Modules:\n(be sure to refresh remote source (-r) first for most current list)\n\n";
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
		fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
		finish(-3);
	}
	map<SWModule *, int> modStats = installMgr->getModuleStatus(*base, *source->second->getMgr());
	SWModule *module;
	int status;
	bool updated;
	for (map<SWModule *, int>::iterator it = modStats.begin(); it != modStats.end(); it++) {
		module = it->first;
		status = it->second;
		updated = (status & InstallMgr::MODSTAT_UPDATED);
		if ((status & InstallMgr::MODSTAT_NEW) || (updated)) {
			cout << ((updated)?"U":"N") << " [" << module->Name() << "]  \t- " << module->Description() << "\n";
		}
	}
}


void remoteListModules(const char *sourceName) {
	cout << "Available Modules:\n(be sure to refresh remote source (-r) first for most current list)\n\n";
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
		fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
		finish(-3);
	}
	listModules(source->second->getMgr());
}


void localDirListModules(const char *dir) {
	cout << "Available Modules:\n\n";
	SWMgr mgr(dir);
	listModules(&mgr);
}


void remoteInstallModule(const char *sourceName, const char *modName) {
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
		fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
		finish(-3);
	}
	InstallSource *is = source->second;
	SWMgr *rmgr = is->getMgr();
	SWModule *module;
	ModMap::iterator it = rmgr->Modules.find(modName);
	if (it == rmgr->Modules.end()) {
		fprintf(stderr, "Remote source [%s] does not make available module [%s]\n", sourceName, modName);
		finish(-4);
	}
	module = it->second;

	int error = installMgr->installModule(mgr, 0, module->Name(), is);
	if (error) {
		cout << "Error installing module: [" << module->Name() << "] (write permissions?)\n";
	} else cout << "Installed module: [" << module->Name() << "]\n";
}


void localDirInstallModule(const char *dir, const char *modName) {
	SWMgr lmgr(dir);
	SWModule *module;
	ModMap::iterator it = lmgr.Modules.find(modName);
	if (it == lmgr.Modules.end()) {
		fprintf(stderr, "Module [%s] not available at path [%s]\n", modName, dir);
		finish(-4);
	}
	module = it->second;
	int error = installMgr->installModule(mgr, dir, module->Name());
	if (error) {
		cout << "Error installing module: [" << module->Name() << "] (write permissions?)\n";
	} else cout << "Installed module: [" << module->Name() << "]\n";
}


int main(int argc, char **argv) {

	init();

	SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);

	cout << "\n";

	if (argc < 2)
		usage(*argv);

	switch (argv[1][1]) {
	case 'i':
		if (strcmp(argv[1], "-init"))
			usage(*argv);
		initConfig();
		break;
	case 'l':
		switch (argv[1][2]) {
		case 0:			// -l list installed modules
			listModules(mgr);
			break;
		case 'l':		// -ll list from local directory
			if (argc < 3)
				usage(*argv);
			localDirListModules(argv[2]);
			break;
		case 'i':		// -li remote install
			if (argc < 4)
				usage(*argv);
			localDirInstallModule(argv[2], argv[3]);
			break;
		default: usage(*argv);
		}
		break;
	case 'u':
		if (argc < 3)
			usage(*argv);

		uninstallModule(argv[2]);
		break;
	case 's':
		switch (argv[1][2]) {
		case 0:			// -s list sources
			listRemoteSources();
			break;
		case 'c':		// -sc sync config with master
			syncConfig();
			break;
		}
		break;
	case 'r':	// remote option
		switch (argv[1][2]) {
		case 0:			// -r refresh
			if (argc < 3)
				usage(*argv);
			refreshRemoteSource(argv[2]);
			break;
		case 'l':		// -rl remote list
			if (argc < 3)
				usage(*argv);
			remoteListModules(argv[2]);
			break;
		case 'd':		// -rl remote list
			if (argc < 3)
				usage(*argv);
			remoteNewModules(mgr, argv[2]);
			break;
		case 'i':		// -ri remote install
			if (argc < 4)
				usage(*argv);
			remoteInstallModule(argv[2], argv[3]);
			break;
		default: usage(*argv);
		}
		break;
	default: usage(*argv);
	}

	finish(0);

	return 0;
}
