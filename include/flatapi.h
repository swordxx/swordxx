#ifndef SWORDAPI_H
#define SWORDAPI_H

#include <swmodule.h>
#include <swkey.h>
#include <versekey.h>

#ifdef __cplusplus
extern "C" {
#endif


#define SWHANDLE long

//-----------------------------------------------------------------
// stringlist_iterator methods

void stringlist_iterator_next(SWHANDLE hsli);
char *stringlist_iterator_val(SWHANDLE hsli);


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
char *   SWMgr_getPrefixPath(SWHANDLE hmgr);
char *   SWMgr_getConfigPath(SWHANDLE hmgr);
void     SWMgr_setGlobalOption(SWHANDLE hmgr, char *option, char *value);
char *   SWMgr_getGlobalOption(SWHANDLE hmgr, char *option);
char *   SWMgr_getGlobalOptionTip(SWHANDLE hmgr, char *option);
// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionsIterator(SWHANDLE hmgr);
// ret: forward_iterator
SWHANDLE SWMgr_getGlobalOptionValuesIterator(SWHANDLE hmgr, char *option);
void     SWMgr_setCipherKey(SWHANDLE hmgr, char *modName, char *key);


//-----------------------------------------------------------------
// SWModule methods

void  SWModule_terminateSearch(SWHANDLE hmodule);
char  SWModule_error(SWHANDLE hmodule);
int   SWModule_getEntrySize(SWHANDLE hmodule);
void  SWModule_setKeyText(SWHANDLE hmodule, char *key);
char *SWModule_getKeyText(SWHANDLE hmodule);
char *SWModule_getName(SWHANDLE hmodule);
char *SWModule_getDescription(SWHANDLE hmodule);
char *SWModule_getType(SWHANDLE hmodule);
void  SWModule_previous(SWHANDLE hmodule);
void  SWModule_next(SWHANDLE hmodule);
void  SWModule_begin(SWHANDLE hmodule);
char *SWModule_getStripText(SWHANDLE hmodule);
char *SWModule_getRenderText(SWHANDLE hmodule);

#ifdef __cplusplus
}
#endif

#endif
