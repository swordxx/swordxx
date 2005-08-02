#include "swordorb-impl.hpp"
#include <iostream>
#include <swmgr.h>
#include <versekey.h>
#include <treekeyidx.h>
#include <swbuf.h>
#include <localemgr.h>

/*
char* swordorb::SWModule_impl::helloWorld(const char* greeting) throw(CORBA::SystemException) {
  std::cout << "Server: Greeting was \"" << greeting << "\"" << std::endl;
  return CORBA::string_dup("Hello client, from server!");
}
*/

using sword::VerseKey;
using sword::SWBuf;
using sword::TreeKeyIdx;

namespace swordorb {

sword::RawText NULLMod("/dev/null", SWNULL, SWNULL);

ModInfoList *SWMgr_impl::getModInfoList() throw(CORBA::SystemException) {

	ModInfoList *milist = new ModInfoList;
	sword::SWModule *module = 0;
	int size = 0;
	for (sword::ModMap::iterator it = delegate->Modules.begin(); it != delegate->Modules.end(); it++) {
		if ((!(it->second->getConfigEntry("CipherKey"))) || (*(it->second->getConfigEntry("CipherKey"))))
			size++;
	}
	milist->length(size);
	int i = 0;
	for (sword::ModMap::iterator it = delegate->Modules.begin(); it != delegate->Modules.end(); it++) {
		module = it->second;
		if ((!(module->getConfigEntry("CipherKey"))) || (*(module->getConfigEntry("CipherKey")))) {
			SWBuf type = module->Type();
			SWBuf cat = module->getConfigEntry("Category");
			if (cat.length() > 0)
				type = cat;
			(*milist)[i].name = CORBA::string_dup(module->Name());
			(*milist)[i].description = CORBA::string_dup(module->Description());
			(*milist)[i].category = CORBA::string_dup(type.c_str());
			(*milist)[i++].language = CORBA::string_dup(module->Lang());
		}
	}
	return milist;
}



SWModule_ptr SWMgr_impl::getModuleByName(const char *name) throw(CORBA::SystemException) {
	SWModuleMap::iterator it;
	SWModule_ptr retVal;
	sword::SWModule *mod = delegate->Modules[name];
	it = moduleImpls.find((mod)?name:SWNULL);
	if (it == moduleImpls.end()) {
		moduleImpls[(mod)?name:SWNULL] = new SWModule_impl((mod)?mod:&NULLMod);
		it = moduleImpls.find((mod)?name:SWNULL);
	}
	if (it != moduleImpls.end()) {
		retVal = it->second->_this();
	}
	return ::swordorb::SWModule::_duplicate(retVal);
}


StringList *SWMgr_impl::getGlobalOptions() throw(CORBA::SystemException) {
	sword::StringList options = delegate->getGlobalOptions();
	StringList *retVal = new StringList;
	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		count++;
	}
	retVal->length(count);
	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		(*retVal)[count++] = CORBA::string_dup(it->c_str());
	}
	return retVal;
}


StringList *SWMgr_impl::getGlobalOptionValues(const char *option) throw(CORBA::SystemException) {
	sword::StringList options = delegate->getGlobalOptionValues(option);
	StringList *retVal = new StringList;
	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		count++;
	}
	retVal->length(count);
	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		(*retVal)[count++] = CORBA::string_dup(it->c_str());
	}
	return retVal;
}

void SWMgr_impl::terminate() throw(CORBA::SystemException) {
	exit(0);
}


CORBA::Boolean SWMgr_impl::testConnection() throw(CORBA::SystemException) {
	return true;
}

void SWMgr_impl::setJavascript(CORBA::Boolean val) throw(CORBA::SystemException) {
	delegate->setJavascript(val);
}

char *SWModule_impl::getCategory() throw(CORBA::SystemException) {
	SWBuf type = delegate->Type();
	SWBuf cat = delegate->getConfigEntry("Category");
	if (cat.length() > 0)
		type = cat;
	return CORBA::string_dup((char *)type.c_str());
}


