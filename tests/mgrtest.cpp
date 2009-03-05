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
#include <swlog.h>
#include <iostream>
#include <versekey.h>
#include <swmodule.h>
#include <swconfig.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	std::cerr << "\n";
	SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);
	SWConfig *sysConf = 0;
	if (argc > 1) {
		sysConf = new SWConfig(argv[1]);
	}
	SWMgr mymgr(0, sysConf);
	std::cerr << "\n\nprefixPath: " << mymgr.prefixPath;
	std::cerr << "\nconfigPath: " << mymgr.configPath << "\n\n";



	ModMap::iterator it;

	for (it = mymgr.Modules.begin(); it != mymgr.Modules.end(); it++) {
		std::cout << "[" << (*it).second->Name() << "] (Writable: " << (it->second->isWritable()?"Yes":"No") << ") [" << (*it).second->Description() << "]\n";
		std::cout << "AbsoluteDataPath = " << it->second->getConfigEntry("AbsoluteDataPath") << "\n";
		std::cout << "Has Feature HebrewDef = " << it->second->getConfig().has("Feature", "HebrewDef") << "\n";
		if ((!strcmp((*it).second->Type(), "Biblical Texts")) || (!strcmp((*it).second->Type(), "Commentaries"))) {
			it->second->setKey("James 1:19");
			std::cout << (const char *) *(*it).second << "\n\n";
		}
	}

	SWModule *mhc = mymgr.Modules["MHC"];

	if (mhc) {
		std::cout << "MHC, Lang = " << mhc->Lang() << "\n\n";
		for (mhc->Key("Gen 1:1"); mhc->Key() < (VerseKey) "Gen 1:10"; (*mhc)++)
			std::cout << (const char *) *mhc << "\n";
	}

	if (sysConf)
		delete sysConf;

	return 0;
}
