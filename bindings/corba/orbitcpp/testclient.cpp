/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: f; c-basic-offset: 4 -*- */

#include "swordorb-cpp-stubs.h"
#include "swordorb-cpp-common.h"
#include <iostream>
	
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

		swordorb::ModInfoList *modInfoList;

		std::cout << "PrefixPath: " << mgr->getPrefixPath() << "\n";
		std::cout << "ConfigPath: " << mgr->getConfigPath() << "\n";
		modInfoList = mgr->getModInfoList();
		std::cout << "sequence length: " << modInfoList->length() << "\n";
		swordorb::SWModule_ptr module;
		for (int i = 0; i < modInfoList->length(); i++) {
			std::cout << (*modInfoList)[i].name << ": " << (*modInfoList)[i].type << ": " << (*modInfoList)[i].lang << "\n";
			module = mgr->getModuleByName((*modInfoList)[i].name);
			module->setKeyText("jas1:19");
			std::cout << module->getRenderText() << "\n";
		}
		
	} catch(const CORBA::Exception& ex) {
		std::cout << "Exception caught. Maybe the server is not running, or the IOR is wrong." << std::endl;
	}
	
	return 0;
}
