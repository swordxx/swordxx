#ifndef SWORDAPI_H
#define SWORDAPI_H

#include <swmodule.h>
#include <swkey.h>
#include <versekey.h>
#include <swmgr.h>

#ifdef __cplusplus
#endif

extern "C" {

#define SWHANDLE int

typedef struct {
	ModMap::iterator it;
	ModMap::iterator end;
} ModItType;

//-----------------------------------------------------------------
// stringlist_iterator methods

void stringlist_iterator_next(SWHANDLE hsli);
const char *stringlist_iterator_val(SWHANDLE hsli);


//-----------------------------------------------------------------
// modmap methods
//
void ModList_iterator_next(SWHANDLE hmmi);
SWHANDLE ModList_iterator_val(SWHANDLE hmmi);


//-----------------------------------------------------------------
// SWMgr methods
//
SWHANDLE SWMgr_new();
// SWConfig *, SWConfig *, bool, SWFilterMgr *
SWHANDLE SWMgr_newEx(SWHANDLE hiconfig, SWHANDLE hisysconfig, char autoload, SWHANDLE hfilterMgr);
void     SWMgr_delete(SWHANDLE hmgr);
SWHANDLE SWMgr_getConfig(SWHANDLE hmgr);
SWHANDLE SWMgr_getModulesIterator(SWHANDLE hmgr);
SWHANDLE SWMgr_getModuleByName(SWHANDLE hmgr, const char *name);
const char *   SWMgr_getPrefixPath(SWHANDLE hmgr);
const char *   SWMgr_getConfigPath(SWHANDLE hmgr);
void     SWMgr_setGlobalOption(SWHANDLE hmgr, const char *option, const char *value);
const char *   SWMgr_getGlobalOption(SWHANDLE hmgr, const char *option);
const char *   SWMgr_getGlobalOptionTip(SWHANDLE hmgr, const char *option);
// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionsIterator(SWHANDLE hmgr);
// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionValuesIterator(SWHANDLE hmgr, const char *option);
void     SWMgr_setCipherKey(SWHANDLE hmgr, const char *modName, const char *key);


//-----------------------------------------------------------------
// SWModule methods

void  SWModule_terminateSearch(SWHANDLE hmodule);
char  SWModule_error(SWHANDLE hmodule);
int   SWModule_getEntrySize(SWHANDLE hmodule);
void  SWModule_setKeyText(SWHANDLE hmodule, const char *key);
const char *SWModule_getKeyText(SWHANDLE hmodule);
const char *SWModule_getName(SWHANDLE hmodule);
const char *SWModule_getDescription(SWHANDLE hmodule);
const char *SWModule_getType(SWHANDLE hmodule);
void  SWModule_previous(SWHANDLE hmodule);
void  SWModule_next(SWHANDLE hmodule);
void  SWModule_begin(SWHANDLE hmodule);
const char *SWModule_getStripText(SWHANDLE hmodule);
const char *SWModule_getRenderText(SWHANDLE hmodule);

}
#ifdef __cplusplus
#endif

#endif
