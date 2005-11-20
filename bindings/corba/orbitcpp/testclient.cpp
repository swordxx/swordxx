/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */

#include "swordorb-cpp-stubs.h"
#include "swordorb-cpp-common.h"
#include <iostream>
#include <swbuf.h>
	
int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "Usage:" << std::endl
				  << "  " << argv[0] << " IOR" << std::endl
				  << std::endl;
		return -1;
	}
	
	try
	{
		// Initialize ORBit
		CORBA::ORB_ptr orb = CORBA::ORB_init(argc, argv, "orbit-local-orb");

		// Get a reference to the server from the IOR passed on the
		// command line
		CORBA::Object_var obj = orb->string_to_object(argv[1]);
		swordorb::SWMgr_var mgr = swordorb::SWMgr::_narrow(obj);

		swordorb::SWModule_ptr module;
		swordorb::ModInfoList *modInfoList;

		std::cout << "Connected: "  << mgr->testConnection() << "\n";
		std::cout << "PrefixPath: " << mgr->getPrefixPath() << "\n";
		std::cout << "ConfigPath: " << mgr->getConfigPath() << "\n";
//		modInfoList = mgr->getModInfoList();
//		std::cout << "sequence length: " << modInfoList->length() << "\n";
/*
		for (int i = 0; i < modInfoList->length(); i++) {
			std::cout << (*modInfoList)[i].name << ": " << (*modInfoList)[i].category << ": " << (*modInfoList)[i].language << "\n";
			if (!strncmp((*modInfoList)[i].category, "Bibl", 4)) {
				module = mgr->getModuleByName((*modInfoList)[i].name);
				module->setKeyText("jas1:19");
				std::cout << module->getRenderText() << "\n";
			}
			std::cout << "\n";
		}
*/
		swordorb::StringList *localeNames = mgr->getAvailableLocales();
		for (int i = 0; i < localeNames->length(); i++) {
			std::cout << (*localeNames)[i] << "\n";
		}
		mgr->setDefaultLocale("de");
		mgr->setJavascript(true);
		mgr->setGlobalOption("Textual Variants", "Secondary Reading");
		module = mgr->getModuleByName("LXX");
		for (module->setKeyText("is.53.4"); !module->error(); module->next()) {
			std::cout << "KeyText: " << module->getKeyText() << "\n";
			std::cout << "Text: " << module->getRenderText() << "\n";
			break;
		}
/*
		swordorb::SearchHitList *searchResults;
		bool lucene = module->hasSearchFramework();
		searchResults = module->search("God love world", (lucene)?swordorb::LUCENE:swordorb::MULTIWORD, 0, "");
		for (int i = 0; i < searchResults->length(); i++) {
			std::cout << (*searchResults)[i].key << "\n";
		}
*/

		mgr->setGlobalOption("Greek Accents", "Off");
		std::cout << "\nFiltered text: " << mgr->filterText("Greek Accents", "ὁ θεὸς") << "\n";

		
	} catch(const CORBA::Exception& ex) {
		std::cout << "Exception caught. Maybe the server is not running, or the IOR is wrong." << std::endl;
	}
	
	return 0;
}
