/******************************************************************************
 *
 *  swordstub.cpp -	JNI bindings
 *
 * $Id$
 *
 * Copyright 2009-2013 CrossWire Bible Society (http://www.crosswire.org)
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

#include <jni.h>
#include <android/log.h>

#include <utilstr.h>
#include <swversion.h>
#include <swmgr.h>
#include <swlog.h>
#include <filemgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <localemgr.h>
#include <treekeyidx.h>
#include <installmgr.h>

#include "webmgr.hpp"
#include "org_crosswire_android_sword_SWMgr.h"
#include "org_crosswire_android_sword_SWModule.h"
#include "org_crosswire_android_sword_InstallMgr.h"


using std::cerr;
using std::map;
using std::vector;

using namespace sword;


WebMgr *mgr = 0;
InstallMgr *installMgr = 0;

class AndroidLogger : public SWLog {
	vector<int> levelMapping;
public:
	AndroidLogger() {
		levelMapping.resize(10, 0);
		levelMapping[SWLog::LOG_ERROR] = ANDROID_LOG_ERROR;
		levelMapping[SWLog::LOG_WARN] = ANDROID_LOG_WARN;
		levelMapping[SWLog::LOG_INFO] = ANDROID_LOG_INFO;
		levelMapping[SWLog::LOG_TIMEDINFO] = ANDROID_LOG_INFO;
		levelMapping[SWLog::LOG_DEBUG] = ANDROID_LOG_DEBUG;
	}
	virtual void logMessage(const char *message, int level) const {
		SWBuf msg = message;
		if (msg.size() > 512) msg.setSize(512);
		__android_log_write(levelMapping[level], "libsword.so", msg.c_str());
	}
};


static void init() {
	if (!mgr) {
		SWLog::setSystemLog(new AndroidLogger());
		SWLog::getSystemLog()->setLogLevel(SWLog::LOG_DEBUG);
		SWBuf baseDir  = "/sdcard/sword";
		SWBuf confPath = baseDir + "/mods.d/globals.conf";
		// be sure we have at least some config file already out there
		if (!FileMgr::existsFile(confPath.c_str())) {
			FileMgr::createParent(confPath.c_str());
			remove(confPath.c_str());

			SWConfig config(confPath.c_str());
			config["Globals"]["HiAndroid"] = "weeee";
			config.Save();
		}
		mgr = new WebMgr("/sdcard/sword");
	}
}


static void initInstall() {

	if (!installMgr) {
		SWBuf baseDir  = "/sdcard/sword/InstallMgr";
		SWBuf confPath = baseDir + "/InstallMgr.conf";
		// be sure we have at least some config file already out there
		if (!FileMgr::existsFile(confPath.c_str())) {
			FileMgr::createParent(confPath.c_str());
			remove(confPath.c_str());

			SWConfig config(confPath.c_str());
			config["General"]["PassiveFTP"] = "true";
			config.Save();
		}
		installMgr = new InstallMgr(baseDir);
	}
}



JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_version
  (JNIEnv *env, jobject me) {

	init();

	SWVersion v;
	return env->NewStringUTF(v.currentVersion);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    reInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_reInit
  (JNIEnv *, jobject) {

	delete mgr;
	mgr = 0;
}


JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_getPrefixPath
  (JNIEnv *env, jobject me) {

	init();

	return env->NewStringUTF(mgr->prefixPath);
}

JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_getConfigPath
  (JNIEnv *env, jobject me) {

	init();

	return env->NewStringUTF(mgr->configPath);
}


JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getModInfoList
  (JNIEnv *env, jobject) {

	init();

	int size = 0;
	for (sword::ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); ++it) {
		if ((!(it->second->getConfigEntry("CipherKey"))) || (*(it->second->getConfigEntry("CipherKey"))))
			size++;
	}

SWLog::getSystemLog()->logDebug("getModInfoList returning %d length array\n", size);

	jclass clazzModInfo = env->FindClass("org/crosswire/android/sword/SWMgr$ModInfo");
	jfieldID nameID     = env->GetFieldID(clazzModInfo, "name",        "Ljava/lang/String;");
	jfieldID descID     = env->GetFieldID(clazzModInfo, "description", "Ljava/lang/String;");
	jfieldID catID      = env->GetFieldID(clazzModInfo, "category",    "Ljava/lang/String;");
	jfieldID langID     = env->GetFieldID(clazzModInfo, "language",    "Ljava/lang/String;");
	jfieldID versionID  = env->GetFieldID(clazzModInfo, "version",     "Ljava/lang/String;");
	jfieldID deltaID    = env->GetFieldID(clazzModInfo, "delta",       "Ljava/lang/String;");

	jobjectArray ret = (jobjectArray) env->NewObjectArray(size, clazzModInfo, NULL);

	int i = 0;
	for (sword::ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); ++it) {
		SWModule *module = it->second;

		if ((!(module->getConfigEntry("CipherKey"))) || (*(module->getConfigEntry("CipherKey")))) {
			SWBuf type = module->getType();
			SWBuf cat = module->getConfigEntry("Category");
			SWBuf version = module->getConfigEntry("Version");
			if (cat.length() > 0) type = cat;

			jobject modInfo = env->AllocObject(clazzModInfo); 

			jstring val;
			val = env->NewStringUTF(assureValidUTF8(module->getName()));        env->SetObjectField(modInfo, nameID   , val); env->DeleteLocalRef(val);
			val = env->NewStringUTF(assureValidUTF8(module->getDescription())); env->SetObjectField(modInfo, descID   , val); env->DeleteLocalRef(val);
			val = env->NewStringUTF(assureValidUTF8(type.c_str()));          env->SetObjectField(modInfo, catID    , val); env->DeleteLocalRef(val);
			val = env->NewStringUTF(assureValidUTF8(module->getLanguage()));        env->SetObjectField(modInfo, langID   , val); env->DeleteLocalRef(val);
			val = env->NewStringUTF(assureValidUTF8(version.c_str()));       env->SetObjectField(modInfo, versionID, val); env->DeleteLocalRef(val);
			val = env->NewStringUTF(assureValidUTF8(""));                    env->SetObjectField(modInfo, deltaID  , val); env->DeleteLocalRef(val);

			env->SetObjectArrayElement(ret, i++, modInfo);

			env->DeleteLocalRef(modInfo);

		}
	}
	return ret;
}

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getModuleByName
 * Signature: (Ljava/lang/String;)Lorg/crosswire/android/sword/SWModule;
 */