SearchHitList *SWModule_impl::search(const char *istr, SearchType searchType, CORBA::Long flags, const char *scope) throw(CORBA::SystemException) {
	int stype = 2;
	sword::ListKey lscope;
	sword::VerseKey parser;
	if (searchType == REGEX) stype = 0;
	if (searchType == PHRASE) stype = -1;
	if (searchType == MULTIWORD) stype = -2;
	if (searchType == ENTRYATTR) stype = -3;
	if (searchType == LUCENE) stype = -4;
	sword::ListKey result;

	if ((scope) && (strlen(scope)) > 0) {
		lscope = parser.ParseVerseList(scope, parser, true);
		result = delegate->Search(istr, stype, flags, &lscope);
	}
	else	result = delegate->Search(istr, stype, flags);

	SearchHitList *retVal = new SearchHitList;
	int count = 0;
	for (result = sword::TOP; !result.Error(); result++) count++;
	retVal->length(count);
	int i = 0;
	for (result = sword::TOP; !result.Error(); result++) {
		(*retVal)[i].modName = CORBA::string_dup(delegate->Name());
		(*retVal)[i].key = CORBA::string_dup((const char *)result);
		(*retVal)[i++].score = (long)result.getElement()->userData;
	}

	return retVal;
}


StringList *SWModule_impl::getEntryAttribute(const char *level1, const char *level2, const char *level3) throw(CORBA::SystemException) {
	delegate->RenderText();	// force parse
	sword::AttributeTypeList &entryAttribs = delegate->getEntryAttributes();
	sword::AttributeTypeList::iterator i1 = entryAttribs.find(level1);
	sword::AttributeList::iterator i2;
	sword::AttributeValue::iterator i3, j3;
	StringList *retVal = new StringList;
	int count = 0;

	if (i1 != entryAttribs.end()) {
		i2 = i1->second.find(level2);
		if (i2 != i1->second.end()) {
			i3 = i2->second.find(level3);
			if (i3 != i2->second.end()) {
				for (j3 = i3; j3 != i2->second.end(); j3++)
					count++;
				retVal->length(count);
				count = 0;
				for (j3 = i3; j3 != i2->second.end(); j3++) {
					(*retVal)[count++] = CORBA::string_dup(i3->second.c_str());
				}
			}
		}
	}
	if (!count)
		retVal->length(count);

	return retVal;
}


StringList *SWModule_impl::getKeyChildren() throw(CORBA::SystemException) {
	sword::SWKey *key = delegate->getKey();
	StringList *retVal = new StringList;
	int count = 0;

	sword::VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
	if (vkey) {
		retVal->length(6);
		SWBuf num;
		num.appendFormatted("%d", vkey->Testament());
		(*retVal)[0] = CORBA::string_dup(num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->Book());
		(*retVal)[1] = CORBA::string_dup(num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->Chapter());
		(*retVal)[2] = CORBA::string_dup(num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->Verse());
		(*retVal)[3] = CORBA::string_dup(num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->books[vkey->Testament()-1][vkey->Book()-1].chapmax);
		(*retVal)[4] = CORBA::string_dup(num.c_str());
		num = "";
		num.appendFormatted("%d", vkey->books[vkey->Testament()-1][vkey->Book()-1].versemax[vkey->Chapter()-1]);
		(*retVal)[5] = CORBA::string_dup(num.c_str());
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
			retVal->length(count);
			count = 0;
			if (tkey->firstChild()) {
				do {
					(*retVal)[count++] = CORBA::string_dup(tkey->getLocalName());
				}
				while (tkey->nextSibling());
				tkey->parent();
			}
		}
	}
	return retVal;
}

CORBA::Boolean SWModule_impl::hasKeyChildren() throw(CORBA::SystemException) {
	sword::SWKey *key = delegate->getKey();
	bool retVal = "";

	TreeKeyIdx *tkey = SWDYNAMIC_CAST(TreeKeyIdx, key);
	if (tkey) {
		retVal = tkey->hasChildren();
	}
	return retVal;
}


char *SWModule_impl::getKeyParent() throw(CORBA::SystemException) {
	sword::SWKey *key = delegate->getKey();
	SWBuf retVal = "";

	TreeKeyIdx *tkey = SWDYNAMIC_CAST(TreeKeyIdx, key);
	if (tkey) {
		if (tkey->parent()) {
			retVal = tkey->getFullName();
		}
	}
	return CORBA::string_dup((const char *)retVal);
}


StringList *SWMgr_impl::getAvailableLocales() throw(CORBA::SystemException) {
	sword::StringList localeNames = LocaleMgr::getSystemLocaleMgr()->getAvailableLocales();
	StringList *retVal = new StringList;
	int count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); it++) {
		count++;
	}
	retVal->length(count);
	count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); it++) {
		(*retVal)[count++] = CORBA::string_dup(it->c_str());
	}
	return retVal;
}


void SWMgr_impl::setDefaultLocale(const char *name) throw(CORBA::SystemException) {
	LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(name);
}

}
