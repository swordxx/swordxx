#ifndef GSOAPAPI_H
#define GSOAPAPI_H

typedef int SWHANDLE;

//-----------------------------------------------------------------
// stringlist_iterator methods

//int sword__stringlist_iterator_next(SWHANDLE hsli char **);
//int sword__stringlist_iterator_val(SWHANDLE hsli, char **);


//-----------------------------------------------------------------
// modmap methods
//
int sword__ModList_iterator_next(SWHANDLE hmmi, int &noop);
int sword__ModList_iterator_val(SWHANDLE hmmi, SWHANDLE &hmodule);


//-----------------------------------------------------------------
// SWMgr methods
//
int sword__SWMgr_new(SWHANDLE &retVal);
// SWConfig *, SWConfig *, bool, SWFilterMgr *
//SWHANDLE sword__SWMgr_newEx(SWHANDLE hiconfig, SWHANDLE hisysconfig, char autoload, SWHANDLE hfilterMgr);
int sword__SWMgr_delete(SWHANDLE hmgr, int &noop);
//SWHANDLE sword__SWMgr_getConfig(SWHANDLE hmgr);
int sword__SWMgr_getModulesIterator(SWHANDLE hmgr, SWHANDLE &hmodIterator);
int sword__SWMgr_getModuleByName(SWHANDLE hmgr, char *name, SWHANDLE &hmodule);
//char *   sword__SWMgr_getPrefixPath(SWHANDLE hmgr);
//char *   sword__SWMgr_getConfigPath(SWHANDLE hmgr);
//void     sword__SWMgr_setGlobalOption(SWHANDLE hmgr, char *option, char *value);
//char *   sword__SWMgr_getGlobalOption(SWHANDLE hmgr, char *option);
//char *   sword__SWMgr_getGlobalOptionTip(SWHANDLE hmgr, char *option);
// ret: forward_iterator
//SWHANDLE sword__SWMgr_getGlobalOptionsIterator(SWHANDLE hmgr);
// ret: forward_iterator
//SWHANDLE sword__SWMgr_getGlobalOptionValuesIterator(SWHANDLE hmgr, char *option);
//void     sword__SWMgr_setCipherKey(SWHANDLE hmgr, char *modName, char *key);


//-----------------------------------------------------------------
// SWModule methods

//void  sword__SWModule_terminateSearch(SWHANDLE hmodule);
//char  sword__SWModule_error(SWHANDLE hmodule);
//int   sword__SWModule_getEntrySize(SWHANDLE hmodule);
//void  sword__SWModule_setKeyText(SWHANDLE hmodule, char *key);
//char *sword__SWModule_getKeyText(SWHANDLE hmodule);
int sword__SWModule_getName(SWHANDLE hmodule, char **name);
int sword__SWModule_getDescription(SWHANDLE hmodule, char **description);
//char *sword__SWModule_getType(SWHANDLE hmodule);
//void  sword__SWModule_previous(SWHANDLE hmodule);
//void  sword__SWModule_next(SWHANDLE hmodule);
//void  sword__SWModule_begin(SWHANDLE hmodule);
//char *sword__SWModule_getStripText(SWHANDLE hmodule);
//char *sword__SWModule_getRenderText(SWHANDLE hmodule);

#endif
