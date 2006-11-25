 /*****************************************************************************
 * InstallMgr functions to be made into something usefully exposed by
 * master Glassey
 *
 */


#ifndef EXCLUDEZLIB
extern "C" {
#include <untgz.h>
}
#endif

#include <installmgr.h>
#include <filemgr.h>
#include <utilstr.h>

#include <fcntl.h>

#include <swmgr.h>
#include <dirent.h>

#include <stdio.h>

#ifdef CURLAVAILABLE
#include <curlftpt.h>
#else
#include <ftplibftpt.h>
#endif


SWORD_NAMESPACE_START


// override this method and provide your own custom FTPTransport subclass
// here we try a couple defaults if sword was compiled with support for them.
// see these classes for examples of how to make your own
FTPTransport *InstallMgr::createFTPTransport(const char *host, StatusReporter *statusReporter) {
#ifdef CURLAVAILABLE
	return new CURLFTPTransport(host, statusReporter);
#else
	return new FTPLibFTPTransport(host, statusReporter);
#endif
}




InstallMgr::InstallMgr(const char *privatePath, StatusReporter *sr) {
	statusReporter = sr;
	this->privatePath = 0;
	this->transport = 0;
	stdstr(&(this->privatePath), privatePath);
	SWBuf confPath = (SWBuf)privatePath + "/InstallMgr.conf";
	FileMgr::createParent(confPath.c_str());
	
	installConf = new SWConfig(confPath.c_str());

	SectionMap::iterator sourcesSection;
	ConfigEntMap::iterator sourceBegin;
	ConfigEntMap::iterator sourceEnd;

	sources.clear();
	
	setFTPPassive(stricmp((*installConf)["General"]["PassiveFTP"].c_str(), "false")!=0);

	sourcesSection = installConf->Sections.find("Sources");
	if (sourcesSection != installConf->Sections.end()) {
		sourceBegin = sourcesSection->second.lower_bound("FTPSource");
		sourceEnd = sourcesSection->second.upper_bound("FTPSource");

		while (sourceBegin != sourceEnd) {
			InstallSource *is = new InstallSource("FTP", sourceBegin->second.c_str());
			sources[is->caption] = is;
			SWBuf parent = (SWBuf)privatePath + "/" + is->source + "/file";
			FileMgr::createParent(parent.c_str());
			is->localShadow = (SWBuf)privatePath + "/" + is->source;
			sourceBegin++;
		}
	}

	defaultMods.clear();
	sourcesSection = installConf->Sections.find("General");
	if (sourcesSection != installConf->Sections.end()) {
		sourceBegin = sourcesSection->second.lower_bound("DefaultMod");
		sourceEnd = sourcesSection->second.upper_bound("DefaultMod");

		while (sourceBegin != sourceEnd) {
			defaultMods.insert(sourceBegin->second.c_str());
			sourceBegin++;
		}
	}
}


InstallMgr::~InstallMgr() {
	delete [] privatePath;
	delete installConf;

	for (InstallSourceMap::iterator it = sources.begin(); it != sources.end(); ++it) {
		delete it->second;
	}
}


void InstallMgr::terminate() { if (transport) transport->terminate(); }

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
		if (fileBegin != fileEnd) {	// remove each file
			while (fileBegin != fileEnd) {
				modFile = modDir;
				modFile += "/";
				modFile += fileBegin->second.c_str();
				//remove file
				FileMgr::removeFile(modFile.c_str());
				fileBegin++;
			}
		}
		else {	//remove all files in DataPath directory

			DIR *dir;
			struct dirent *ent;
			ConfigEntMap::iterator entry;

			FileMgr::removeDir(modDir.c_str());

			if ((dir = opendir(manager->configPath))) {	// find and remove .conf file
				rewinddir(dir);
				while ((ent = readdir(dir))) {
					if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
						modFile = manager->configPath;
						modFile += "/";
						modFile += ent->d_name;
						SWConfig *config = new SWConfig(modFile.c_str());
						if (config->Sections.find(modName) != config->Sections.end()) {
							delete config;
							FileMgr::removeFile(modFile.c_str());
						}
						else	delete config;
					}
				}
				closedir(dir);
			}
		}
		return 0;
	}
	return 1;
}



