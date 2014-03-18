/******************************************************************************
 *
 *  flatapi.cpp -	This file contains an api usable by non-C++
 *			environments
 *
 * $Id$
 *
 * Copyright 2002-2014 CrossWire Bible Society (http://www.crosswire.org)
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

#include <iostream>
#include <vector>
#include <map>

#include <swversion.h>
#include <swmgr.h>
#include <installmgr.h>
#include <versekey.h>
#include <treekeyidx.h>
#include <swbuf.h>
#include <localemgr.h>
#include <utilstr.h>
#include "corba/orbitcpp/webmgr.hpp"

extern "C" {
#include <flatapi.h>
}

using sword::VerseKey;
using sword::SWVersion;
using sword::SWBuf;
using sword::TreeKeyIdx;


#define GETSWMGR(handle, failReturn) HandleSWMgr *hmgr = (HandleSWMgr *)hSWMgr; if (!hmgr) return failReturn; WebMgr *mgr = hmgr->mgr; if (!mgr) return failReturn;

#define GETSWMODULE(handle, failReturn) HandleSWModule *hmod = (HandleSWModule *)hSWModule; if (!hmod) return failReturn; SWModule *module = hmod->mod; if (!module) return failReturn;


namespace {


void clearStringArray(const char ***stringArray) {
	if (*stringArray) {
		for (int i = 0; true; ++i) {
			if ((*stringArray)[i]) {
				delete [] (*stringArray)[i];
			}
			else break;
		}
		free((*stringArray));
		(*stringArray) = 0;
	}
}


class HandleSWModule {
public:
	SWModule *mod;
	char *renderBuf;
	char *stripBuf;
	char *renderHeader;
	char *rawEntry;
	char *configEntry;
	// making searchHits cache static saves memory only having a single
	// outstanding copy, but also is not threadsafe.  Remove static here
	// and fix compiling bugs and add clearSearchHits() to d-tor to change
	static org_crosswire_sword_SearchHit *searchHits;
	static const char **entryAttributes;
	static const char **parseKeyList;
	static const char **keyChildren;

	HandleSWModule(SWModule *mod) {
		this->mod = mod;
		this->renderBuf = 0;
		this->stripBuf = 0;
		this->renderHeader = 0;
		this->rawEntry = 0;
		this->configEntry = 0;
	}
	~HandleSWModule() {
		delete [] renderBuf;
		delete [] stripBuf;
		delete [] renderHeader;
		delete [] rawEntry;
		delete [] configEntry;
	}

	static void clearSearchHits() {
		if (searchHits) {
			for (int i = 0; true; ++i) {
				if (searchHits[i].modName) {
					delete [] searchHits[i].key;
				}
				else break;
			}
			free(searchHits);
			searchHits = 0;
		}
	}
	static void clearEntryAttributes() {
		clearStringArray(&entryAttributes);
	}
	static void clearParseKeyList() {
		clearStringArray(&parseKeyList);
	}
	static void clearKeyChildren() {
		clearStringArray(&keyChildren);
	}
};


class HandleSWMgr {
public:
	WebMgr *mgr;
	org_crosswire_sword_ModInfo *modInfo;
	std::map<SWModule *, HandleSWModule *> moduleHandles;
	SWBuf filterBuf;
	static const char **globalOptions;
	static const char **globalOptionValues;
	static const char **availableLocales;

	HandleSWMgr(WebMgr *mgr) {
		this->mgr = mgr;
		this->modInfo = 0;
	}

	void clearModInfo() {
		if (modInfo) {
			for (int i = 0; true; ++i) {
				if (modInfo[i].name) {
					delete [] modInfo[i].name;
					delete [] modInfo[i].description;
					delete [] modInfo[i].category;
					delete [] modInfo[i].language;
				}
				else break;
			}
			free(modInfo);
			modInfo = 0;
		}
	}

	~HandleSWMgr() {
		clearModInfo();
		for (std::map<SWModule *, HandleSWModule *>::iterator it = moduleHandles.begin(); it != moduleHandles.end(); ++it) {
			delete it->second;
		}
		delete mgr;
	}

	HandleSWModule *getModuleHandle(SWModule *mod) {
		if (!mod) return 0;
		if (moduleHandles.find(mod) == moduleHandles.end()) {
			moduleHandles[mod] = new HandleSWModule(mod);
		}
		return moduleHandles[mod];
	}

	static void clearGlobalOptions() {
		clearStringArray(&globalOptions);
	}

	static void clearGlobalOptionValues() {
		clearStringArray(&globalOptionValues);
	}

	static void clearAvailableLocales() {
		clearStringArray(&availableLocales);
	}
};


org_crosswire_sword_SearchHit *HandleSWModule::searchHits = 0;
const char **HandleSWModule::entryAttributes = 0;
const char **HandleSWModule::parseKeyList = 0;
const char **HandleSWModule::keyChildren = 0;

const char **HandleSWMgr::globalOptions = 0;
const char **HandleSWMgr::globalOptionValues = 0;
const char **HandleSWMgr::availableLocales = 0;

class InitStatics {
public:
	InitStatics() {
// these are redundant with the static declarations above, but ??? doesn't hurt
		HandleSWModule::searchHits = 0;
		HandleSWModule::entryAttributes = 0;
		HandleSWModule::parseKeyList = 0;
		HandleSWModule::keyChildren = 0;

		HandleSWMgr::globalOptions = 0;
		HandleSWMgr::globalOptionValues = 0;
		HandleSWMgr::availableLocales = 0;
	}
	~InitStatics() {
		HandleSWModule::clearSearchHits();
		HandleSWModule::clearEntryAttributes();
		HandleSWModule::clearParseKeyList();
		HandleSWModule::clearKeyChildren();

		HandleSWMgr::clearGlobalOptions();
		HandleSWMgr::clearGlobalOptionValues();
	}
} _initStatics;

}




//
// SWModule methods
//
//

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    terminateSearch
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_SWModule_terminateSearch
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, );

	module->terminateSearch = true;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    search
 * Signature: (Ljava/lang/String;IJLjava/lang/String;Lorg/crosswire/android/sword/SWModule/SearchProgressReporter;)[Lorg/crosswire/android/sword/SWModule/SearchHit;
 */
