/******************************************************************************
 *  refsysmgr.cpp   - definition of class RefSysMgr used to interact with
 *				registered biblical reference systems for a sword installation
 *
 * $Id: refsysmgr.cpp,v 1.1 2004/05/07 17:10:46 dglassey Exp $
 *
 * Copyright 2004 CrossWire Bible Society (http://www.crosswire.org)
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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#include <unixstr.h>
#endif
#include <sys/stat.h>
#include <dirent.h>
#if 1
#include <errno.h>
#endif

#include <swmgr.h>
#include <utilfuns.h>

#include <stringmgr.h>
#include <filemgr.h>
#include <refsysmgr.h>
#include <refsys.h>
#include <versekey2.h>

SWORD_NAMESPACE_START


RefSysMgr *RefSysMgr::systemRefSysMgr = 0;

class __staticsystemRefSysMgr {
public:
	__staticsystemRefSysMgr() { }
	~__staticsystemRefSysMgr() { delete RefSysMgr::systemRefSysMgr; }
} _staticsystemRefSysMgr;


RefSysMgr *RefSysMgr::getSystemRefSysMgr() {
	if (!systemRefSysMgr)
		systemRefSysMgr = new RefSysMgr();

	return systemRefSysMgr;
}


void RefSysMgr::setSystemRefSysMgr(RefSysMgr *newRefSysMgr) {
	if (systemRefSysMgr)
		delete systemRefSysMgr;
	systemRefSysMgr = newRefSysMgr;
}


RefSysMgr::RefSysMgr(const char *iConfigPath) {
	m_reflist = new RefSysMap();
	char *prefixPath = 0;
	char *configPath = 0;
	char configType = 0;
	SWBuf path;
	
	defaultRefSysName = 0;
	refpath = "";
	

	if (!iConfigPath)
		SWMgr::findConfig(&configType, &prefixPath, &configPath);
	else configPath = (char *)iConfigPath;

	if (prefixPath) {
		switch (configType) {
		case 2:
			int i;
			for (i = strlen(configPath)-1; ((i) && (configPath[i] != '/') && (configPath[i] != '\\')); i--);
			configPath[i] = 0;
			path = configPath;
			path += "/";
			break;
		default:
			path = prefixPath;
			if ((prefixPath[strlen(prefixPath)-1] != '\\') && (prefixPath[strlen(prefixPath)-1] != '/'))
				path += "/";

			break;
		}
		if (FileMgr::existsDir(path.c_str(), "refsys")) {
			refpath = path;
			refpath += "refsys";
		}
		if (FileMgr::existsDir(path.c_str(), "refs.d")) {
			path += "refs.d";
			loadConfigDir(path.c_str());
		}
	}

	if (prefixPath)
		delete [] prefixPath;

	if (configPath)
		delete [] configPath;

	char *ref = getenv ("REFSYS");
	if (ref) {
		if (strlen(ref) > 0)
		{
			if (getRefSys(ref)!=0)
				setDefaultRefSysName(ref);
			else setDefaultRefSysName("KJV");
		}
		else setDefaultRefSysName("KJV");
	}
	else setDefaultRefSysName("KJV");
	

}


RefSysMgr::~RefSysMgr() {
	if (defaultRefSysName)
		delete [] defaultRefSysName;
    deleteRefSys();
	delete m_reflist;
}


void RefSysMgr::loadConfigDir(const char *ipath) {
	DIR *dir;
	struct dirent *ent;
	SWBuf newmodfile;
	RefSysMap::iterator it;
	SWConfig *refconf;
	ConfigEntMap::iterator confEntry;
	RefDetails refdet;
	RefSys *ref = NULL;
	char *name;
	int cps;
 
	if ((dir = opendir(ipath))) {
		rewinddir(dir);
		while ((ent = readdir(dir))) {
			if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {
				newmodfile = ipath;
				if ((ipath[strlen(ipath)-1] != '\\') && (ipath[strlen(ipath)-1] != '/'))
					newmodfile += "/";
				newmodfile += ent->d_name;
				printf("found refsys %s\n", newmodfile.c_str());
				refconf = new SWConfig(newmodfile.c_str());
				name = 0;
				confEntry = refconf->Sections["RefSys"].find("Name");
				if (confEntry != refconf->Sections["RefSys"].end())
					stdstr(&(name), (*confEntry).second.c_str());
				confEntry = refconf->Sections["RefSys"].find("CpsSize");
				if (confEntry != refconf->Sections["RefSys"].end())
					cps = atoi((*confEntry).second.c_str());
				refdet = std::make_pair(ref, cps);
				it = m_reflist->find(name);
				if (name && cps && it == m_reflist->end()) // not already present
					m_reflist->insert(RefSysMap::value_type(name, refdet));
				delete[] name;
				delete refconf;
			}
		}
		closedir(dir);
	}
	#if 0
	it = m_reflist->find("KJV");
	if (it == m_reflist->end()) // add the system default KJV refsys if not already there
	{
		printf("adding default refsys KJV\n");
		ref = NULL;
		refdet = std::make_pair(ref, 0);
		m_reflist->insert(RefSysMap::value_type("KJV", refdet));
	}
	#endif
}


void RefSysMgr::deleteRefSys() {

	RefSysMap::iterator it;

	for (it = m_reflist->begin(); it != m_reflist->end(); it++)
	{
		if ((*it).second.first)
			delete (*it).second.first;		// RefSys*
		//delete[] (*it).first;				// name
	}

	m_reflist->erase(m_reflist->begin(), m_reflist->end());
}


RefSys *RefSysMgr::getRefSys(const char *name) {
	RefSysMap::iterator it;

	it = m_reflist->find(name);
	if (it != m_reflist->end())
	{
		if (!(*it).second.first)
			(*it).second.first = loadRefSys(name, (*it).second.second);
		return (*it).second.first;
	}

	return 0;
}

VerseKey2 *RefSysMgr::getVerseKey(const char *name, const char *key) {
	VerseKey2 *vk = NULL;
	RefSys *ref = getRefSys(name);
	if (ref)
		vk = new VerseKey2(key, ref);
	return vk;
}


RefSys *RefSysMgr::loadRefSys(const char *name, int cps) {
	const bkref *offsets[2];
	int offsize[2];
	SWBuf buf;
	FileDesc *fd;
	int bkssize, cpssize;
	RefSys *retval = NULL;
	int bread;

	bkssize = (MAXOSISBOOKS+1) * sizeof(bkref);
	cpssize = cps * sizeof(bkref);
	offsets[0] = new bkref[MAXOSISBOOKS+1];
	offsets[1] = new bkref[cpssize];
	buf.setFormatted("%s/%s.bks", refpath.c_str(), name);
	printf("Opening BKS %s\n", buf.c_str());
	fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), O_RDONLY|O_BINARY, true);
	if ((bread = read(fd->getFd(), (void *)offsets[0], bkssize)) == bkssize) {
	}
	else {
		printf ("Error reading RefSys BKS read %d\n", bread);
	}
	FileMgr::getSystemFileMgr()->close(fd);

	buf.setFormatted("%s/%s.cps", refpath.c_str(), name);
	printf("Opening CPS %s\n", buf.c_str());
	fd = FileMgr::getSystemFileMgr()->open(buf.c_str(), O_RDONLY|O_BINARY, true);
	if ((bread = read(fd->getFd(), (void *)offsets[1], cpssize)) == cpssize) {
		retval = new RefSys(name, offsets, offsize);
	}
	else {
		printf ("Error reading RefSys CPS read %d\n", bread);
		if (bread == -1)
			printf("error %d: %s\n", errno, strerror(errno));
	}

	FileMgr::getSystemFileMgr()->close(fd);
	
	return retval;
}

std::list <SWBuf> RefSysMgr::getAvailableRefSys() {
	std::list <SWBuf> retVal;
	for (RefSysMap::iterator it = m_reflist->begin(); it != m_reflist->end(); it++) 
		retVal.push_back((*it).first);

	return retVal;
}




const char *RefSysMgr::getDefaultRefSysName() {
	return defaultRefSysName;
}


void RefSysMgr::setDefaultRefSysName(const char *name) {
	stdstr(&defaultRefSysName, name);
}

SWORD_NAMESPACE_END
