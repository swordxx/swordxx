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
using std::cin;
using std::map;


SWMgr *mgr;
InstallMgr *installMgr;


void finish(int status) {
	delete installMgr;
	delete mgr;
	cout << "\n";
	exit(status);
}


void usage(const char *progName) {
	fprintf(stderr, "usage: %s <option>\nOptions:\n"
		"\t-init\t\t\t\tcreate a basic user config file.\n"
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


void initConfig() {
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
	cout << "                -=+* WARNING *+=- -=+* WARNING *+=-\n\n\n";
	cout << "Although Install Manager provides a convenient way for installing\n";
	cout << "and upgrading SWORD components, it also uses a systematic method\n";
	cout << "for accessing sites which gives packet sniffers a target to lock\n";
	cout << "into for singling out users. \n\n\n";
	cout << "IF YOU LIVE IN A PERSECUTED COUNTRY AND DO NOT WISH TO RISK DETECTION,\n";
	cout << "YOU SHOULD *NOT* USE INSTALL MANAGER'S REMOTE SOURCE FEATURES.\n\n\n";
	cout << "if you understand this and are willing to enable remote source features\n";
	cout << "then type yes at the prompt\n\n";
	cout << "enable? [no] ";
	char prompt[10];
	fgets(prompt, 9, stdin);
	bool enable = (!strcmp(prompt, "yes\n"));
	char *envhomedir  = getenv ("HOME");
	SWBuf confPath = (envhomedir) ? envhomedir : ".";
	confPath += "/.sword/InstallMgr/InstallMgr.conf";
	FileMgr::createParent(confPath.c_str());
	remove(confPath.c_str());

	InstallSource is("FTP");
	is.caption = "CrossWire";
	is.source = "ftp.crosswire.org";
	is.directory = "/pub/sword/raw";

	SWConfig config(confPath.c_str());
	config["General"]["PassiveFTP"] = "true";
	if (enable) {
		config["Sources"]["FTPSource"] = is.getConfEnt();
	}
	config.Save();
	cout << "\n\nInitialized basic config file at [" << confPath << "]\n";
	cout << "with remote source features " << ((enable) ? "ENABLED" : "DISABLED") << "\n";
}


void listModules(SWMgr *mgr) {
	cout << "Installed Modules:\n\n";
	SWModule *module;
	for (ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); it++) {
		module = it->second;
		cout << "[" << module->Name() << "]  \t- " << module->Description() << "\n";
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
		cout << "\tType     ; " << it->second->type << "\n";
		cout << "\tSource   ; " << it->second->source << "\n";
		cout << "\tDirectory; " << it->second->directory << "\n";
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

	mgr = new SWMgr();
	char *envhomedir  = getenv ("HOME");
	SWBuf baseDir = (envhomedir) ? envhomedir : ".";
	baseDir += "/.sword/InstallMgr";
	installMgr = new InstallMgr(baseDir);

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
		listRemoteSources();
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
