#ifndef SWORDAPI_H
#define SWORDAPI_H

//#include <swmodule.h>
//#include <swkey.h>
//#include <versekey.h>
//#include <swmgr.h>
#include <defs.h>
#ifdef __cplusplus
#endif

extern "C" {

#define SWHANDLE int

//using namespace sword;

//-----------------------------------------------------------------
// stringlist_iterator methods

void SWDLLEXPORT stringlist_iterator_next(SWHANDLE hsli);
const char * SWDLLEXPORT stringlist_iterator_val(SWHANDLE hsli);


//-----------------------------------------------------------------
// modmap methods
//
void SWDLLEXPORT ModList_iterator_next(SWHANDLE hmmi);
SWHANDLE SWDLLEXPORT ModList_iterator_val(SWHANDLE hmmi);


//-----------------------------------------------------------------
// SWMgr methods
//
SWHANDLE SWDLLEXPORT SWMgr_new();
// SWConfig *, SWConfig *, bool, SWFilterMgr *
SWHANDLE SWDLLEXPORT SWMgr_newEx(SWHANDLE hiconfig, SWHANDLE hisysconfig, char autoload, SWHANDLE hfilterMgr);
void     SWDLLEXPORT SWMgr_delete(SWHANDLE hmgr);
SWHANDLE SWDLLEXPORT SWMgr_getConfig(SWHANDLE hmgr);
SWHANDLE SWDLLEXPORT SWMgr_getModulesIterator(SWHANDLE hmgr);
SWHANDLE SWDLLEXPORT SWMgr_getModuleByName(SWHANDLE hmgr, const char *name);
const char *   SWDLLEXPORT SWMgr_getPrefixPath(SWHANDLE hmgr);
const char *   SWDLLEXPORT SWMgr_getConfigPath(SWHANDLE hmgr);
void     SWDLLEXPORT SWMgr_setGlobalOption(SWHANDLE hmgr, const char *option, const char *value);
const char *   SWDLLEXPORT SWMgr_getGlobalOption(SWHANDLE hmgr, const char *option);
const char *   SWDLLEXPORT SWMgr_getGlobalOptionTip(SWHANDLE hmgr, const char *option);
// ret: forward_iterator
SWHANDLE SWDLLEXPORT SWMgr_getGlobalOptionsIterator(SWHANDLE hmgr);
// ret: forward_iterator
SWHANDLE SWDLLEXPORT SWMgr_getGlobalOptionValuesIterator(SWHANDLE hmgr, const char *option);
void     SWDLLEXPORT SWMgr_setCipherKey(SWHANDLE hmgr, const char *modName, const char *key);


//-----------------------------------------------------------------
// SWModule methods

void  SWDLLEXPORT SWModule_terminateSearch(SWHANDLE hmodule);
char  SWDLLEXPORT SWModule_error(SWHANDLE hmodule);
int   SWDLLEXPORT SWModule_getEntrySize(SWHANDLE hmodule);
void  SWDLLEXPORT SWModule_setKeyText(SWHANDLE hmodule, const char *key);
const char * SWDLLEXPORT SWModule_getKeyText(SWHANDLE hmodule);
const char * SWDLLEXPORT SWModule_getName(SWHANDLE hmodule);
const char * SWDLLEXPORT SWModule_getDescription(SWHANDLE hmodule);
const char * SWDLLEXPORT SWModule_getType(SWHANDLE hmodule);
void  SWDLLEXPORT SWModule_previous(SWHANDLE hmodule);
void  SWDLLEXPORT SWModule_next(SWHANDLE hmodule);
void  SWDLLEXPORT SWModule_begin(SWHANDLE hmodule);
const char * SWDLLEXPORT SWModule_getStripText(SWHANDLE hmodule);
const char * SWDLLEXPORT SWModule_getRenderText(SWHANDLE hmodule);

}
#ifdef __cplusplus
#endif

#endif
