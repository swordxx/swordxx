#include "swordorb-impl.hpp"
#include <iostream>
#include <swmgr.h>
#include <versekey.h>

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
		SWModuleMap::iterator it;
		SWModule_ptr retVal;
		it = moduleImpls.find(name);
		if (it == moduleImpls.end()) {
			sword::SWModule *mod = delegate->Modules[name];
			if (mod)
				moduleImpls[name] = new SWModule_impl(mod);
			it = moduleImpls.find(name);
		}
		if (it != moduleImpls.end()) {
			retVal = it->second->_this();
		}
		return ::swordorb::SWModule::_duplicate(retVal);
	}


	StringList *SWMgr_impl::getGlobalOptions() throw(CORBA::SystemException) {
	}

	StringList *SWMgr_impl::getGlobalOptionValues(const char *option) throw(CORBA::SystemException) {
	}

	void SWMgr_impl::terminate() throw(CORBA::SystemException) {
		exit(0);
	}

	StringList *SWModule_impl::search(const char *istr, SearchType searchType, CORBA::Long flags, const char *scope) throw(CORBA::SystemException) {
		int stype = 2;
		sword::ListKey lscope;
		sword::VerseKey parser;
		if (searchType == REGEX) stype = 0;
		if (searchType == PHRASE) stype = -1;
		if (searchType == MULTIWORD) stype = -2;
		sword::ListKey result;

		if ((scope) && (strlen(scope)) > 0) {
			lscope = parser.ParseVerseList(scope, parser, true);
			result = delegate->Search(istr, stype, flags, &lscope);
		}
		else	result = delegate->Search(istr, stype, flags);

		StringList *retVal = new StringList;
		int count = 0;
		for (result = sword::TOP; !result.Error(); result++) count++;
		retVal->length(count);
		int i = 0;
		for (result = sword::TOP; !result.Error(); result++)
			(*retVal)[i++] = CORBA::string_dup((const char *)result);

		return retVal;
	}
		





}