const struct org_crosswire_sword_SearchHit * SWDLLEXPORT org_crosswire_sword_SWModule_search
  (SWHANDLE hSWModule, const char *searchString, int searchType, long flags, const char *scope, SWHANDLE progressReporter) {

	GETSWMODULE(hSWModule, 0);

	hmod->clearSearchHits();

	sword::ListKey lscope;
	sword::ListKey result;


	if ((scope) && (strlen(scope)) > 0) {
		sword::SWKey *p = module->createKey();
        	sword::VerseKey *parser = SWDYNAMIC_CAST(VerseKey, p);
	        if (!parser) {
        		delete p;
	                parser = new VerseKey();
	        }
	        *parser = module->getKeyText();
		lscope = parser->parseVerseList(scope, *parser, true);
		result = module->search(searchString, searchType, flags, &lscope);
                delete parser;
	}
	else	result = module->search(searchString, searchType, flags);

	int count = 0;
	for (result = sword::TOP; !result.popError(); result++) count++;

	// if we're sorted by score, let's re-sort by verse, because Java can always re-sort by score
	result = sword::TOP;
	if ((count) && (long)result.getElement()->userData)
		result.sort();

	struct org_crosswire_sword_SearchHit *retVal = (struct org_crosswire_sword_SearchHit *)calloc(count+1, sizeof(struct org_crosswire_sword_SearchHit));
	
	int i = 0;
	for (result = sword::TOP; !result.popError(); result++) {
		// don't alloc this; we have a persistent const char * in SWModule we can just reference
		retVal[i].modName = module->getName();
		stdstr(&(retVal[i].key), assureValidUTF8(result.getShortText()));
		retVal[i++].score = (long)result.getElement()->userData;
		// in case we limit count to a max number of hits
		if (i >= count) break;
	}
	hmod->searchHits = retVal;
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    error
 * Signature: ()C
 */
char SWDLLEXPORT org_crosswire_sword_SWModule_popError
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, -1);

	return module->popError();
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getEntrySize
 * Signature: ()J
 */