JNIEXPORT jobject JNICALL Java_org_crosswire_android_sword_SWMgr_getModuleByName
  (JNIEnv *env, jobject me, jstring modNameJS) {

	init();

	jobject retVal = 0;

     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
	sword::SWModule *module = mgr->getModule(modName);
     env->ReleaseStringUTFChars(modNameJS, modName);

	if (module) {
		SWBuf type = module->getType();
		SWBuf cat = module->getConfigEntry("Category");
		if (cat.length() > 0) type = cat;
		jfieldID fieldID;
		jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
		retVal = env->AllocObject(clazzSWModule); 
		fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(assureValidUTF8(module->getName())));
		fieldID = env->GetFieldID(clazzSWModule, "description", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(assureValidUTF8(module->getDescription())));
		fieldID = env->GetFieldID(clazzSWModule, "category", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(assureValidUTF8(type.c_str())));
	}
	return retVal;
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setGlobalOption
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setGlobalOption
  (JNIEnv *env, jobject me, jstring optionJS, jstring valueJS) {

	init();

     const char *option = env->GetStringUTFChars(optionJS, NULL);
     const char *value  = env->GetStringUTFChars(valueJS, NULL);

	mgr->setGlobalOption(option, value);

     env->ReleaseStringUTFChars(valueJS, value);
     env->ReleaseStringUTFChars(optionJS, option);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOption
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOption
  (JNIEnv *env, jobject me, jstring optionJS) {

	init();

     const char *option = env->GetStringUTFChars(optionJS, NULL);

	SWBuf value = mgr->getGlobalOption(option);

     env->ReleaseStringUTFChars(optionJS, option);

	return env->NewStringUTF(assureValidUTF8(value));
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptionTip
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptionTip
  (JNIEnv *env, jobject me, jstring optionJS) {

	init();

     const char *option = env->GetStringUTFChars(optionJS, NULL);

	SWBuf value = mgr->getGlobalOptionTip(option);

     env->ReleaseStringUTFChars(optionJS, option);

	return env->NewStringUTF(assureValidUTF8(value));
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    filterText
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_filterText
  (JNIEnv *env, jobject me, jstring filterNameJS, jstring textJS) {

	init();

     const char *filterName = env->GetStringUTFChars(filterNameJS, NULL);
     const char *text  = env->GetStringUTFChars(textJS, NULL);

	SWBuf buf = text;
	// hmmm, in the future, provide a param to specify filter value maybe?
	mgr->setGlobalOption("Greek Accents", "Off");
	char errStatus = mgr->filterText(filterName, buf);

     env->ReleaseStringUTFChars(textJS, text);
     env->ReleaseStringUTFChars(filterNameJS, filterName);

	return env->NewStringUTF(assureValidUTF8(buf));
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptions
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptions
  (JNIEnv *env, jobject me) {

	init();

	sword::StringList options = mgr->getGlobalOptions();
	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		count++;
	}

	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		env->SetObjectArrayElement(ret, count++, env->NewStringUTF(assureValidUTF8(*it)));
	}

	return ret;
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptionValues
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptionValues
  (JNIEnv *env, jobject me, jstring optionJS) {

	init();

     const char *option = env->GetStringUTFChars(optionJS, NULL);

	sword::StringList options = mgr->getGlobalOptionValues(option);

     env->ReleaseStringUTFChars(optionJS, option);

	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		count++;
	}
	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); ++it) {
		env->SetObjectArrayElement(ret, count++, env->NewStringUTF(assureValidUTF8(*it)));
	}

	return ret;
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setCipherKey
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setCipherKey
  (JNIEnv *env, jobject me , jstring modNameJS, jstring keyJS) {

	init();

     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
     const char *key     = env->GetStringUTFChars(keyJS, NULL);

	mgr->setCipherKey(modName, key);

     env->ReleaseStringUTFChars(keyJS, key);
     env->ReleaseStringUTFChars(modNameJS, modName);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setJavascript
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setJavascript
  (JNIEnv *env, jobject me, jboolean val) {

	init();

	mgr->setJavascript(val == JNI_TRUE);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getAvailableLocales
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getAvailableLocales
  (JNIEnv *env, jobject me) {

	init();

	sword::StringList localeNames = LocaleMgr::getSystemLocaleMgr()->getAvailableLocales();
	int count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); ++it) {
		count++;
	}

	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

	count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); ++it) {
		env->SetObjectArrayElement(ret, count++, env->NewStringUTF(assureValidUTF8(*it)));
	}
	return ret;
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setDefaultLocale
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setDefaultLocale
  (JNIEnv *env, jobject me, jstring localeNameJS) {

	init();

     const char *localeName = env->GetStringUTFChars(localeNameJS, NULL);

	LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(localeName);

     env->ReleaseStringUTFChars(localeNameJS, localeName);
}



