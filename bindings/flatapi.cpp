/******************************************************************************
 *	swordapi.cpp	- This file contains an api usable by non-C++
 *					environments
 *
 * $Id$
 *
 * Copyright 1998 CrossWire Bible Society (http://www.crosswire.org)
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
 
#include <rawtext.h>
#include <rawcom.h>
#include <rawld.h>
#include <strkey.h>
#include <listkey.h>
#include <versekey.h>
#include <swmgr.h>
#include <markupfiltmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
extern "C" {
#include "flatapi.h"
}

typedef struct {
	ModMap::iterator it;
//	void *it;
	ModMap::iterator end;
//	void *end;
} ModItType;


//-----------------------------------------------------------------
// SWMgr methods
//
extern "C" SWHANDLE SWMgr_new(char filterType) { 
	return (SWHANDLE) new SWMgr(new MarkupFilterMgr(filterType));
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


SWHANDLE SWMgr_getModuleByName(SWHANDLE hmgr, const char *name) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? (SWHANDLE) mgr->Modules[name] : 0;
}


const char *SWMgr_getPrefixPath(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? mgr->prefixPath : 0;
}


const char *SWMgr_getConfigPath(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? mgr->configPath : 0;
}


void SWMgr_setGlobalOption(SWHANDLE hmgr, const char *option, const char *value) {
	SWMgr *mgr = (SWMgr *)hmgr;
	if (mgr)
		mgr->setGlobalOption(option, value);
}


const char *SWMgr_getGlobalOption(SWHANDLE hmgr, const char *option) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? (const char *)mgr->getGlobalOption(option) : 0;
}


const char *SWMgr_getGlobalOptionTip(SWHANDLE hmgr, const char *option) {
	SWMgr *mgr = (SWMgr *)hmgr;
	return (mgr) ? (const char *)mgr->getGlobalOptionTip(option) : 0;
}


// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionsIterator(SWHANDLE hmgr) {
	SWMgr *mgr = (SWMgr *)hmgr;
	static StringList::iterator it;
	static StringList optionslist;
	if (mgr) {
		optionslist = mgr->getGlobalOptions();
		it = optionslist.begin();
	}
	return (SWHANDLE)&it;
}


// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionValuesIterator(SWHANDLE hmgr, const char *option) {
	SWMgr *mgr = (SWMgr *)hmgr;
	static StringList::iterator it;
	
	if (mgr) 
		it = mgr->getGlobalOptionValues(option).begin();
	return (SWHANDLE)&it;
}


void SWMgr_setCipherKey(SWHANDLE hmgr, const char *modName, const char *key) {
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

// SWModule (const const char *imodname = 0, const const char *imoddesc = 0, SWDisplay * idisp = 0, const char *imodtype = 0, SWTextEncoding encoding = ENC_UNKNOWN, SWTextDirection dir = DIRECTION_LTR, SWTextMarkup markup = FMT_UNKNOWN, const char* modlang = 0);
// virtual ~ SWModule ();

SWHANDLE SWModule_doSearch(SWHANDLE hmodule, const char *searchString, int type, int params ,void (*percent) (char, void *), void *percentUserData) {
	
	static ListKey results;
	SWKey * scope = 0;
	SWModule *module = (SWModule *)hmodule;
	if (!module) 	
		return -1;
	
	results.ClearList();
	results = module->Search(searchString, type, params, scope, 0, percent, (void *) &percentUserData);
	
	return (SWHANDLE)&results;
}
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


void SWModule_setKeyText(SWHANDLE hmodule, const char *key) {
	SWModule *module = (SWModule *)hmodule;
	if (module)
		module->setKey(key);
}

//  virtual char setKey (const SWKey &ikey);
//  virtual SWKey & Key () const {
  
const char *SWModule_getKeyText(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (const char *)((module) ? module->KeyText() : 0);
}
  

//  virtual char Display ();
//  virtual SWDisplay *Disp (SWDisplay * idisp = 0);
  
const char *SWModule_getName(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (const char *)((module) ? module->Name() : 0);
}
  

const char *SWModule_getDescription(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (const char *)((module) ? module->Description() : 0);
}


const char *SWModule_getType(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (const char *)((module) ? module->Type() : 0);
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
  
  
const char *SWModule_getStripText(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (const char *)((module) ? module->StripText() : 0);
}
  
  
const char *SWModule_getRenderText(SWHANDLE hmodule) {
	SWModule *module = (SWModule *)hmodule;
	return (const char *)((module) ? module->RenderText() : 0);
}

const char *SWModule_getEntryAttributes(SWHANDLE hmodule, const char *level1, const char *level2, const char *level3) {
	SWModule *module = (SWModule *)hmodule;
	static SWBuf retval;	
	module->RenderText();                 	
	retval = module->getEntryAttributes()[level1][level2][level3].c_str();
	return (retval.length()) ? (const char*)retval.c_str() : NULL;
}

const char *SWModule_getPreverseHeader(SWHANDLE hmodule, const char *key, int pvHeading) {
	SWModule *module = (SWModule *)hmodule;
	static SWBuf preverseHeading;
	char buf[12];	
	sprintf(buf, "%i", pvHeading);  
	module->SetKey(key);	
	module->RenderText();                 	
	preverseHeading = module->getEntryAttributes()["Heading"]["Preverse"][buf].c_str();
	return (preverseHeading.length()) ? (const char*)preverseHeading.c_str() : NULL;
}

const char *SWModule_getFootnoteType(SWHANDLE hmodule, const char *key, const char *note) {
	SWModule *module = (SWModule *)hmodule;
	static SWBuf type;
	module->Error();
	module->SetKey(key);
	module->RenderText();	
	type = module->getEntryAttributes()["Footnote"][note]["type"].c_str();
	return (type) ? (const char*)type.c_str() : NULL;
}

const char *SWModule_getFootnoteBody(SWHANDLE hmodule, const char *key, const char *note) {
	SWModule *module = (SWModule *)hmodule;
	static SWBuf body;
	module->Error();
	module->setKey(key);
	module->RenderText();
	body = module->getEntryAttributes()["Footnote"][note]["body"].c_str();
	SWKey *keybuf = module->getKey();;
	module->renderFilter(body, keybuf);
	return (body) ? (const char*)body.c_str() : NULL;
}

const char *SWModule_getFootnoteRefList(SWHANDLE hmodule, const char *key, const char *note) {
	SWModule *module = (SWModule *)hmodule;
	static SWBuf refList;
	module->Error();
	module->SetKey(key);
	module->RenderText();	
	refList = module->getEntryAttributes()["Footnote"][note]["refList"].c_str();
	return (refList) ? (const char*)refList.c_str() : NULL;
}



SWHANDLE listkey_getVerselistIterator(const char *list, const char *key, const char *v11n) {
	VerseKey versekey;
        versekey.setVersificationSystem(v11n);
	static ListKey verses;
	
	versekey.setText(key);
	verses.ClearList();
	verses = versekey.ParseVerseList(list, versekey);
	return (SWHANDLE)&verses;
}

//-----------------------------------------------------------------
// stringlist_iterator methods

void stringlist_iterator_next(SWHANDLE hsli) {
	StringList::iterator *sli = (StringList::iterator *)hsli;
	(*sli)++;
}


const char *stringlist_iterator_val(SWHANDLE hsli) {
	StringList::iterator *sli = (StringList::iterator *)hsli;
	return (const char *)(*sli)->c_str();
}



//-----------------------------------------------------------------
// listkey_iterator methods

void listkey_iterator_next(SWHANDLE lki) {
	ListKey *lk = (ListKey*)lki;
	(*lk)++;
}


const char *listkey_iterator_val(SWHANDLE lki) {	
	ListKey *lk = (ListKey*)lki;
	if(!lk->Error())
		return (const char *) lk->getText();
	return NULL;
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
