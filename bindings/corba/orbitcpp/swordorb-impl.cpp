#include "sword-impl.hpp"
#include <iostream>
#include <swmgr.h>

/*
char* swordorb::SWModule_impl::helloWorld(const char* greeting) throw(CORBA::SystemException) {
  std::cout << "Server: Greeting was \"" << greeting << "\"" << std::endl;
  return CORBA::string_dup("Hello client, from server!");
}
*/

namespace swordorb {
	ModInfoList *SWMgr_impl::getModInfoList() throw(CORBA::SystemException) {
		ModInfoList *milist = new ModInfoList;
		sword::SWModule *module = 0;
		int size = 0;
		for (sword::ModMap::iterator it = delegate->Modules.begin(); it != delegate->Modules.end(); it++) size++;
		milist->length(size);
		int i = 0;
		for (sword::ModMap::iterator it = delegate->Modules.begin(); it != delegate->Modules.end(); it++) {
			module = it->second;
			(*milist)[i].name = CORBA::string_dup(module->Name());
			(*milist)[i].type = CORBA::string_dup(module->Type());
			(*milist)[i++].lang = CORBA::string_dup(module->Lang());
		}
		return milist;
	}

	SWModule_ptr SWMgr_impl::getModuleByName(const char *name) throw(CORBA::SystemException) {
	}

	StringList *SWMgr_impl::getGlobalOptionsIterator() throw(CORBA::SystemException) {
	}

	StringList *SWMgr_impl::getGlobalOptionValuesIterator(const char *option) throw(CORBA::SystemException) {
	}
}
