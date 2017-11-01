/******************************************************************************
 *
 *  installmgr.cpp -	commandline InstallMgr utility
 *
 * $Id$
 *
 * Copyright 2003-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#ifdef _MSC_VER
	#pragma warning( disable: 4251 )
#endif

#include <swmgr.h>
#include <installmgr.h>
#include <remotetrans.h>
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


SWMgr *mgr = 0;
InstallMgr *installMgr = 0;
StatusReporter *statusReporter = 0;
SWBuf baseDir;
SWBuf confPath;

bool isConfirmed;
bool isUnvPeerAllowed;

void usage(const char *progName = 0, const char *error = 0);

class MyInstallMgr : public InstallMgr {
public:
	MyInstallMgr(const char *privatePath = "./", StatusReporter *sr = 0) : InstallMgr(privatePath, sr) {}

virtual bool isUserDisclaimerConfirmed() const {
	static bool confirmed = false;
	
	if (isConfirmed) { 
		confirmed = true;
	}
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
		cout << "\n";
	}
	return confirmed;
}

};

bool isUnverifiedPeerAllowed() {
	static bool allowed = false;
	
	if (isUnvPeerAllowed) { 
		allowed = true;
	}
        if (!allowed) {
		cout << "\n\n";
		cout << "While connecting to an encrypted install source, SWORD can allow\n";
		cout << "unverified peers, e.g., self-signed certificates. While this is\n";
		cout << "generally considered safe because SWORD only retrieves Bible content\n";
		cout << "and does not send any data to the server, it could still possibly\n";
		cout << "allow a malicious actor to sit between you and the server, as with\n";
		cout << "unencrypted sources.  Type no to turn this off.\n\n";
		cout << "Would you like to allow unverified peers? [yes] ";

		char prompt[10];
		fgets(prompt, 9, stdin);
		allowed = (strcmp(prompt, "no\n"));
		cout << "\n";
	}
	return allowed;
}

class MyStatusReporter : public StatusReporter {
	int last;
        virtual void update(unsigned long totalBytes, unsigned long completedBytes) {
		int p = (totalBytes > 0) ? (int)(74.0 * ((double)completedBytes / (double)totalBytes)) : 0;
		for (;last < p; ++last) {
			if (!last) {
				SWBuf output;
				output.setFormatted("[ File Bytes: %ld", totalBytes);
				while (output.size() < 75) output += " ";
				output += "]";
				cout << output.c_str() << "\n ";
			}
			cout << "-";
		}
		cout.flush();
	}
        virtual void preStatus(long totalBytes, long completedBytes, const char *message) {
		SWBuf output;
		output.setFormatted("[ Total Bytes: %ld; Completed Bytes: %ld", totalBytes, completedBytes);
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
			usage(0, "ERROR: SWORD configuration not found.  Please configure SWORD before using this program.");

		SWBuf baseDir = FileMgr::getSystemFileMgr()->getHomeDir();
		if (baseDir.length() < 1) baseDir = ".";
		baseDir += "/.sword/InstallMgr";
		confPath = baseDir + "/InstallMgr.conf";
		statusReporter = new MyStatusReporter();
		installMgr = new MyInstallMgr(baseDir, statusReporter);
	}
}


// clean up and exit if status is 0 or negative error code
void finish(int status) {
	delete statusReporter;
	delete installMgr;
	delete mgr;

	installMgr = 0;
	mgr        = 0;

	if (status < 1) {
		cout << "\n";
		exit(status);
	}
}


void createBasicConfig(bool enableRemote, bool addCrossWire, bool unverifiedPeerAllowed) {

	FileMgr::createParent(confPath.c_str());
	remove(confPath.c_str());

	InstallSource is("FTP");
	is.caption = "CrossWire";
	is.source = "ftp.crosswire.org";
	is.directory = "/pub/sword/raw";

	SWConfig config(confPath.c_str());
	config["General"]["PassiveFTP"] = "true";
	config["General"]["UnverifiedPeerAllowed"] = (unverifiedPeerAllowed) ? "true" : "false";
	if (enableRemote) {
		config["Sources"]["FTPSource"] = is.getConfEnt();
	}
	config.save();
}


void initConfig() {
	init();

	bool enable = installMgr->isUserDisclaimerConfirmed();
	bool allowed = isUnverifiedPeerAllowed();

	createBasicConfig(enable, true, allowed);

	cout << "\n\nInitialized basic config file at [" << confPath << "]\n";
	cout << "with remote source features " << ((enable) ? "ENABLED" : "DISABLED") << "\n";
	cout << "with unverified peers " << ((allowed) ? "ALLOWED" : "DISALLOWED") << "\n";
}


void syncConfig() {
	init();

	if (!installMgr->isUserDisclaimerConfirmed()) {  // assert disclaimer is accepted
		cout << "\n\nDisclaimer not accepted.  Aborting.";
		return;
	}

	// be sure we have at least some config file already out there
	if (!FileMgr::existsFile(confPath.c_str())) {
		createBasicConfig(true, false, false);
		finish(1); // cleanup and don't exit
		init();    // re-init with InstallMgr which uses our new config
	}

	if (!installMgr->refreshRemoteSourceConfiguration()) 
		cout << "\nSync'd config file with master remote source list.\n";
	else cout << "\nFailed to sync config file with master remote source list.\n";
}


void uninstallModule(const char *modName) {
	init();
	SWModule *module;
	ModMap::iterator it = mgr->Modules.find(modName);
	if (it == mgr->Modules.end()) {
		fprintf(stderr, "Couldn't find module [%s] to remove\n", modName);
		finish(-2);
	}
	module = it->second;
	installMgr->removeModule(mgr, module->getName());
	cout << "Removed module: [" << modName << "]\n";
}


void listRemoteSources() {
	init();
	cout << "Remote Sources:\n\n";
	for (InstallSourceMap::iterator it = installMgr->sources.begin(); it != installMgr->sources.end(); it++) {
		cout << "[" << it->second->caption << "]\n";
		cout << "\tType     : " << it->second->type << "\n";
		cout << "\tSource   : " << it->second->source << "\n";
		cout << "\tDirectory: " << it->second->directory << "\n";
	}
}


void refreshRemoteSource(const char *sourceName) {
	init();
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
		fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
		finish(-3);
	}

	if (!installMgr->refreshRemoteSource(source->second))
		cout << "\nRemote Source Refreshed\n";
	else	cerr << "\nError Refreshing Remote Source\n";
}


void listModules(SWMgr *otherMgr = 0, bool onlyNewAndUpdates = false) {
	init();
	SWModule *module;
	if (!otherMgr) otherMgr = mgr;
	std::map<SWModule *, int> mods = InstallMgr::getModuleStatus(*mgr, *otherMgr);
	for (std::map<SWModule *, int>::iterator it = mods.begin(); it != mods.end(); it++) {
		module = it->first;
		SWBuf version = module->getConfigEntry("Version");
		SWBuf status = " ";
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
	listModules(source->second->getMgr(), onlyNewAndUpdated);
}


void remoteDescribeModule(const char *sourceName, const char *modName) {
	init();
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
		fprintf(stderr, "Couldn't find remote source [%s]\n", sourceName);
		finish(-3);
	}
	SWMgr *mgr = source->second->getMgr();
	SWModule *m = mgr->getModule(modName);
	if (!m) {
		fprintf(stderr, "Couldn't find module [%s] in source [%s]\n", modName, sourceName);
		finish(-3);
	}
	cout << "Module Description\n\n";
	for (ConfigEntMap::const_iterator it = m->getConfig().begin(); it != m->getConfig().end(); ++it) {
		cout << "[" << it->first << "]:" << it->second << "\n";
	}
}


void localDirListModules(const char *dir) {
	cout << "Available Modules:\n\n";
	SWMgr mgr(dir);
	listModules(&mgr);
}


void remoteInstallModule(const char *sourceName, const char *modName) {
	init();
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

	int error = installMgr->installModule(mgr, 0, module->getName(), is);
	if (error) {
		cout << "\nError installing module: [" << module->getName() << "] (write permissions?)\n";
	} else cout << "\nInstalled module: [" << module->getName() << "]\n";
}


void localDirInstallModule(const char *dir, const char *modName) {
	init();
	SWMgr lmgr(dir);
	SWModule *module;
	ModMap::iterator it = lmgr.Modules.find(modName);
	if (it == lmgr.Modules.end()) {
		fprintf(stderr, "Module [%s] not available at path [%s]\n", modName, dir);
		finish(-4);
	}
	module = it->second;
	int error = installMgr->installModule(mgr, dir, module->getName());
	if (error) {
		cout << "\nError installing module: [" << module->getName() << "] (write permissions?)\n";
	} else cout << "\nInstalled module: [" << module->getName() << "]\n";
}


void usage(const char *progName, const char *error) {

	if (error) fprintf(stderr, "\n%s: %s\n", (progName ? progName : "installmgr"), error);

	fprintf(stderr, "\nusage: %s [--allow...] <command> [command ...]\n"
		"\n\t --allow-internet-access-and-risk-tracing-and-jail-or-martyrdom \n"
		"\n  This aptly named option will allow the program to connect to the internet without asking for user confirmation\n"
		"  In many places this may well be a risky or even foolish undertaking.\n"
		"  Please take special care before you use this option in scripts, particularly in scripts you want to offer for public download.\n" 
		"  What may appear to be safe for you, may well not be safe for someone else, who uses your scripts. \n"
		"\n\t --allow-unverified-tls-peer \n"
		"\n  This option will allow the program to connect to unverified peers\n"
		"  (e.g., hosts using self-signed certificates) without asking for user confirmation.\n"
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


int main(int argc, char **argv) {
	
	isConfirmed = false;
	isUnvPeerAllowed = false;
	
	if (argc < 2) usage(*argv);

	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-d")) {
			SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);
		}
		else if (!strcmp(argv[i], "--allow-internet-access-and-risk-tracing-and-jail-or-martyrdom")) {
			isConfirmed = true;
		}
		else if (!strcmp(argv[i], "--allow-unverified-tls-peer")) {
			isUnvPeerAllowed = true;
		}
		else if (!strcmp(argv[i], "-init")) {
			initConfig();
		}
		else if (!strcmp(argv[i], "-l")) {	// list installed modules
			cout << "Installed Modules:\n\n";
			listModules();
		}
		else if (!strcmp(argv[i], "-ll")) {	// list from local directory
			if (i+1 < argc) localDirListModules(argv[++i]);
			else usage(*argv, "-ll requires <path>");
		}
		else if (!strcmp(argv[i], "-li")) {	// install from local directory
			if (i+2 < argc) {
				const char *path = argv[++i];
				const char *modName = argv[++i];
				localDirInstallModule(path, modName);
			}
			else usage(*argv, "-li requires <path> <modName>");
		}
		else if (!strcmp(argv[i], "-u")) {	// uninstall module
			if (i+1 < argc) uninstallModule(argv[++i]);
			else usage(*argv, "-u requires <modName>");
		}
		else if (!strcmp(argv[i], "-s")) {	// list sources
			listRemoteSources();
		}
		else if (!strcmp(argv[i], "-sc")) {	// sync config with master
			syncConfig();
		}
		else if (!strcmp(argv[i], "-r")) {	// refresh remote source
			if (i+1 < argc) refreshRemoteSource(argv[++i]);
			else usage(*argv, "-r requires <remoteSrcName>");
		}
		else if (!strcmp(argv[i], "-rl")) {	// list remote modules
			if (i+1 < argc) remoteListModules(argv[++i]);
			else usage(*argv, "-rl requires <remoteSrcName>");
		}
		else if (!strcmp(argv[i], "-rd")) {	// list differences between remote source and installed modules
			if (i+1 < argc) remoteListModules(argv[++i], true);
			else usage(*argv, "-rd requires <remoteSrcName>");
		}
		else if (!strcmp(argv[i], "-rdesc")) {	// describe remove module
			if (i+2 < argc) {
				const char *source = argv[++i];
				const char *modName = argv[++i];
				remoteDescribeModule(source, modName);
			}
			else usage(*argv, "-rdesc requires <remoteSrcName> <modName>");
		}
		else if (!strcmp(argv[i], "-ri")) {	// install from remote directory
			if (i+2 < argc) {
				const char *source = argv[++i];
				const char *modName = argv[++i];
				remoteInstallModule(source, modName);
			}
			else usage(*argv, "-ri requires <remoteSrcName> <modName>");
		}
		else usage(*argv, (((SWBuf)"Unknown argument: ")+ argv[i]).c_str());
	}

	finish(0);

	return 0;
}
