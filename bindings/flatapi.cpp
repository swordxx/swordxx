/******************************************************************************
 *	swordapi.cpp	- This file contains an api usable by non-C++ windows
 *					environments
 */

#include <string.h>
#include <stdio.h>

#include <rawtext.h>
#include <rawcom.h>
#include <rawld.h>
#include <strkey.h>
#include <listkey.h>
#include <versekey.h>
#include <swmgr.h>
#include <markupfiltmgr.h>

#include "flatapi.h"


//-----------------------------------------------------------------
// SWMgr methods
//
extern "C" SWHANDLE SWMgr_new() {
	return (SWHANDLE) new SWMgr(new MarkupFilterMgr());
}


// SWConfig *, SWConfig *, bool, SWFilterMgr *
SWHANDLE SWMgr_newEx(SWHANDLE hiconfig, SWHANDLE hisysconfig, char autoload, SWHANDLE hfilterMgr) {
	SWConfig *iconfig = (SWConfig *)hiconfig;
	SWConfig *isysconfig = (SWConfig *)hisysconfig;
	SWFilterMgr *filterMgr = (SWFilterMgr *)hfilterMgr;
	
	return (SWHANDLE) new SWMgr(iconfig, isysconfig, autoload, filterMgr);
}


void SWMgr_delete(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	if (mgr)
		delete mgr;
}


SWHANDLE SWMgr_getConfig(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? (SWHANDLE)mgr->config : 0;
}


SWHANDLE SWMgr_getModulesIterator(SWHANDLE hmgr) {
	static ModItType it;

	SWMgr *mgr = (SWMgr *)hmgr;
	if (mgr) {
	    it.it = mgr->Modules.begin();
	    it.end = mgr->Modules.end();
	}
	return (SWHANDLE)&it;
}


SWHANDLE SWMgr_getModuleByName(SWHANDLE hmgr, char *name) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? (SWHANDLE) mgr->Modules[name] : 0;
}


char *SWMgr_getPrefixPath(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? mgr->prefixPath : 0;
}


char *SWMgr_getConfigPath(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? mgr->configPath : 0;
}


void SWMgr_setGlobalOption(SWHANDLE hmgr, char *option, char *value) {
	SWMgr *mgr = (SWMgr *)hmgr;
	if (mgr)
		mgr->setGlobalOption(option, value);
}


char *SWMgr_getGlobalOption(SWHANDLE hmgr, char *option) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? (char *)mgr->getGlobalOption(option) : 0;
}


char *SWMgr_getGlobalOptionTip(SWHANDLE hmgr, char *option) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? (char *)mgr->getGlobalOptionTip(option) : 0;
}


// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionsIterator(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	static OptionsList::iterator it;
	
	if (mgr) 
		it = mgr->getGlobalOptions().begin();
	return (SWHANDLE)&it;
}


// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionValuesIterator(SWHANDLE hmgr, char *option) {
	SWMgr *mgr = (SWMgr *)hmgr;
	static OptionsList::iterator it;
	
	if (mgr) 
		it = mgr->getGlobalOptionValues(option).begin();
	return (SWHANDLE)&it;
}


void SWMgr_setCipherKey(SWHANDLE hmgr, char *modName, char *key) {
	SWMgr *mgr = (SWMgr *)hmgr;
	if (mgr)
		mgr->setCipherKey(modName, key);
}


//-----------------------------------------------------------------
// SWModule methods

// static void nullPercent (char percent, void *userData);
void SWModule_terminateSearch(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	if (module)
		module->terminateSearch = true;
}

// SWModule (const char *imodname = 0, const char *imoddesc = 0, SWDisplay * idisp = 0, char *imodtype = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN, const char* modlang = 0);
// virtual ~ SWModule ();

  /** Gets and clears error status
  *
  * @return error status
  */
char SWModule_error(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (module) ? module->Error() : 0;
}


int SWModule_getEntrySize(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (module) ? module->getEntrySize() : 0;
}


void SWModule_setKeyText(SWHANDLE hmodule, char *key) {
	SWModule *module = (SWModule *)hmodule;
	if (module)
		module->Key(key);
}

//  virtual char SetKey (const SWKey &ikey);
//  virtual SWKey & Key () const {
  
char *SWModule_getKeyText(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (char *)((module) ? module->KeyText() : 0);
}
  

//  virtual char Display ();
//  virtual SWDisplay *Disp (SWDisplay * idisp = 0);
  
char *SWModule_getName(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (char *)((module) ? module->Name() : 0);
}
  

char *SWModule_getDescription(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (char *)((module) ? module->Description() : 0);
}


char *SWModule_getType(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (char *)((module) ? module->Type() : 0);
}


void SWModule_previous(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	if (module)
		(*module)--;
}


void SWModule_next(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	if (module)
		(*module)++;
}


void SWModule_begin(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	if (module)
		(*module) = TOP;
}
  
  
char *SWModule_getStripText(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (char *)((module) ? module->StripText() : 0);
}
  
  
char *SWModule_getRenderText(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (char *)((module) ? module->RenderText() : 0);
}



//-----------------------------------------------------------------
// stringlist_iterator methods

void stringlist_iterator_next(SWHANDLE hsli) {
	OptionsList::iterator *sli = (OptionsList::iterator *)hsli;
	(*sli)++;
}


char *stringlist_iterator_val(SWHANDLE hsli) {
	OptionsList::iterator *sli = (OptionsList::iterator *)hsli;
	return (char *)(*sli)->c_str();
}



//-----------------------------------------------------------------
// modmap methods

void ModList_iterator_next(SWHANDLE hmmi) {
	ModItType *it  = (ModItType *)hmmi;
	if (it->it != it->end)
		it->it++;
}


SWHANDLE ModList_iterator_val(SWHANDLE hmmi) {
	ModItType *it  = (ModItType *)hmmi;
	return (it->it != it->end) ? (SWHANDLE)it->it->second : 0;
}

