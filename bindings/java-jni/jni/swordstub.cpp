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

#include <jni.h>

#include <utilstr.h>
#include <swversion.h>
#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

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

#if 0
/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setGlobalOption
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setGlobalOption
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOption
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOption
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptionTip
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptionTip
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    filterText
 * Signature: (Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWMgr_filterText
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptions
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptions
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getGlobalOptionValues
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getGlobalOptionValues
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setCipherKey
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setCipherKey
  (JNIEnv *, jobject, jstring, jstring);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setJavascript
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setJavascript
  (JNIEnv *, jobject, jboolean);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    getAvailableLocales
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWMgr_getAvailableLocales
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWMgr
 * Method:    setDefaultLocale
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWMgr_setDefaultLocale
  (JNIEnv *, jobject, jstring);

#endif






// SWModule methods ----------------------------------------------------------------------------------


/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    setKeyText
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_setKeyText
  (JNIEnv *env, jobject me, jstring keyTextJS) {

	jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
	jfieldID fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;");
	jstring modNameJS = (jstring)env->GetObjectField(me, fieldID);
     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
	sword::SWModule *module = mgr->getModule(modName);
     env->ReleaseStringUTFChars(modNameJS, modName);

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
	jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
	jfieldID fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;");
	jstring modNameJS = (jstring)env->GetObjectField(me, fieldID);
     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
	sword::SWModule *module = mgr->getModule(modName);
     env->ReleaseStringUTFChars(modNameJS, modName);

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

	jclass clazzSWModule = env->FindClass("org/crosswire/android/sword/SWModule");
	jfieldID fieldID = env->GetFieldID(clazzSWModule, "name", "Ljava/lang/String;");
	jstring modNameJS = (jstring)env->GetObjectField(me, fieldID);
     const char *modName = env->GetStringUTFChars(modNameJS, NULL);
	sword::SWModule *module = mgr->getModule(modName);
     env->ReleaseStringUTFChars(modNameJS, modName);

	jstring retVal = 0;
	if (module) {
		retVal = env->NewStringUTF(module->RenderText());
	}
	return retVal;
}




#if 0

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    terminateSearch
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_terminateSearch
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    search
 * Signature: (Ljava/lang/String;IJLjava/lang/String;)[Lorg/crosswire/android/sword/SWModule/SearchHit;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_search
  (JNIEnv *, jobject, jstring, jint, jlong, jstring);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    error
 * Signature: ()C
 */
JNIEXPORT jchar JNICALL Java_org_crosswire_android_sword_SWModule_error
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getEntrySize
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_org_crosswire_android_sword_SWModule_getEntrySize
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getEntryAttribute
 * Signature: (Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;Z)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_getEntryAttribute
  (JNIEnv *, jobject, jstring, jstring, jstring, jboolean);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    parseKeyList
 * Signature: (Ljava/lang/String;)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_parseKeyList
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    hasKeyChildren
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_crosswire_android_sword_SWModule_hasKeyChildren
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getKeyChildren
 * Signature: ()[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_org_crosswire_android_sword_SWModule_getKeyChildren
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getKeyParent
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getKeyParent
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    previous
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_previous
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    next
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_next
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    begin
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_begin
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getStripText
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getStripText
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getRawEntry
 * Signature: ()Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getRawEntry
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    setRawEntry
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_setRawEntry
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    getConfigEntry
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_org_crosswire_android_sword_SWModule_getConfigEntry
  (JNIEnv *, jobject, jstring);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    deleteSearchFramework
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_org_crosswire_android_sword_SWModule_deleteSearchFramework
  (JNIEnv *, jobject);

/*
 * Class:     org_crosswire_android_sword_SWModule
 * Method:    hasSearchFramework
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_org_crosswire_android_sword_SWModule_hasSearchFramework
  (JNIEnv *, jobject);

#endif