InstallSource::InstallSource(const char *type, const char *confEnt) {
	this->type = type;
	mgr = 0;
	userData = 0;
	if (confEnt) {
		char *buf = 0;
		stdstr(&buf, confEnt);

		caption = strtok(buf, "|");
		source = strtok(0, "|");
		directory = strtok(0, "|");
		delete [] buf;
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


int InstallMgr::ftpCopy(InstallSource *is, const char *src, const char *dest, bool dirTransfer, const char *suffix) {
	int retVal = 0;
	FTPTransport *trans = createFTPTransport(is->source, statusReporter);
	transport = trans; // set classwide current transport for other thread terminate() call
	trans->setPassive(passive);
	
	SWBuf urlPrefix = (SWBuf)"ftp://" + is->source;

	// let's be sure we can connect.  This seems to be necessary but sucks
//	SWBuf url = urlPrefix + is->directory.c_str() + "/"; //dont forget the final slash
//	if (trans->getURL("dirlist", url.c_str())) {
//		 fprintf(stderr, "FTPCopy: failed to get dir %s\n", url.c_str());
//		 return -1;
//	}

	   
	if (dirTransfer) {
		SWBuf dir = (SWBuf)is->directory.c_str() + "/" + src; //dont forget the final slash

		retVal = trans->copyDirectory(urlPrefix, dir, dest, suffix);


	}
	else {
		SWTRY {
			SWBuf url = urlPrefix + is->directory.c_str() + "/" + src; //dont forget the final slash
			if (trans->getURL(dest, url.c_str())) {
				fprintf(stderr, "FTPCopy: failed to get file %s", url.c_str());
				retVal = -1;
			}
		}
		SWCATCH (...) {
			retVal = -1;
		}
	}
	SWTRY {
		FTPTransport *deleteMe = trans;
		// do this order for threadsafeness
		// (see terminate())
		trans = transport = 0;
		delete deleteMe;
	}
	SWCATCH (...) {}
	return retVal;
}


int InstallMgr::installModule(SWMgr *destMgr, const char *fromLocation, const char *modName, InstallSource *is) {
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


	if (is)
		sourceDir = (SWBuf)privatePath + "/" + is->source;
	else	sourceDir = fromLocation;

	if (sourceDir[sourceDir.length()-1] != '/')
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

		if (fileBegin != fileEnd) {	// copy each file
			if (is) {
				while (fileBegin != fileEnd) {	// ftp each file first
					buffer = sourceDir + "/" + fileBegin->second.c_str();
					if (ftpCopy(is, fileBegin->second.c_str(), buffer.c_str())) {
						aborted = true;
						break;	// user aborted
					}
					fileBegin++;
				}
				fileBegin = module->second.lower_bound("File");
			}

			if (!aborted) {
				// DO THE INSTALL
				while (fileBegin != fileEnd) {
					SWBuf sourcePath = sourceDir;
					sourcePath += fileBegin->second.c_str();
					SWBuf dest = destMgr->prefixPath;
					if ((destMgr->prefixPath[strlen(destMgr->prefixPath)-1] != '\\') && (destMgr->prefixPath[strlen(destMgr->prefixPath)-1] != '/'))
						dest += "/";
					dest += fileBegin->second.c_str();
					FileMgr::copyFile(sourcePath.c_str(), dest.c_str());

					fileBegin++;
				}
			}
			//---------------

			if (is) {
				fileBegin = module->second.lower_bound("File");
				while (fileBegin != fileEnd) {	// delete each tmp ftp file
					buffer = sourceDir + "/" + fileBegin->second.c_str();
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
				relativePath << strlen(mgr.prefixPath);

				if (is) {
					if (ftpCopy(is, relativePath.c_str(), absolutePath.c_str(), true)) {
						aborted = true;	// user aborted
					}
				}
				if (!aborted) {
					SWBuf destPath = (SWBuf)destMgr->prefixPath + relativePath;
					FileMgr::copyDir(absolutePath.c_str(), destPath.c_str());
				}
				if (is) {		// delete tmp ftp files
//					mgr->deleteModule(modName);
					FileMgr::removeDir(absolutePath.c_str());
				}
			}
		}
		if (!aborted) {
			SWBuf confDir = sourceDir + "/mods.d/";
			if ((dir = opendir(confDir.c_str()))) {	// find and copy .conf file
				rewinddir(dir);
				while ((ent = readdir(dir))) {
					if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
						modFile = confDir;
						modFile += ent->d_name;
						SWConfig *config = new SWConfig(modFile.c_str());
						if (config->Sections.find(modName) != config->Sections.end()) {
							SWBuf targetFile = destMgr->configPath; //"./mods.d/";
							targetFile += "/";
							targetFile += ent->d_name;
							FileMgr::copyFile(modFile.c_str(), targetFile.c_str());
							if (cipher) {
								if (getCipherCode(modName, config)) {
									SWMgr newDest(destMgr->prefixPath);
									removeModule(&newDest, modName);
									aborted = true;
								}
								else {
									config->Save();
									FileMgr::copyFile(modFile.c_str(), targetFile.c_str());
								}
							}
						}
						delete config;
					}
				}
				closedir(dir);
			}
		}
		return (aborted) ? -1 : 0;
	}
	return 1;
}


// override this and provide an input mechanism to allow your users
// to enter the decipher code for a module.
// return true you added the cipher code to the config.
// default to return 'aborted'
bool InstallMgr::getCipherCode(const char *modName, SWConfig *config) {
	return false;

/* a sample implementation, roughly taken from the windows installmgr

	SectionMap::iterator section;
	ConfigEntMap::iterator entry;
	SWBuf tmpBuf;
	section = config->Sections.find(modName);
	if (section != config->Sections.end()) {
		entry = section->second.find("CipherKey");
		if (entry != section->second.end()) {
			entry->second = GET_USER_INPUT();
			config->Save();

			// LET'S SHOW THE USER SOME SAMPLE TEXT FROM THE MODULE
			SWMgr *mgr = new SWMgr();
			SWModule *mod = mgr->Modules[modName];
			mod->setKey("Ipet 2:12");
			tmpBuf = mod->StripText();
			mod->setKey("gen 1:10");
			tmpBuf += "\n\n";
			tmpBuf += mod->StripText();
			SOME_DIALOG_CONTROL->SETTEXT(tmpBuf.c_str());
			delete mgr;

			// if USER CLICKS OK means we should return true
			return true;
		}
	}
	return false;
*/

}


int InstallMgr::refreshRemoteSource(InstallSource *is) {
	SWBuf root = (SWBuf)privatePath + (SWBuf)"/" + is->source.c_str();
	SWBuf target = root + "/mods.d";
	int errorCode = -1; //0 means successful
	
	FileMgr::removeDir(target.c_str());

	if (!FileMgr::existsDir(target))
		FileMgr::createPathAndFile(target+"/globals.conf");

#ifndef EXCLUDEZLIB
	SWBuf archive = root + "/mods.d.tar.gz";
	
	errorCode = ftpCopy(is, "mods.d.tar.gz", archive.c_str(), false);
	if (!errorCode) { //sucessfully downloaded the tar,gz of module configs
		FileDesc *fd = FileMgr::getSystemFileMgr()->open(archive.c_str(), FileMgr::RDONLY);
		untargz(fd->getFd(), root.c_str());
		FileMgr::getSystemFileMgr()->close(fd);
	}
	else if (!term) //if the tar.gz download was canceled don't continue with another download
#endif
	errorCode = ftpCopy(is, "mods.d", target.c_str(), true, ".conf"); //copy the whole directory
	
	is->flush();
	return errorCode;
}


bool InstallMgr::isDefaultModule(const char *modName) {
	return defaultMods.count(modName);
}


SWORD_NAMESPACE_END

