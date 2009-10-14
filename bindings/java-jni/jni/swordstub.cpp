/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
//#include "first.h"
#include <iostream>

#include <vector>

#include <jni.h>

#include <utilstr.h>
#include <swversion.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <localemgr.h>
#include <treekeyidx.h>

#include "webmgr.hpp"
#include "org_crosswire_android_sword_SWMgr.h"
#include "org_crosswire_android_sword_SWModule.h"


using std::cerr;

using namespace sword;

WebMgr *mgr;

static void init() {
	if (!mgr) mgr = new WebMgr("/sdcard/sword");
}

JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_version
  (JNIEnv *env, jobject me) {
	SWVersion v;
	return env->NewStringUTF(v.currentVersion);
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

	sword::SWModule *module = 0;

	int size = 0;
	for (sword::ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); it++) {
		if ((!(it->second->getConfigEntry("CipherKey"))) || (*(it->second->getConfigEntry("CipherKey"))))
			size++;
	}

	jclass clazzModInfo = env->FindClass("org/crosswire/android/sword/SWMgr$ModInfo");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(size, clazzModInfo, NULL);

	int i = 0;
	for (sword::ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); it++) {
		module = it->second;
		if ((!(module->getConfigEntry("CipherKey"))) || (*(module->getConfigEntry("CipherKey")))) {
			SWBuf type = module->Type();
			SWBuf cat = module->getConfigEntry("Category");
			if (cat.length() > 0) type = cat;
			jfieldID fieldID;
			jobject modInfo = env->AllocObject(clazzModInfo); 

			fieldID = env->GetFieldID(clazzModInfo, "name", "Ljava/lang/String;"); env->SetObjectField(modInfo, fieldID, env->NewStringUTF(module->Name()));
			fieldID = env->GetFieldID(clazzModInfo, "description", "Ljava/lang/String;"); env->SetObjectField(modInfo, fieldID, env->NewStringUTF(module->Description()));
			fieldID = env->GetFieldID(clazzModInfo, "category", "Ljava/lang/String;"); env->SetObjectField(modInfo, fieldID, env->NewStringUTF(type.c_str()));
			fieldID = env->GetFieldID(clazzModInfo, "language", "Ljava/lang/String;"); env->SetObjectField(modInfo, fieldID, env->NewStringUTF(module->Lang()));
			env->SetObjectArrayElement(ret, i++, modInfo);
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
	jobject retVal = 0;

     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
	sword::SWModule *module = mgr->getModule(modName);
     env->ReleaseStringUTFChars(modNameJS, modName);

	if (module) {
		SWBuf type = module->Type();
		SWBuf cat = module->getConfigEntry("Category");
		if (cat.length() > 0) type = cat;
		jfieldID fieldID;
		jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
		retVal = env->AllocObject(clazzSWModule); 
		fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(module->Name()));
		fieldID = env->GetFieldID(clazzSWModule, "description", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(module->Description()));
		fieldID = env->GetFieldID(clazzSWModule, "category", "Ljava/lang/String;"); env->SetObjectField(retVal, fieldID, env->NewStringUTF(type.c_str()));
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
     const char *option = env->GetStringUTFChars(optionJS, NULL);

	SWBuf value = mgr->getGlobalOption(option);

     env->ReleaseStringUTFChars(optionJS, option);

	return env->NewStringUTF(value);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptionTip
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptionTip
  (JNIEnv *env, jobject me, jstring optionJS) {
     const char *option = env->GetStringUTFChars(optionJS, NULL);

	SWBuf value = mgr->getGlobalOptionTip(option);

     env->ReleaseStringUTFChars(optionJS, option);

	return env->NewStringUTF(value);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    filterText
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_filterText
  (JNIEnv *env, jobject me, jstring filterNameJS, jstring textJS) {
     const char *filterName = env->GetStringUTFChars(filterNameJS, NULL);
     const char *text  = env->GetStringUTFChars(textJS, NULL);

	SWBuf buf = text;
	// hmmm, in the future, provide a param to specify filter value maybe?
	mgr->setGlobalOption("Greek Accents", "Off");
	char errStatus = mgr->filterText(filterName, buf);

     env->ReleaseStringUTFChars(textJS, text);
     env->ReleaseStringUTFChars(filterNameJS, filterName);

	return env->NewStringUTF(buf);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptions
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptions
  (JNIEnv *env, jobject me) {
	sword::StringList options = mgr->getGlobalOptions();
	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		count++;
	}

	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		env->SetObjectArrayElement(ret, count++, env->NewStringUTF(*it));
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
     const char *option = env->GetStringUTFChars(optionJS, NULL);

	sword::StringList options = mgr->getGlobalOptionValues(option);

     env->ReleaseStringUTFChars(optionJS, option);

	int count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		count++;
	}
	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

	count = 0;
	for (sword::StringList::iterator it = options.begin(); it != options.end(); it++) {
		env->SetObjectArrayElement(ret, count++, env->NewStringUTF(*it));
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
	mgr->setJavascript(val == JNI_TRUE);
}


/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getAvailableLocales
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getAvailableLocales
  (JNIEnv *env, jobject me) {
	sword::StringList localeNames = LocaleMgr::getSystemLocaleMgr()->getAvailableLocales();
	int count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); it++) {
		count++;
	}

	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

	count = 0;
	for (sword::StringList::iterator it = localeNames.begin(); it != localeNames.end(); it++) {
		env->SetObjectArrayElement(ret, count++, env->NewStringUTF(*it));
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
     const char *localeName = env->GetStringUTFChars(localeNameJS, NULL);

	LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(localeName);

     env->ReleaseStringUTFChars(localeNameJS, localeName);
}



// SWModule methods ----------------------------------------------------------------------------------

SWModule *getModule(JNIEnv *env, jobject me) {
	jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
	jfieldID fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;");
	jstring modNameJS = (jstring)env->GetObjectField(me, fieldID);
     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
	sword::SWModule *module = mgr->getModule(modName);
     env->ReleaseStringUTFChars(modNameJS, modName);
	return module;
}

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    setKeyText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_setKeyText
  (JNIEnv *env, jobject me, jstring keyTextJS) {

	SWModule *module = getModule(env, me);

	if (module) {
		const char *keyText = env->GetStringUTFChars(keyTextJS, NULL);
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

		module->KeyText(keyText);
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

	SWModule *module = getModule(env, me);

	jstring retVal = 0;
	if (module) {
		retVal = env->NewStringUTF(module->getKeyText());
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

	SWModule *module = getModule(env, me);

	jstring retVal = 0;
	if (module) {
		retVal = env->NewStringUTF(module->RenderText());
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

	SWModule *module = getModule(env, me);
	
	return (module) ? module->Error() : -99;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getEntrySize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_crosswire_android_sword_SWModule_getEntrySize
  (JNIEnv *env, jobject me) {

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
	const char *level1 = env->GetStringUTFChars(level1JS, NULL);
	const char *level2 = env->GetStringUTFChars(level2JS, NULL);
	const char *level3 = env->GetStringUTFChars(level3JS, NULL);
	bool filtered = (filteredJS == JNI_TRUE);

	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(0, clazzString, NULL);

	SWModule *module = getModule(env, me);

	if (module) {

		module->RenderText();	// force parse
		std::vector<SWBuf> results;

		sword::AttributeTypeList &entryAttribs = module->getEntryAttributes();
		sword::AttributeTypeList::iterator i1Start, i1End;
		sword::AttributeList::iterator i2Start, i2End;
		sword::AttributeValue::iterator i3Start, i3End;

		if ((level1) && (*level1)) {
			i1Start = entryAttribs.find(level1);
			i1End = i1Start;
			if (i1End != entryAttribs.end())
				i1End++;
		}
		else {
			i1Start = entryAttribs.begin();
			i1End   = entryAttribs.end();
		}
		for (;i1Start != i1End; i1Start++) {
			if ((level2) && (*level2)) {
				i2Start = i1Start->second.find(level2);
				i2End = i2Start;
				if (i2End != i1Start->second.end())
					i2End++;
			}
			else {
				i2Start = i1Start->second.begin();
				i2End   = i1Start->second.end();
			}
			for (;i2Start != i2End; i2Start++) {
				if ((level3) && (*level3)) {
					i3Start = i2Start->second.find(level3);
					i3End = i3Start;
					if (i3End != i2Start->second.end())
						i3End++;
				}
				else {
					i3Start = i2Start->second.begin();
					i3End   = i2Start->second.end();
				}
				for (;i3Start != i3End; i3Start++) {
					results.push_back(i3Start->second);
				}
				if (i3Start != i3End)
					break;
			}
			if (i2Start != i2End)
				break;
		}

		ret = (jobjectArray) env->NewObjectArray(results.size(), clazzString, NULL);

		for (int i = 0; i < results.size(); i++) {
			if (filtered) {
				env->SetObjectArrayElement(ret, i, env->NewStringUTF(module->RenderText(results[i].c_str())));
			}
			else {
				env->SetObjectArrayElement(ret, i, env->NewStringUTF(results[i].c_str()));
			}
		}
	}

	env->ReleaseStringUTFChars(level3JS, level3);
	env->ReleaseStringUTFChars(level2JS, level2);
	env->ReleaseStringUTFChars(level1JS, level1);

	return ret;
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    parseKeyList
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_parseKeyList
  (JNIEnv *env, jobject me, jstring keyListTextJS) {

	const char *keyListText = env->GetStringUTFChars(keyListTextJS, NULL);

	SWModule *module = getModule(env, me);
	jclass clazzString = env->FindClass("java/lang/String");
	jobjectArray ret;

	if (module) {
		sword::SWKey *k = module->getKey();
		sword::VerseKey *parser = SWDYNAMIC_CAST(VerseKey, k);
		if (parser) {
			sword::ListKey result;
			result = parser->ParseVerseList(keyListText, *parser, true);
			int count = 0;
			for (result = sword::TOP; !result.Error(); result++) {
				count++;
			}
			ret = (jobjectArray) env->NewObjectArray(count, clazzString, NULL);

			count = 0;
			for (result = sword::TOP; !result.Error(); result++) {
				env->SetObjectArrayElement(ret, count++, env->NewStringUTF((const char *)result));
			}
		}
		else	{
			ret = (jobjectArray) env->NewObjectArray(1, clazzString, NULL);
			env->SetObjectArrayElement(ret, 0, env->NewStringUTF(keyListText));
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
			env->SetObjectArrayElement(ret, 0, env->NewStringUTF(num.c_str()));
			num = "";
			num.appendFormatted("%d", vkey->getBook());
			env->SetObjectArrayElement(ret, 1, env->NewStringUTF(num.c_str()));
			num = "";
			num.appendFormatted("%d", vkey->getChapter());
			env->SetObjectArrayElement(ret, 2, env->NewStringUTF(num.c_str()));
			num = "";
			num.appendFormatted("%d", vkey->getVerse());
			env->SetObjectArrayElement(ret, 3, env->NewStringUTF(num.c_str()));
			num = "";
			num.appendFormatted("%d", vkey->getChapterMax());
			env->SetObjectArrayElement(ret, 4, env->NewStringUTF(num.c_str()));
			num = "";
			num.appendFormatted("%d", vkey->getVerseMax());
			env->SetObjectArrayElement(ret, 5, env->NewStringUTF(num.c_str()));
			num = "";
			num.appendFormatted("%d", vkey->getBookName());
			env->SetObjectArrayElement(ret, 6, env->NewStringUTF(num.c_str()));
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
						env->SetObjectArrayElement(ret, count++, env->NewStringUTF(tkey->getLocalName()));
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
	return env->NewStringUTF(retVal);
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    previous
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_previous
  (JNIEnv *env, jobject me) {

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

	SWBuf retVal = "";

	SWModule *module = getModule(env, me);

	if (module) {
		retVal = module->StripText();
	}

	return env->NewStringUTF(retVal);
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getRawEntry
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getRawEntry
  (JNIEnv *env, jobject me) {

	SWBuf retVal = "";

	SWModule *module = getModule(env, me);

	if (module) {
		retVal = module->getRawEntry();
	}

	return env->NewStringUTF(retVal);
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    setRawEntry
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_setRawEntry
  (JNIEnv *env, jobject me, jstring newEntryTextJS) {

	const char *newEntryText = env->GetStringUTFChars(newEntryTextJS, NULL);

	SWModule *module = getModule(env, me);

	if (module) {
		module->setEntry(newEntryText);
	}

	env->ReleaseStringUTFChars(newEntryTextJS, newEntryText);
}


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getConfigEntry
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getConfigEntry
  (JNIEnv *env, jobject me, jstring configKeyJS) {

	jstring retVal = 0;

	const char *configKey = env->GetStringUTFChars(configKeyJS, NULL);

	SWModule *module = getModule(env, me);

	if (module && module->getConfigEntry(configKey)) {
		retVal = env->NewStringUTF(module->getConfigEntry(configKey));
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

	const char *expression = env->GetStringUTFChars(expressionJS, NULL);
	const char *scope = env->GetStringUTFChars(scopeJS, NULL);

	jclass clazzSearchHit = env->FindClass("org/crosswire/android/sword/SWModule$SearchHit");
	jobjectArray ret = (jobjectArray) env->NewObjectArray(0, clazzSearchHit, NULL);

	SWModule *module = getModule(env, me);

	struct pu peeuuu(env, progressReporter);

	if (module) {
		sword::ListKey lscope;
		sword::ListKey result;

		if ((scope) && (strlen(scope)) > 0) {
			sword::SWKey *p = module->CreateKey();
			sword::VerseKey *parser = SWDYNAMIC_CAST(VerseKey, p);
			if (!parser) {
				delete p;
				parser = new VerseKey();
			}
			*parser = module->getKeyText();
			lscope = parser->ParseVerseList(scope, *parser, true);
			result = module->search(expression, srchType, flags, &lscope, 0, &percentUpdate, &peeuuu);
			delete parser;
		}
		else	result = module->search(expression, srchType, flags, 0, 0, &percentUpdate, &peeuuu);

		int count = 0;
		for (result = sword::TOP; !result.Error(); result++) count++;

		ret = (jobjectArray) env->NewObjectArray(count, clazzSearchHit, NULL);

		// if we're sorted by score, let's re-sort by verse, because Java can always re-sort by score
		result = sword::TOP;
		if ((count) && (long)result.getElement()->userData)
			result.sort();

		int i = 0;
		for (result = sword::TOP; !result.Error(); result++) {
			jfieldID fieldID;
			jobject searchHit = env->AllocObject(clazzSearchHit); 

			fieldID = env->GetFieldID(clazzSearchHit, "modName", "Ljava/lang/String;"); env->SetObjectField(searchHit, fieldID, env->NewStringUTF(module->Name()));
			fieldID = env->GetFieldID(clazzSearchHit, "key", "Ljava/lang/String;"); env->SetObjectField(searchHit, fieldID, env->NewStringUTF((const char *)result));
			fieldID = env->GetFieldID(clazzSearchHit, "score", "J"); env->SetLongField(searchHit, fieldID, (long)result.getElement()->userData);

			env->SetObjectArrayElement(ret, i++, searchHit);
		}
	}

	env->ReleaseStringUTFChars(scopeJS, scope);
	env->ReleaseStringUTFChars(expressionJS, expression);

	return ret;
}


