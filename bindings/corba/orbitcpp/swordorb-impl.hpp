/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t;c-basic-offset: 4 -*- */
#ifndef _ORBIT_CPP_IDL_sword_IMPL_HH
#define _ORBIT_CPP_IDL_sword_IMPL_HH

#include "swordorb-cpp-skels.h"
#include <swmodule.h>
#include <swmgr.h>


namespace swordorb {

//Inherit from abstract Skeleton:
class SWModule_impl : public POA_swordorb::SWModule {
	sword::SWModule *delegate;
public:
	SWModule_impl(sword::SWModule *delegate)  { this->delegate = delegate; }

	void   terminateSearch() throw(CORBA::SystemException) { delegate->terminateSearch = true; }
	char   error() throw(CORBA::SystemException) { return delegate->Error(); }
	CORBA::Long getEntrySize() throw(CORBA::SystemException) { return delegate->getEntrySize(); }
	void   setKeyText(const char *key) throw(CORBA::SystemException) { delegate->KeyText(key); }
	char *getKeyText() throw(CORBA::SystemException) { return (char *)delegate->KeyText(); }
	char *getName() throw(CORBA::SystemException) { return (char *)delegate->Name(); }
	char *getDescription() throw(CORBA::SystemException) { return (char *)delegate->Description(); }
	char *getType() throw(CORBA::SystemException) { return (char *)delegate->Type(); }
	void   previous() throw(CORBA::SystemException) { delegate->decrement(); }
	void   next() throw(CORBA::SystemException) { delegate->increment(); }
	void   begin() throw(CORBA::SystemException) { delegate->setPosition(sword::TOP); }
	char *getStripText() throw(CORBA::SystemException) { return (char *)delegate->StripText(); }
	char *getRenderText() throw(CORBA::SystemException) { return (char *)delegate->RenderText(); }
};

class SWMgr_impl : public POA_swordorb::SWMgr {
	sword::SWMgr *delegate;
public:
	SWMgr_impl(sword::SWMgr *delegate)  { this->delegate = delegate; }

	ModInfoList *getModInfoList() throw(CORBA::SystemException);
	SWModule_ptr getModuleByName(const char *name) throw(CORBA::SystemException);
	char *getPrefixPath() throw(CORBA::SystemException) { return delegate->prefixPath; }
	char *getConfigPath() throw(CORBA::SystemException) { return delegate->configPath; }
	void  setGlobalOption(const char *option, const char *value) throw(CORBA::SystemException) { delegate->setGlobalOption(option, value); }
	char *getGlobalOption(const char *option) throw(CORBA::SystemException) { return (char *)delegate->getGlobalOption(option); }
	char *getGlobalOptionTip(const char *option) throw(CORBA::SystemException) { return (char *)delegate->getGlobalOptionTip(option); }
	StringList *getGlobalOptionsIterator() throw(CORBA::SystemException);
	StringList *getGlobalOptionValuesIterator(const char *option) throw(CORBA::SystemException);
	void     setCipherKey(const char *modName, const char *key) throw(CORBA::SystemException) { delegate->setCipherKey(modName, key); }
	void     terminate() throw(CORBA::SystemException);

};
}; // namespace hellomodule


#endif //_ORBIT_CPP_IDL_helloworld_IMPL_HH