// SWModule methods ----------------------------------------------------------------------------------


SWModule *getModule(JNIEnv *env, jobject me) {

	init(); 

	SWModule *module = 0;
	jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
	jfieldID fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;");
	jfieldID sourceFieldID = env->GetFieldID(clazzSWModule, "remoteSourceName", "Ljava/lang/String;");
	jstring modNameJS = (jstring)env->GetObjectField(me, fieldID);
	jstring sourceNameJS = (jstring)env->GetObjectField(me, sourceFieldID);
     const char *modName = (modNameJS?env->GetStringUTFChars(modNameJS, NULL):0);
     const char *sourceName = (sourceNameJS?env->GetStringUTFChars(sourceNameJS, NULL):0);
	if (sourceName && *sourceName) {
		initInstall();
		InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
		if (source == installMgr->sources.end()) {
			SWMgr *mgr = source->second->getMgr();
			module = mgr->getModule(modName);
		}
	}
	else module = mgr->getModule(modName);
     if (modName) env->ReleaseStringUTFChars(modNameJS, modName);
     if (sourceName) env->ReleaseStringUTFChars(sourceNameJS, sourceName);
	return module;
}

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    setKeyText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_setKeyText
  (JNIEnv *env, jobject me, jstring keyTextJS) {

	init();

	SWModule *module = getModule(env, me);

	if (module) {
		const char *keyText = env->GetStringUTFChars(keyTextJS, NULL);
SWLog::getSystemLog()->logDebug("setKeyText(%s, %s)", module->getName(), keyText);
		sword::SWKey *key = module->getKey();
		sword::VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
		if (vkey && (*keyText=='+' ||*keyText=='-')) {
			if (!stricmp(keyText+1, "book")) {
				vkey->setBook(vkey->getBook() + ((*keyText=='+')?1:-1));
				env->ReleaseStringUTFChars(keyTextJS, keyText);
				return;
			}
			else if (!stricmp(keyText+1, "chapter")) {
				vkey->setChapter(vkey->getChapter() + ((*keyText=='+')?1:-1));
				env->ReleaseStringUTFChars(keyTextJS, keyText);
				return;
			}
		}

		module->setKey(keyText);
		env->ReleaseStringUTFChars(keyTextJS, keyText);
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getKeyText
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getKeyText
  (JNIEnv *env, jobject me) {

	init();

	SWModule *module = getModule(env, me);

	jstring retVal = 0;
	if (module) {
		retVal = env->NewStringUTF(assureValidUTF8(module->getKeyText()));
	}
	return retVal;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getRenderText
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getRenderText
  (JNIEnv *env, jobject me) {

	init();

	SWModule *module = getModule(env, me);

	jstring retVal = 0;
	if (module) {
		retVal = env->NewStringUTF(assureValidUTF8(module->renderText()));
	}
	return retVal;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    terminateSearch
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_terminateSearch
  (JNIEnv *env, jobject me) {

	init(); 

	SWModule *module = getModule(env, me);

	if (module) {
		module->terminateSearch = true;
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    error
 * Signature: ()C
 */
JNIEXPORT jchar JNICALL Java_org_crosswire_android_sword_SWModule_error
  (JNIEnv *env, jobject me) {

	init(); 

	SWModule *module = getModule(env, me);
	
	return (module) ? module->popError() : -99;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getEntrySize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_crosswire_android_sword_SWModule_getEntrySize
  (JNIEnv *env, jobject me) {

	init(); 

	SWModule *module = getModule(env, me);

	return (module) ? module->getEntrySize() : 0;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getEntryAttribute
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_getEntryAttribute
  (JNIEnv *env, jobject me, jstring level1JS, jstring level2JS, jstring level3JS, jboolean filteredJS) {

	init(); 

	const char *level1 = env->GetStringUTFChars(level1JS, NULL);
	const char *level2 = env->GetStringUTFChars(level2JS, NULL);
	const char *level3 = env->GetStringUTFChars(level3JS, NULL);
	bool filtered = (filteredJS == JNI_TRUE);

	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = 0;

	SWModule *module = getModule(env, me);

	if (module) {

		module->renderText();	// force parse
		vector<SWBuf> results;

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

		ret = (jobjectArray) env->NewObjectArray(results.size(), clazzString, NULL);

SWLog::getSystemLog()->logDebug("getEntryAttributes: size returned: %d", results.size());

		for (int i = 0; i < results.size(); i++) {
			if (filtered) {
				env->SetObjectArrayElement(ret, i, env->NewStringUTF(assureValidUTF8(module->renderText(results[i].c_str()))));
			}
			else {
				env->SetObjectArrayElement(ret, i, env->NewStringUTF(assureValidUTF8(results[i].c_str())));
			}
		}
	}

	env->ReleaseStringUTFChars(level3JS, level3);
	env->ReleaseStringUTFChars(level2JS, level2);
	env->ReleaseStringUTFChars(level1JS, level1);

	return (ret) ? ret : (jobjectArray) env->NewObjectArray(0, clazzString, NULL);
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    parseKeyList
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_parseKeyList
  (JNIEnv *env, jobject me, jstring keyListTextJS) {

	init(); 

	const char *keyListText = env->GetStringUTFChars(keyListTextJS, NULL);

	SWModule *module = getModule(env, me);
	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret;

	if (module) {
		sword::SWKey *k = module->getKey();
		sword::VerseKey *parser = SWDYNAMIC_CAST(VerseKey, k);
		if (parser) {
			sword::ListKey result;
			result = parser->parseVerseList(keyListText, *parser, true);
			int count = 0;
			for (result = sword::TOP; !result.popError(); result++) {
				count++;
			}
			ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

			count = 0;
			for (result = sword::TOP; !result.popError(); result++) {
				env->SetObjectArrayElement(ret, count++, env->NewStringUTF(assureValidUTF8((const char *)result)));
			}
		}
		else	{
			ret = (jobjectArray) env->NewObjectArray(1, clazzString, NULL);
			env->SetObjectArrayElement(ret, 0, env->NewStringUTF(assureValidUTF8(keyListText)));
		}
	}

	env->ReleaseStringUTFChars(keyListTextJS, keyListText);

	return ret;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    hasKeyChildren
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_crosswire_android_sword_SWModule_hasKeyChildren
  (JNIEnv *env, jobject me) {

	init(); 


	SWModule *module = getModule(env, me);
	jboolean retVal = JNI_FALSE;

	if (module) {
		sword::SWKey *key = module->getKey();

		TreeKeyIdx *tkey = SWDYNAMIC_CAST(TreeKeyIdx, key);
		if (tkey) {
			retVal = (tkey->hasChildren())?JNI_TRUE:JNI_FALSE;
		}
	}
	return retVal;
}

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getKeyChildren
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_getKeyChildren
  (JNIEnv *env, jobject me) {

	init(); 


	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret;

	SWModule *module = getModule(env, me);

	if (module) {
		sword::SWKey *key = module->getKey();
		int count = 0;

		sword::VerseKey *vkey = SWDYNAMIC_CAST(VerseKey, key);
		if (vkey) {
			ret = (jobjectArray) env->NewObjectArray(7, clazzString, NULL);
			SWBuf num;
			num.appendFormatted("%d", vkey->getTestament());
			env->SetObjectArrayElement(ret, 0, env->NewStringUTF(assureValidUTF8(num.c_str())));
			num = "";
			num.appendFormatted("%d", vkey->getBook());
			env->SetObjectArrayElement(ret, 1, env->NewStringUTF(assureValidUTF8(num.c_str())));
			num = "";
			num.appendFormatted("%d", vkey->getChapter());
			env->SetObjectArrayElement(ret, 2, env->NewStringUTF(assureValidUTF8(num.c_str())));
			num = "";
			num.appendFormatted("%d", vkey->getVerse());
			env->SetObjectArrayElement(ret, 3, env->NewStringUTF(assureValidUTF8(num.c_str())));
			num = "";
			num.appendFormatted("%d", vkey->getChapterMax());
			env->SetObjectArrayElement(ret, 4, env->NewStringUTF(assureValidUTF8(num.c_str())));
			num = "";
			num.appendFormatted("%d", vkey->getVerseMax());
			env->SetObjectArrayElement(ret, 5, env->NewStringUTF(assureValidUTF8(num.c_str())));
			env->SetObjectArrayElement(ret, 6, env->NewStringUTF(assureValidUTF8(vkey->getBookName())));
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
				ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);
				count = 0;
				if (tkey->firstChild()) {
					do {
						env->SetObjectArrayElement(ret, count++, env->NewStringUTF(assureValidUTF8(tkey->getLocalName())));
					}
					while (tkey->nextSibling());
					tkey->parent();
				}
			}
		}
		return ret;
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getKeyParent
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getKeyParent
  (JNIEnv *env, jobject me) {

	init(); 


	SWBuf retVal = "";

	SWModule *module = getModule(env, me);

	if (module) {

		sword::SWKey *key = module->getKey();

		TreeKeyIdx *tkey = SWDYNAMIC_CAST(TreeKeyIdx, key);
		if (tkey) {
			if (tkey->parent()) {
				retVal = tkey->getText();
			}
		}
	}
	return env->NewStringUTF(assureValidUTF8(retVal));
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    previous
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_previous
  (JNIEnv *env, jobject me) {

	init(); 


	SWModule *module = getModule(env, me);

	if (module) {
		module->decrement();
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    next
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_next
  (JNIEnv *env, jobject me) {

	init(); 


	SWModule *module = getModule(env, me);

	if (module) {
		module->increment();
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    begin
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_begin
  (JNIEnv *env, jobject me) {

	init(); 


	SWModule *module = getModule(env, me);

	if (module) {
		module->setPosition(sword::TOP);
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getStripText
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getStripText
  (JNIEnv *env, jobject me) {

	init(); 


	SWBuf retVal = "";

	SWModule *module = getModule(env, me);

	if (module) {
		retVal = module->stripText();
	}

	return env->NewStringUTF(assureValidUTF8(retVal));
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getRawEntry
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getRawEntry
  (JNIEnv *env, jobject me) {

	init(); 


	SWBuf retVal = "";

	SWModule *module = getModule(env, me);

	if (module) {
		retVal = module->getRawEntry();
	}

	return env->NewStringUTF(assureValidUTF8(retVal));
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    setRawEntry
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_setRawEntry
  (JNIEnv *env, jobject me, jstring newEntryTextJS) {

	init(); 


	const char *newEntryText = env->GetStringUTFChars(newEntryTextJS, NULL);

	SWModule *module = getModule(env, me);

	if (module) {
		module->setEntry(newEntryText);
	}

	env->ReleaseStringUTFChars(newEntryTextJS, newEntryText);
}


jstring newBigString(JNIEnv *env, const char *buf) {
	SWBuf str = assureValidUTF8(buf);
     jclass stringClass = env->FindClass("java/lang/String");
     jmethodID ctorID = env->GetMethodID(stringClass, "<init>", "([B)V");

	jstring result;
SWLog::getSystemLog()->logDebug("newBigString: making byte array size: %d", str.size());
	jbyteArray bytes = env->NewByteArray(str.size());
SWLog::getSystemLog()->logDebug("newBigString: setting array region");

	env->SetByteArrayRegion(bytes, 0, str.size(), (jbyte *)str.c_str());
SWLog::getSystemLog()->logDebug("newBigString: newing string");
	result = (jstring)env->NewObject(stringClass, ctorID, bytes);
	env->DeleteLocalRef(bytes);
SWLog::getSystemLog()->logDebug("newBigString: returning");
	return result;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getConfigEntry
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getConfigEntry
  (JNIEnv *env, jobject me, jstring configKeyJS) {

	init(); 


	jstring retVal = 0;

	const char *configKey = env->GetStringUTFChars(configKeyJS, NULL);
SWLog::getSystemLog()->logDebug("getConfigEntry(%s)\n", configKey);

	SWModule *module = getModule(env, me);

	if (module) {
		SWBuf about = module->getConfigEntry(configKey);
		SWBuf assuredBuf = assureValidUTF8(about.c_str());
		retVal = env->NewStringUTF(assuredBuf.c_str());
	}

	env->ReleaseStringUTFChars(configKeyJS, configKey);

	return retVal;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    deleteSearchFramework
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_deleteSearchFramework
  (JNIEnv *env, jobject me) {

	init(); 


	SWModule *module = getModule(env, me);

	if (module) {
		module->deleteSearchFramework();
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    hasSearchFramework
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_crosswire_android_sword_SWModule_hasSearchFramework
  (JNIEnv *env, jobject me) {

	init(); 


	SWModule *module = getModule(env, me);

	return (module && module->hasSearchFramework()) ? JNI_TRUE : JNI_FALSE;
}


struct pu {
	pu(JNIEnv *env, jobject pr) : env(env), progressReporter(pr), last(0) {}
	JNIEnv *env;
	jobject progressReporter;
	char last;
};


void percentUpdate(char percent, void *userData) {
	struct pu *p = (struct pu *)userData;

	if (percent != p->last) {
		p->last = percent;
		jclass cls = p->env->GetObjectClass(p->progressReporter);
		jmethodID mid = p->env->GetMethodID(cls, "progressReport", "(I)V");
		if (mid != 0) {
			p->env->CallVoidMethod(p->progressReporter, mid, (jint)percent);
		}
	}
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    search
 * Signature: (Ljava/lang/String;IJLjava/lang/String;Lorg/crosswire/android/sword/SWModule/SearchProgressReporter;)[Lorg/crosswire/android/sword/SWModule/SearchHit;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_search
  (JNIEnv *env, jobject me, jstring expressionJS, jint srchType, jlong flags, jstring scopeJS, jobject progressReporter) {

	init(); 

	const int MAX_RETURN_COUNT = 999999;

	const char *expression = env->GetStringUTFChars(expressionJS, NULL);
	const char *scope = env->GetStringUTFChars(scopeJS, NULL);

	jclass clazzSearchHit = env->FindClass("org/crosswire/android/sword/SWModule$SearchHit");
	jobjectArray ret = 0;

	SWModule *module = getModule(env, me);

	// TODO: remove this from the stack
	struct pu peeuuu(env, progressReporter);

	if (module) {
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
			result = module->search(expression, srchType, flags, &lscope, 0, &percentUpdate, &peeuuu);
			delete parser;
		}
		else	result = module->search(expression, srchType, flags, 0, 0, &percentUpdate, &peeuuu);

		int count = 0;
		for (result = sword::TOP; !result.popError(); result++) count++;

		if (count > MAX_RETURN_COUNT) count = MAX_RETURN_COUNT;

		ret = (jobjectArray) env->NewObjectArray(count, clazzSearchHit, NULL);

		// if we're sorted by score, let's re-sort by verse, because Java can always re-sort by score
		result = sword::TOP;
		if ((count) && (long)result.getElement()->userData)
			result.sort();

		int i = 0;
		jstring modName = env->NewStringUTF(assureValidUTF8(module->getName()));
		jfieldID fieldIDModName = env->GetFieldID(clazzSearchHit, "modName", "Ljava/lang/String;");
		jfieldID fieldIDKey     = env->GetFieldID(clazzSearchHit, "key"    , "Ljava/lang/String;");
		jfieldID fieldIDScore   = env->GetFieldID(clazzSearchHit, "score"  , "J");
		for (result = sword::TOP; !result.popError(); result++) {
			jfieldID fieldID;
			jobject searchHit = env->AllocObject(clazzSearchHit); 

			env->SetObjectField(searchHit, fieldIDModName, modName);
			jstring key = env->NewStringUTF(assureValidUTF8((const char *)result));
			env->SetObjectField(searchHit, fieldIDKey, key);
			env->DeleteLocalRef(key);
			env->SetLongField(searchHit, fieldIDScore, (long)result.getElement()->userData);

			env->SetObjectArrayElement(ret, i++, searchHit);
			env->DeleteLocalRef(searchHit);
			if (i >= MAX_RETURN_COUNT) break;
		}
		env->DeleteLocalRef(modName);
	}

	env->ReleaseStringUTFChars(scopeJS, scope);
	env->ReleaseStringUTFChars(expressionJS, expression);

	return (ret) ? ret : (jobjectArray) env->NewObjectArray(0, clazzSearchHit, NULL);
}



// InstallMgr methods ----------------------------------------------------------------------------------


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    reInit
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_InstallMgr_reInit
  (JNIEnv *env, jobject me) {

	delete installMgr;
	installMgr = 0;
}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    syncConfig
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_org_crosswire_android_sword_InstallMgr_syncConfig
  (JNIEnv *env, jobject me) {

	initInstall();

	return installMgr->refreshRemoteSourceConfiguration();
}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    uninstallModule
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_crosswire_android_sword_InstallMgr_uninstallModule
  (JNIEnv *env, jobject me, jstring modNameJS) {

	init();
	initInstall();

	const char *modName = env->GetStringUTFChars(modNameJS, NULL);

SWLog::getSystemLog()->logDebug("uninstallModule %s\n", modName);

	SWModule *module;
	ModMap::iterator it = mgr->Modules.find(modName);
	if (it == mgr->Modules.end()) {
		return -2;
	}
	module = it->second;
	int retVal = installMgr->removeModule(mgr, module->getName());

	env->ReleaseStringUTFChars(modNameJS, modName);

	return retVal;
}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    getRemoteSources
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_InstallMgr_getRemoteSources
  (JNIEnv *env, jobject me) {

	initInstall();

	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret;

	int count = 0;
	for (InstallSourceMap::iterator it = installMgr->sources.begin(); it != installMgr->sources.end(); ++it) {
		count++;
	}
	ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);
	count = 0;
	for (InstallSourceMap::iterator it = installMgr->sources.begin(); it != installMgr->sources.end(); ++it) {
		env->SetObjectArrayElement(ret, count++, env->NewStringUTF(assureValidUTF8(it->second->caption.c_str())));
	}

	return ret;
}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    refreshRemoteSource
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_crosswire_android_sword_InstallMgr_refreshRemoteSource
  (JNIEnv *env, jobject me, jstring sourceNameJS) {

	initInstall();

	const char *sourceName = env->GetStringUTFChars(sourceNameJS, NULL);

	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
		return -3;
	}

	env->ReleaseStringUTFChars(sourceNameJS, sourceName);

	return installMgr->refreshRemoteSource(source->second);
}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    getRemoteModInfoList
 * Signature: (Ljava/lang/String;)[Lorg/crosswire/android/sword/SWMgr/ModInfo;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_InstallMgr_getRemoteModInfoList
  (JNIEnv *env, jobject me, jstring sourceNameJS) {

SWLog::getSystemLog()->logDebug("getRemoteModInfoList\n");
	init();
	initInstall();

	const char *sourceName = env->GetStringUTFChars(sourceNameJS, NULL);
SWLog::getSystemLog()->logDebug("sourceName: %s\n", sourceName);

	jclass clazzModInfo = env->FindClass("org/crosswire/android/sword/SWMgr$ModInfo");
	jfieldID nameID     = env->GetFieldID(clazzModInfo, "name",        "Ljava/lang/String;");
	jfieldID descID     = env->GetFieldID(clazzModInfo, "description", "Ljava/lang/String;");
	jfieldID catID      = env->GetFieldID(clazzModInfo, "category",    "Ljava/lang/String;");
	jfieldID langID     = env->GetFieldID(clazzModInfo, "language",    "Ljava/lang/String;");
	jfieldID versionID  = env->GetFieldID(clazzModInfo, "version",     "Ljava/lang/String;");
	jfieldID deltaID    = env->GetFieldID(clazzModInfo, "delta",       "Ljava/lang/String;");

	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	if (source == installMgr->sources.end()) {
SWLog::getSystemLog()->logDebug("remoteListModules returning 0 length array\n");
		return (jobjectArray) env->NewObjectArray(0, clazzModInfo, NULL);
	}
SWLog::getSystemLog()->logDebug("found source: %s\n", sourceName);

	map<SWModule *, int> modStats = installMgr->getModuleStatus(*mgr, *source->second->getMgr());

	int size = 0;
	for (map<SWModule *, int>::iterator it = modStats.begin(); it != modStats.end(); ++it) {
		size++;
	}

SWLog::getSystemLog()->logDebug("remoteListModules returning %d length array\n", size);
	jobjectArray ret = (jobjectArray) env->NewObjectArray(size, clazzModInfo, NULL);

	int i = 0;
	for (map<SWModule *, int>::iterator it = modStats.begin(); it != modStats.end(); ++it) {
		SWModule *module = it->first;
		int status = it->second;

		SWBuf version = module->getConfigEntry("Version");
		SWBuf statusString = " ";
		if (status & InstallMgr::MODSTAT_NEW) statusString = "*";
		if (status & InstallMgr::MODSTAT_OLDER) statusString = "-";
		if (status & InstallMgr::MODSTAT_UPDATED) statusString = "+";

		SWBuf type = module->getType();
		SWBuf cat = module->getConfigEntry("Category");
		if (cat.length() > 0) type = cat;
		jobject modInfo = env->AllocObject(clazzModInfo); 

		jstring val;
		val = env->NewStringUTF(assureValidUTF8(module->getName()));        env->SetObjectField(modInfo, nameID   , val); env->DeleteLocalRef(val);
		val = env->NewStringUTF(assureValidUTF8(module->getDescription())); env->SetObjectField(modInfo, descID   , val); env->DeleteLocalRef(val);
		val = env->NewStringUTF(assureValidUTF8(type.c_str()));          env->SetObjectField(modInfo, catID    , val); env->DeleteLocalRef(val);
		val = env->NewStringUTF(assureValidUTF8(module->getLanguage()));        env->SetObjectField(modInfo, langID   , val); env->DeleteLocalRef(val);
		val = env->NewStringUTF(assureValidUTF8(version.c_str()));       env->SetObjectField(modInfo, versionID, val); env->DeleteLocalRef(val);
		val = env->NewStringUTF(assureValidUTF8(statusString.c_str()));  env->SetObjectField(modInfo, deltaID  , val); env->DeleteLocalRef(val);

		env->SetObjectArrayElement(ret, i++, modInfo);

		env->DeleteLocalRef(modInfo);
	}

	env->ReleaseStringUTFChars(sourceNameJS, sourceName);

	return ret;
}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    remoteInstallModule
 * Signature: (Ljava/lang/String;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_org_crosswire_android_sword_InstallMgr_remoteInstallModule
  (JNIEnv *env, jobject me, jstring sourceNameJS, jstring modNameJS) {

	initInstall();

	const char *sourceName = env->GetStringUTFChars(sourceNameJS, NULL);
SWLog::getSystemLog()->logDebug("remoteInstallModule: sourceName: %s\n", sourceName);
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	env->ReleaseStringUTFChars(sourceNameJS, sourceName);

	if (source == installMgr->sources.end()) {
		return -3;
	}

	InstallSource *is = source->second;
	SWMgr *rmgr = is->getMgr();
	SWModule *module;

	const char *modName = env->GetStringUTFChars(modNameJS, NULL);
SWLog::getSystemLog()->logDebug("remoteInstallModule: modName: %s\n", modName);
	ModMap::iterator it = rmgr->Modules.find(modName);
	env->ReleaseStringUTFChars(modNameJS, modName);

	if (it == rmgr->Modules.end()) {
		return -4;
	}

	module = it->second;

	int error = installMgr->installModule(mgr, 0, module->getName(), is);

	return error;
}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    getRemoteModuleByName
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Lorg/crosswire/android/sword/SWModule;
 */
JNIEXPORT jobject JNICALL Java_org_crosswire_android_sword_InstallMgr_getRemoteModuleByName
  (JNIEnv *env, jobject me, jstring sourceNameJS, jstring modNameJS) {

	jobject retVal = 0;

	initInstall();

	const char *sourceName = env->GetStringUTFChars(sourceNameJS, NULL);
SWLog::getSystemLog()->logDebug("getRemoteModuleByName: sourceName: %s\n", sourceName);
	InstallSourceMap::iterator source = installMgr->sources.find(sourceName);
	env->ReleaseStringUTFChars(sourceNameJS, sourceName);

	if (source == installMgr->sources.end()) {
SWLog::getSystemLog()->logDebug("Couldn't find remote source [%s]\n", sourceName);
		return 0;
	}

	SWMgr *mgr = source->second->getMgr();

     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
	sword::SWModule *module = mgr->getModule(modName);
     env->ReleaseStringUTFChars(modNameJS, modName);

	if (module) {
		SWBuf type = module->getType();
		SWBuf cat = module->getConfigEntry("Category");
		if (cat.length() > 0) type = cat;
		jfieldID fieldID;
		jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
		retVal = env->AllocObject(clazzSWModule); 
		fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(assureValidUTF8(module->getName())));
		fieldID = env->GetFieldID(clazzSWModule, "description", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(assureValidUTF8(module->getDescription())));
		fieldID = env->GetFieldID(clazzSWModule, "category", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(assureValidUTF8(type.c_str())));
		fieldID = env->GetFieldID(clazzSWModule, "remoteSourceName", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, sourceNameJS);
	}

	return retVal;

}


/*
 * Class:     org_crosswire_android_sword_InstallMgr
 * Method:    setUserDisclaimerConfirmed
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_InstallMgr_setUserDisclaimerConfirmed
  (JNIEnv *env, jobject me) {

	initInstall();

	installMgr->setUserDisclaimerConfirmed(true);
}