long SWDLLEXPORT org_crosswire_sword_SWModule_getEntrySize
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	return module->getEntrySize();
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getEntryAttribute
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)[Ljava/lang/String;
 */
const char ** SWDLLEXPORT org_crosswire_sword_SWModule_getEntryAttribute
  (SWHANDLE hSWModule, const char *level1, const char *level2, const char *level3, char filteredBool) {

	GETSWMODULE(hSWModule, 0);

	hmod->clearEntryAttributes();

	module->renderText();	// force parse
	std::vector<SWBuf> results;

	sword::AttributeTypeList &entryAttribs = module->getEntryAttributes();
	sword::AttributeTypeList::iterator i1Start, i1End;
	sword::AttributeList::iterator i2Start, i2End;
	sword::AttributeValue::iterator i3Start, i3End;

	if ((level1) && (*level1)) {
		i1Start = entryAttribs.find(level1);
		i1End = i1Start;
		if (i1End != entryAttribs.end())
			++i1End;
	}
	else {
		i1Start = entryAttribs.begin();
		i1End   = entryAttribs.end();
	}
	for (;i1Start != i1End; ++i1Start) {
		if ((level2) && (*level2)) {
			i2Start = i1Start->second.find(level2);
			i2End = i2Start;
			if (i2End != i1Start->second.end())
				++i2End;
		}
		else {
			i2Start = i1Start->second.begin();
			i2End   = i1Start->second.end();
		}
		for (;i2Start != i2End; ++i2Start) {
			if ((level3) && (*level3)) {
				i3Start = i2Start->second.find(level3);
				i3End = i3Start;
				if (i3End != i2Start->second.end())
					++i3End;
			}
			else {
				i3Start = i2Start->second.begin();
				i3End   = i2Start->second.end();
			}
			for (;i3Start != i3End; ++i3Start) {
				results.push_back(i3Start->second);
			}
			if (i3Start != i3End)
				break;
		}
		if (i2Start != i2End)
			break;
	}

	const char **retVal = (const char **)calloc(results.size()+1, sizeof(const char *));
	for (int i = 0; i < (int)results.size(); i++) {
		if (filteredBool) {
			stdstr((char **)&(retVal[i]), assureValidUTF8(module->renderText(results[i].c_str())));
		}
		else {
			stdstr((char **)&(retVal[i]), assureValidUTF8(results[i].c_str()));
		}
	}

	hmod->entryAttributes = retVal;
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    parseKeyList
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
const char ** SWDLLEXPORT org_crosswire_sword_SWModule_parseKeyList
  (SWHANDLE hSWModule, const char *keyText) {

	GETSWMODULE(hSWModule, 0);

	hmod->clearParseKeyList();

	sword::VerseKey *parser = dynamic_cast<VerseKey *>(module->getKey());
	const char **retVal = 0;
	if (parser) {
		sword::ListKey result;
		result = parser->parseVerseList(keyText, *parser, true);
		int count = 0;
		for (result = sword::TOP; !result.popError(); result++) {
			count++;
		}
		retVal = (const char **)calloc(count+1, sizeof(const char *));
		count = 0;
		for (result = sword::TOP; !result.popError(); result++) {
			stdstr((char **)&(retVal[count++]), assureValidUTF8(VerseKey(result).getOSISRef()));
		}
	}
	else	{
		retVal = (const char **)calloc(2, sizeof(const char *));
		stdstr((char **)&(retVal[0]), assureValidUTF8(keyText));
	}

	hmod->parseKeyList = retVal;
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    setKeyText
 * Signature: (Ljava/lang/String;)V
 */
// Special values handled for VerseKey modules:
//	[+-][book|chapter]	- [de|in]crement by chapter or book
//	(e.g.	"+chapter" will increment the VerseKey 1 chapter)
//	[=][key]		- position absolutely and don't normalize
//	(e.g.	"jn.1.0" for John Chapter 1 intro; "jn.0.0" For Book of John Intro)
void SWDLLEXPORT org_crosswire_sword_SWModule_setKeyText
  (SWHANDLE hSWModule, const char *keyText) {

	GETSWMODULE(hSWModule, );

	sword::SWKey *key = module->getKey();
	sword::VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
	if (vkey) {
		if ((*keyText=='+' || *keyText=='-')) {
			if (!stricmp(keyText+1, "book")) {
				vkey->setBook(vkey->getBook() + ((*keyText=='+')?1:-1));
				return;
			}
			else if (!stricmp(keyText+1, "chapter")) {
				vkey->setChapter(vkey->getChapter() + ((*keyText=='+')?1:-1));
				return;
			}
		}
		else if (*keyText=='=') {
			vkey->setIntros(true);
			vkey->setAutoNormalize(false);
			vkey->setText(keyText+1);
			return;
		}
	}

	module->setKey(keyText);
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getKeyText
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getKeyText
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	return module->getKeyText();
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    hasKeyChildren
 * Signature: ()Z
 */
char SWDLLEXPORT org_crosswire_sword_SWModule_hasKeyChildren
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	sword::SWKey *key = module->getKey();
	char retVal = 0;

	TreeKeyIdx *tkey = SWDYNAMIC_CAST(TreeKeyIdx, key);
	if (tkey) {
		retVal = tkey->hasChildren()?1:0;
	}
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getKeyChildren
 * Signature: ()[Ljava/lang/String;
 */

// This method returns child nodes for a genbook,
// but has special handling if called on a VerseKey module:
//  [0..7] [testament, book, chapter, verse, chapterMax, verseMax, bookName, osisRef]
const char ** SWDLLEXPORT org_crosswire_sword_SWModule_getKeyChildren
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	hmod->clearKeyChildren();

	sword::SWKey *key = module->getKey();
	const char **retVal = 0;
	int count = 0;

	sword::VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
	if (vkey) {
		retVal = (const char **)calloc(9, sizeof(const char *));
		SWBuf num;
		num.appendFormatted("%d", vkey->getTestament());
		stdstr((char **)&(retVal[0]), num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->getBook());
		stdstr((char **)&(retVal[1]), num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->getChapter());
		stdstr((char **)&(retVal[2]), num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->getVerse());
		stdstr((char **)&(retVal[3]), num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->getChapterMax());
		stdstr((char **)&(retVal[4]), num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->getVerseMax());
		stdstr((char **)&(retVal[5]), num.c_str());
		stdstr((char **)&(retVal[6]), vkey->getBookName());
		stdstr((char **)&(retVal[7]), vkey->getOSISRef());
	}
	else {
		TreeKeyIdx *tkey = SWDYNAMIC_CAST(TreeKeyIdx, key);
		if (tkey) {
			if (tkey->firstChild()) {
				do {
					count++;
				}
				while (tkey->nextSibling());
				tkey->parent();
			}
			retVal = (const char **)calloc(count+1, sizeof(const char *));
			count = 0;
			if (tkey->firstChild()) {
				do {
					stdstr((char **)&(retVal[count++]), assureValidUTF8(tkey->getLocalName()));
				}
				while (tkey->nextSibling());
				tkey->parent();
			}
		}
	}

	hmod->keyChildren = retVal;
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getName
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getName
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	return module->getName();
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getDescription
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getDescription
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	return module->getDescription();
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getCategory
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getCategory
  (SWHANDLE hSWModule) {

	static SWBuf type;

	GETSWMODULE(hSWModule, 0);

	type = module->getType();
	SWBuf cat = module->getConfigEntry("Category");
	if (cat.length() > 0)
		type = cat;

	return type.c_str();
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getKeyParent
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getKeyParent
  (SWHANDLE hSWModule) {

	static SWBuf retVal;

	GETSWMODULE(hSWModule, 0);

	sword::SWKey *key = module->getKey();

	retVal = "";

	TreeKeyIdx *tkey = SWDYNAMIC_CAST(TreeKeyIdx, key);
	if (tkey) {
		if (tkey->parent()) {
			retVal = tkey->getText();
		}
	}
	return assureValidUTF8(retVal.c_str());
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    previous
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_SWModule_previous
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, );

	module->decrement();
}


/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    next
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_SWModule_next
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, );

	module->increment();
}


/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    begin
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_SWModule_begin
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, );

	module->setPosition(sword::TOP);
}


/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getStripText
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_stripText
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	stdstr(&(hmod->stripBuf), assureValidUTF8((const char *)module->stripText()));

	return hmod->stripBuf;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getRenderText
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_renderText
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	stdstr(&(hmod->renderBuf), assureValidUTF8((const char *)module->renderText().c_str()));

	return hmod->renderBuf;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getRenderHeader
 * Signature: ()Ljava/lang/String;
 */
// CSS styles associated with this text
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getRenderHeader
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	stdstr(&(hmod->renderHeader), assureValidUTF8(((const char *)(module->getRenderHeader() ? module->getRenderHeader():""))));

	return hmod->renderHeader;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getRawEntry
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getRawEntry
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	stdstr(&(hmod->rawEntry), assureValidUTF8(((const char *)module->getRawEntry())));

	return hmod->rawEntry;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    setRawEntry
 * Signature: (Ljava/lang/String;)V
 */
void SWDLLEXPORT org_crosswire_sword_SWModule_setRawEntry
  (SWHANDLE hSWModule, const char *entryBuffer) {

	GETSWMODULE(hSWModule, );

	module->setEntry(entryBuffer);
}


/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    getConfigEntry
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWModule_getConfigEntry
  (SWHANDLE hSWModule, const char *key) {

	GETSWMODULE(hSWModule, 0);

	stdstr(&(hmod->configEntry), (module->getConfigEntry(key) ? assureValidUTF8(module->getConfigEntry(key)).c_str() : 0));

	return hmod->configEntry;
}

/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    deleteSearchFramework
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_SWModule_deleteSearchFramework
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, );

	module->deleteSearchFramework(); 
}


/*
 * Class:     org_crosswire_sword_SWModule
 * Method:    hasSearchFramework
 * Signature: ()Z
 */
char SWDLLEXPORT org_crosswire_sword_SWModule_hasSearchFramework
  (SWHANDLE hSWModule) {

	GETSWMODULE(hSWModule, 0);

	return (module->hasSearchFramework() && module->isSearchOptimallySupported("God", -4, 0, 0));
}




//
// SWMgr methods
//
//


/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    new
 * Signature: ()V
 */
SWHANDLE SWDLLEXPORT org_crosswire_sword_SWMgr_new() { 
	SWConfig *sysConf = 0;
	return (SWHANDLE) new HandleSWMgr(new WebMgr(sysConf));
}


/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    delete
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_SWMgr_delete(SWHANDLE hSWMgr) {
	HandleSWMgr *hmgr = (HandleSWMgr *)hSWMgr;
	if (hmgr) delete hmgr;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    version
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWMgr_version
  (SWHANDLE hSWMgr) {
	// we don't actually need an SWMgr to get version
	static SWVersion v;
	return v.currentVersion;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getModInfoList
 * Signature: ()[Lorg/crosswire/android/sword/SWMgr/ModInfo;
 */
const struct org_crosswire_sword_ModInfo * SWDLLEXPORT org_crosswire_sword_SWMgr_getModInfoList
  (SWHANDLE hSWMgr) {

	GETSWMGR(hSWMgr, 0);

	sword::SWModule *module = 0;

	hmgr->clearModInfo();

	int size = 0;
	for (sword::ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); ++it) {
		if ((!(it->second->getConfigEntry("CipherKey"))) || (*(it->second->getConfigEntry("CipherKey"))))
			size++;
	}

	struct org_crosswire_sword_ModInfo *milist = (struct org_crosswire_sword_ModInfo *)calloc(size+1, sizeof(struct org_crosswire_sword_ModInfo));
	int i = 0;
	for (sword::ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); ++it) {
		module = it->second;
		if ((!(module->getConfigEntry("CipherKey"))) || (*(module->getConfigEntry("CipherKey")))) {
			SWBuf type = module->getType();
			SWBuf cat = module->getConfigEntry("Category");
			if (cat.length() > 0)
				type = cat;
			stdstr(&(milist[i].name), assureValidUTF8(module->getName()));
			stdstr(&(milist[i].description), assureValidUTF8(module->getDescription()));
			stdstr(&(milist[i].category), assureValidUTF8(type.c_str()));
			stdstr(&(milist[i++].language), assureValidUTF8(module->getLanguage()));
			if (i >= size) break;
		}
	}
	hmgr->modInfo = milist;
	return milist;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getModuleByName
 * Signature: (Ljava/lang/String;)Lorg/crosswire/android/sword/SWModule;
 */
SWHANDLE SWDLLEXPORT org_crosswire_sword_SWMgr_getModuleByName
  (SWHANDLE hSWMgr, const char *moduleName) {

	GETSWMGR(hSWMgr, 0);

	return (SWHANDLE)hmgr->getModuleHandle(hmgr->mgr->getModule(moduleName));
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getPrefixPath
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWMgr_getPrefixPath
  (SWHANDLE hSWMgr) {

	GETSWMGR(hSWMgr, 0);

	return mgr->prefixPath;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getConfigPath
 * Signature: ()Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWMgr_getConfigPath
  (SWHANDLE hSWMgr) {

	GETSWMGR(hSWMgr, 0);

	return mgr->configPath;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    setGlobalOption
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
void SWDLLEXPORT org_crosswire_sword_SWMgr_setGlobalOption
  (SWHANDLE hSWMgr, const char *option, const char *value) {

	GETSWMGR(hSWMgr, );
	mgr->setGlobalOption(option, value);
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getGlobalOption
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWMgr_getGlobalOption
  (SWHANDLE hSWMgr, const char *option) {

	GETSWMGR(hSWMgr, 0);

	return mgr->getGlobalOption(option);
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getGlobalOptionTip
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWMgr_getGlobalOptionTip
  (SWHANDLE hSWMgr, const char *option) {

	GETSWMGR(hSWMgr, 0);

	return mgr->getGlobalOptionTip(option);
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    filterText
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWMgr_filterText
  (SWHANDLE hSWMgr, const char *filterName, const char *text) {

	GETSWMGR(hSWMgr, 0);

	hmgr->filterBuf = text;

// why was this in bindings/corba/omniorb?
//	mgr->setGlobalOption("Greek Accents", "Off");

	char errStatus = mgr->filterText(filterName, hmgr->filterBuf);
	(void)errStatus;
	return hmgr->filterBuf.c_str();
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getGlobalOptions
 * Signature: ()[Ljava/lang/String;
 */
const char ** SWDLLEXPORT org_crosswire_sword_SWMgr_getGlobalOptions
  (SWHANDLE hSWMgr) {

	GETSWMGR(hSWMgr, 0);

	const char **retVal;
	hmgr->clearGlobalOptions();

	sword::StringList options = mgr->getGlobalOptions();
	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		count++;
	}
	retVal = (const char **)calloc(count+1, sizeof(const char *));
	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		stdstr((char **)&(retVal[count++]), it->c_str());
	}

	hmgr->globalOptions = retVal;
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getGlobalOptionValues
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
const char ** SWDLLEXPORT org_crosswire_sword_SWMgr_getGlobalOptionValues
  (SWHANDLE hSWMgr, const char *option) {

	GETSWMGR(hSWMgr, 0);

	const char **retVal = 0;
	hmgr->clearGlobalOptionValues();

	sword::StringList options = mgr->getGlobalOptionValues(option);
	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		count++;
	}
	retVal = (const char **)calloc(count+1, sizeof(const char *));
	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		stdstr((char **)&(retVal[count++]), it->c_str());
	}

	hmgr->globalOptionValues = retVal;
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    setCipherKey
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
void SWDLLEXPORT org_crosswire_sword_SWMgr_setCipherKey
  (SWHANDLE hSWMgr, const char *modName, const char *key) {

	GETSWMGR(hSWMgr, );

	mgr->setCipherKey(modName, key);
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    setJavascript
 * Signature: (Z)V
 */
void SWDLLEXPORT org_crosswire_sword_SWMgr_setJavascript
  (SWHANDLE hSWMgr, char valueBool) {

	GETSWMGR(hSWMgr, );

	mgr->setJavascript(valueBool);
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    getAvailableLocales
 * Signature: ()[Ljava/lang/String;
 */
const char ** SWDLLEXPORT org_crosswire_sword_SWMgr_getAvailableLocales
  (SWHANDLE hSWMgr) {

	GETSWMGR(hSWMgr, 0);

	hmgr->clearAvailableLocales();
	sword::StringList localeNames = LocaleMgr::getSystemLocaleMgr()->getAvailableLocales();
	const char **retVal = 0;
	int count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); ++it) {
		count++;
	}
	retVal = (const char **)calloc(count+1, sizeof(const char *));
	count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); ++it) {
		stdstr((char **)&(retVal[count++]), it->c_str());
	}

	hmgr->availableLocales = retVal;
	return retVal;
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    setDefaultLocale
 * Signature: (Ljava/lang/String;)V
 */
void SWDLLEXPORT org_crosswire_sword_SWMgr_setDefaultLocale
  (SWHANDLE hSWMgr, const char *name) {

	// we don't actually need an SWMgr instance for this
	GETSWMGR(hSWMgr, );

	LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(name);
}

/*
 * Class:     org_crosswire_sword_SWMgr
 * Method:    translate
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
const char * SWDLLEXPORT org_crosswire_sword_SWMgr_translate
  (SWHANDLE hSWMgr, const char *text, const char *localeName) {

	GETSWMGR(hSWMgr, 0);

	return LocaleMgr::getSystemLocaleMgr()->translate(text, localeName);
}




//
// InstallMgr methods
//
//


/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    reInit
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_InstallMgr_reInit
  (SWHANDLE hInstallMgr) {}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    setUserDisclaimerConfirmed
 * Signature: ()V
 */
void SWDLLEXPORT org_crosswire_sword_InstallMgr_setUserDisclaimerConfirmed
  (SWHANDLE hInstallMgr) {}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    syncConfig
 * Signature: ()I
 */
int SWDLLEXPORT org_crosswire_sword_InstallMgr_syncConfig
  (SWHANDLE hInstallMgr) {

	return 0;
}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    uninstallModule
 * Signature: (Ljava/lang/String;)I
 */
int SWDLLEXPORT org_crosswire_sword_InstallMgr_uninstallModule
  (SWHANDLE hInstallMgr, const char *) {

	return 0;
}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    getRemoteSources
 * Signature: ()[Ljava/lang/String;
 */
const char ** SWDLLEXPORT org_crosswire_sword_InstallMgr_getRemoteSources
  (SWHANDLE hInstallMgr) {

	return 0;
}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    refreshRemoteSource
 * Signature: (Ljava/lang/String;)I
 */
int SWDLLEXPORT org_crosswire_sword_InstallMgr_refreshRemoteSource
  (SWHANDLE hInstallMgr, const char *) {

	return 0;
}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    getRemoteModInfoList
 * Signature: (Ljava/lang/String;)[Lorg/crosswire/android/sword/SWMgr/ModInfo;
 */
const struct org_crosswire_sword_ModInfo * SWDLLEXPORT org_crosswire_sword_InstallMgr_getRemoteModInfoList
  (SWHANDLE hInstallMgr, const char *) {

	return 0;
}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    remoteInstallModule
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
int SWDLLEXPORT org_crosswire_sword_InstallMgr_remoteInstallModule
  (SWHANDLE hInstallMgr, const char *, const char *) {

	return 0;
}

/*
 * Class:     org_crosswire_sword_InstallMgr
 * Method:    getRemoteModuleByName
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Lorg/crosswire/android/sword/SWModule;
 */
SWHANDLE SWDLLEXPORT org_crosswire_sword_InstallMgr_getRemoteModuleByName
  (SWHANDLE hInstallMgr, const char *, const char *) {

	return 0;
}
