# This file was created automatically by SWIG.
# Don't modify this file, modify the SWIG interface instead.
# This file is compatible with both classic and new-style classes.
import _Sword
def _swig_setattr(self,class_type,name,value):
    if (name == "this"):
        if isinstance(value, class_type):
            self.__dict__[name] = value.this
            if hasattr(value,"thisown"): self.__dict__["thisown"] = value.thisown
            del value.thisown
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    self.__dict__[name] = value

def _swig_getattr(self,class_type,name):
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


class SWConfig(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWConfig, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SWConfig, name)
    __swig_setmethods__["filename"] = _Sword.SWConfig_filename_set
    __swig_getmethods__["filename"] = _Sword.SWConfig_filename_get
    if _newclass:filename = property(_Sword.SWConfig_filename_get,_Sword.SWConfig_filename_set)
    __swig_setmethods__["Sections"] = _Sword.SWConfig_Sections_set
    __swig_getmethods__["Sections"] = _Sword.SWConfig_Sections_get
    if _newclass:Sections = property(_Sword.SWConfig_Sections_get,_Sword.SWConfig_Sections_set)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SWConfig,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_SWConfig):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Load(*args): return apply(_Sword.SWConfig_Load,args)
    def Save(*args): return apply(_Sword.SWConfig_Save,args)
    def set(*args): return apply(_Sword.SWConfig_set,args)
    def get(*args): return apply(_Sword.SWConfig_get,args)
    def __repr__(self):
        return "<C SWConfig instance at %s>" % (self.this,)

class SWConfigPtr(SWConfig):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWConfig
_Sword.SWConfig_swigregister(SWConfigPtr)

class SWMgr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWMgr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SWMgr, name)
    __swig_getmethods__["findConfig"] = lambda x: _Sword.SWMgr_findConfig
    if _newclass:findConfig = staticmethod(_Sword.SWMgr_findConfig)
    __swig_setmethods__["config"] = _Sword.SWMgr_config_set
    __swig_getmethods__["config"] = _Sword.SWMgr_config_get
    if _newclass:config = property(_Sword.SWMgr_config_get,_Sword.SWMgr_config_set)
    __swig_setmethods__["sysconfig"] = _Sword.SWMgr_sysconfig_set
    __swig_getmethods__["sysconfig"] = _Sword.SWMgr_sysconfig_get
    if _newclass:sysconfig = property(_Sword.SWMgr_sysconfig_get,_Sword.SWMgr_sysconfig_set)
    __swig_setmethods__["Modules"] = _Sword.SWMgr_Modules_set
    __swig_getmethods__["Modules"] = _Sword.SWMgr_Modules_get
    if _newclass:Modules = property(_Sword.SWMgr_Modules_get,_Sword.SWMgr_Modules_set)
    __swig_setmethods__["prefixPath"] = _Sword.SWMgr_prefixPath_set
    __swig_getmethods__["prefixPath"] = _Sword.SWMgr_prefixPath_get
    if _newclass:prefixPath = property(_Sword.SWMgr_prefixPath_get,_Sword.SWMgr_prefixPath_set)
    __swig_setmethods__["configPath"] = _Sword.SWMgr_configPath_set
    __swig_getmethods__["configPath"] = _Sword.SWMgr_configPath_get
    if _newclass:configPath = property(_Sword.SWMgr_configPath_get,_Sword.SWMgr_configPath_set)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SWMgr,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_SWMgr):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Load(*args): return apply(_Sword.SWMgr_Load,args)
    def setGlobalOption(*args): return apply(_Sword.SWMgr_setGlobalOption,args)
    def getGlobalOption(*args): return apply(_Sword.SWMgr_getGlobalOption,args)
    def getGlobalOptionTip(*args): return apply(_Sword.SWMgr_getGlobalOptionTip,args)
    def getGlobalOptions(*args): return apply(_Sword.SWMgr_getGlobalOptions,args)
    def getGlobalOptionValues(*args): return apply(_Sword.SWMgr_getGlobalOptionValues,args)
    def setCipherKey(*args): return apply(_Sword.SWMgr_setCipherKey,args)
    def module(*args): return apply(_Sword.SWMgr_module,args)
    def __repr__(self):
        return "<C SWMgr instance at %s>" % (self.this,)

class SWMgrPtr(SWMgr):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWMgr
_Sword.SWMgr_swigregister(SWMgrPtr)
SWMgr_findConfig = _Sword.SWMgr_findConfig


class SWModule(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWModule, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SWModule, name)
    __swig_setmethods__["terminateSearch"] = _Sword.SWModule_terminateSearch_set
    __swig_getmethods__["terminateSearch"] = _Sword.SWModule_terminateSearch_get
    if _newclass:terminateSearch = property(_Sword.SWModule_terminateSearch_get,_Sword.SWModule_terminateSearch_set)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SWModule,args)
        self.thisown = 1
    def Error(*args): return apply(_Sword.SWModule_Error,args)
    def isUnicode(*args): return apply(_Sword.SWModule_isUnicode,args)
    def getConfig(*args): return apply(_Sword.SWModule_getConfig,args)
    def getConfigEntry(*args): return apply(_Sword.SWModule_getConfigEntry,args)
    def SetKey(*args): return apply(_Sword.SWModule_SetKey,args)
    def Key(*args): return apply(_Sword.SWModule_Key,args)
    def CreateKey(*args): return apply(_Sword.SWModule_CreateKey,args)
    def KeyText(*args): return apply(_Sword.SWModule_KeyText,args)
    def Display(*args): return apply(_Sword.SWModule_Display,args)
    __swig_getmethods__["nullPercent"] = lambda x: _Sword.SWModule_nullPercent
    if _newclass:nullPercent = staticmethod(_Sword.SWModule_nullPercent)
    def Search(*args): return apply(_Sword.SWModule_Search,args)
    def createSearchFramework(*args): return apply(_Sword.SWModule_createSearchFramework,args)
    def hasSearchFramework(*args): return apply(_Sword.SWModule_hasSearchFramework,args)
    def isSearchOptimallySupported(*args): return apply(_Sword.SWModule_isSearchOptimallySupported,args)
    def next(*args): return apply(_Sword.SWModule_next,args)
    def prev(*args): return apply(_Sword.SWModule_prev,args)
    def inc(*args): return apply(_Sword.SWModule_inc,args)
    def dec(*args): return apply(_Sword.SWModule_dec,args)
    def setPosition(*args): return apply(_Sword.SWModule_setPosition,args)
    def top(*args): return apply(_Sword.SWModule_top,args)
    def bottom(*args): return apply(_Sword.SWModule_bottom,args)
    def text(*args): return apply(_Sword.SWModule_text,args)
    def StripText(*args): return apply(_Sword.SWModule_StripText,args)
    def getRawEntry(*args): return apply(_Sword.SWModule_getRawEntry,args)
    def setSkipConsecutiveLinks(*args): return apply(_Sword.SWModule_setSkipConsecutiveLinks,args)
    def getSkipConsecutiveLinks(*args): return apply(_Sword.SWModule_getSkipConsecutiveLinks,args)
    def getEntryAttributes(*args): return apply(_Sword.SWModule_getEntryAttributes,args)
    def processEntryAttributes(*args): return apply(_Sword.SWModule_processEntryAttributes,args)
    def isProcessEntryAttributes(*args): return apply(_Sword.SWModule_isProcessEntryAttributes,args)
    def Name(*args): return apply(_Sword.SWModule_Name,args)
    def Description(*args): return apply(_Sword.SWModule_Description,args)
    def Type(*args): return apply(_Sword.SWModule_Type,args)
    def Direction(*args): return apply(_Sword.SWModule_Direction,args)
    def Encoding(*args): return apply(_Sword.SWModule_Encoding,args)
    def Markup(*args): return apply(_Sword.SWModule_Markup,args)
    def Lang(*args): return apply(_Sword.SWModule_Lang,args)
    def isWritable(*args): return apply(_Sword.SWModule_isWritable,args)
    __swig_getmethods__["createModule"] = lambda x: _Sword.SWModule_createModule
    if _newclass:createModule = staticmethod(_Sword.SWModule_createModule)
    def setEntry(*args): return apply(_Sword.SWModule_setEntry,args)
    def deleteEntry(*args): return apply(_Sword.SWModule_deleteEntry,args)
    def write(*args): return apply(_Sword.SWModule_write,args)
    def writeLink(*args): return apply(_Sword.SWModule_writeLink,args)
    def __del__(self, destroy= _Sword.delete_SWModule):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SWModule instance at %s>" % (self.this,)

class SWModulePtr(SWModule):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWModule
_Sword.SWModule_swigregister(SWModulePtr)
SWModule_nullPercent = _Sword.SWModule_nullPercent

SWModule_createModule = _Sword.SWModule_createModule


class SW_POSITION(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SW_POSITION, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SW_POSITION, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SW_POSITION,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_SW_POSITION):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SW_POSITION instance at %s>" % (self.this,)

class SW_POSITIONPtr(SW_POSITION):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SW_POSITION
_Sword.SW_POSITION_swigregister(SW_POSITIONPtr)

class SWKey(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWKey, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SWKey, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SWKey,args)
        self.thisown = 1
    def clone(*args): return apply(_Sword.SWKey_clone,args)
    def Persist(*args): return apply(_Sword.SWKey_Persist,args)
    def setPersist(*args): return apply(_Sword.SWKey_setPersist,args)
    def Error(*args): return apply(_Sword.SWKey_Error,args)
    def setText(*args): return apply(_Sword.SWKey_setText,args)
    def getText(*args): return apply(_Sword.SWKey_getText,args)
    def getShortText(*args): return apply(_Sword.SWKey_getShortText,args)
    def compare(*args): return apply(_Sword.SWKey_compare,args)
    def equals(*args): return apply(_Sword.SWKey_equals,args)
    def decrement(*args): return apply(_Sword.SWKey_decrement,args)
    def increment(*args): return apply(_Sword.SWKey_increment,args)
    def Traversable(*args): return apply(_Sword.SWKey_Traversable,args)
    def Index(*args): return apply(_Sword.SWKey_Index,args)
    def next(*args): return apply(_Sword.SWKey_next,args)
    def prev(*args): return apply(_Sword.SWKey_prev,args)
    def setKey(*args): return apply(_Sword.SWKey_setKey,args)
    def __del__(self, destroy= _Sword.delete_SWKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SWKey instance at %s>" % (self.this,)

class SWKeyPtr(SWKey):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWKey
_Sword.SWKey_swigregister(SWKeyPtr)

class VerseKey(SWKey):
    __swig_setmethods__ = {}
    for _s in [SWKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, VerseKey, name, value)
    __swig_getmethods__ = {}
    for _s in [SWKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, VerseKey, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_VerseKey,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_VerseKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def clone(*args): return apply(_Sword.VerseKey_clone,args)
    def LowerBound(*args): return apply(_Sword.VerseKey_LowerBound,args)
    def UpperBound(*args): return apply(_Sword.VerseKey_UpperBound,args)
    def ClearBounds(*args): return apply(_Sword.VerseKey_ClearBounds,args)
    def decrement(*args): return apply(_Sword.VerseKey_decrement,args)
    def increment(*args): return apply(_Sword.VerseKey_increment,args)
    def Traversable(*args): return apply(_Sword.VerseKey_Traversable,args)
    def getBookName(*args): return apply(_Sword.VerseKey_getBookName,args)
    def getBookAbbrev(*args): return apply(_Sword.VerseKey_getBookAbbrev,args)
    def Testament(*args): return apply(_Sword.VerseKey_Testament,args)
    def Book(*args): return apply(_Sword.VerseKey_Book,args)
    def Chapter(*args): return apply(_Sword.VerseKey_Chapter,args)
    def Verse(*args): return apply(_Sword.VerseKey_Verse,args)
    def Normalize(*args): return apply(_Sword.VerseKey_Normalize,args)
    def AutoNormalize(*args): return apply(_Sword.VerseKey_AutoNormalize,args)
    def Headings(*args): return apply(_Sword.VerseKey_Headings,args)
    def getOSISRef(*args): return apply(_Sword.VerseKey_getOSISRef,args)
    def compare(*args): return apply(_Sword.VerseKey_compare,args)
    def _compare(*args): return apply(_Sword.VerseKey__compare,args)
    def setLocale(*args): return apply(_Sword.VerseKey_setLocale,args)
    def getLocale(*args): return apply(_Sword.VerseKey_getLocale,args)
    def __repr__(self):
        return "<C VerseKey instance at %s>" % (self.this,)

class VerseKeyPtr(VerseKey):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = VerseKey
_Sword.VerseKey_swigregister(VerseKeyPtr)

class ListKey(SWKey):
    __swig_setmethods__ = {}
    for _s in [SWKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ListKey, name, value)
    __swig_getmethods__ = {}
    for _s in [SWKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ListKey, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_ListKey,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_ListKey):
        try:
            if self.thisown: destroy(self)
        except: pass
    def clone(*args): return apply(_Sword.ListKey_clone,args)
    def ClearList(*args): return apply(_Sword.ListKey_ClearList,args)
    def Count(*args): return apply(_Sword.ListKey_Count,args)
    def Remove(*args): return apply(_Sword.ListKey_Remove,args)
    def SetToElement(*args): return apply(_Sword.ListKey_SetToElement,args)
    def GetElement(*args): return apply(_Sword.ListKey_GetElement,args)
    def add(*args): return apply(_Sword.ListKey_add,args)
    def copyFrom(*args): return apply(_Sword.ListKey_copyFrom,args)
    def setPosition(*args): return apply(_Sword.ListKey_setPosition,args)
    def decrement(*args): return apply(_Sword.ListKey_decrement,args)
    def increment(*args): return apply(_Sword.ListKey_increment,args)
    def Traversable(*args): return apply(_Sword.ListKey_Traversable,args)
    def Index(*args): return apply(_Sword.ListKey_Index,args)
    def __repr__(self):
        return "<C ListKey instance at %s>" % (self.this,)

class ListKeyPtr(ListKey):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = ListKey
_Sword.ListKey_swigregister(ListKeyPtr)

class TreeKey(SWKey):
    __swig_setmethods__ = {}
    for _s in [SWKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, TreeKey, name, value)
    __swig_getmethods__ = {}
    for _s in [SWKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, TreeKey, name)
    def getLocalName(*args): return apply(_Sword.TreeKey_getLocalName,args)
    def setLocalName(*args): return apply(_Sword.TreeKey_setLocalName,args)
    def getUserData(*args): return apply(_Sword.TreeKey_getUserData,args)
    def setUserData(*args): return apply(_Sword.TreeKey_setUserData,args)
    def getFullName(*args): return apply(_Sword.TreeKey_getFullName,args)
    def root(*args): return apply(_Sword.TreeKey_root,args)
    def parent(*args): return apply(_Sword.TreeKey_parent,args)
    def firstChild(*args): return apply(_Sword.TreeKey_firstChild,args)
    def nextSibling(*args): return apply(_Sword.TreeKey_nextSibling,args)
    def previousSibling(*args): return apply(_Sword.TreeKey_previousSibling,args)
    def hasChildren(*args): return apply(_Sword.TreeKey_hasChildren,args)
    def append(*args): return apply(_Sword.TreeKey_append,args)
    def appendChild(*args): return apply(_Sword.TreeKey_appendChild,args)
    def insertBefore(*args): return apply(_Sword.TreeKey_insertBefore,args)
    def remove(*args): return apply(_Sword.TreeKey_remove,args)
    def setOffset(*args): return apply(_Sword.TreeKey_setOffset,args)
    def getOffset(*args): return apply(_Sword.TreeKey_getOffset,args)
    def setPosition(*args): return apply(_Sword.TreeKey_setPosition,args)
    def Traversable(*args): return apply(_Sword.TreeKey_Traversable,args)
    def Index(*args): return apply(_Sword.TreeKey_Index,args)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C TreeKey instance at %s>" % (self.this,)

class TreeKeyPtr(TreeKey):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TreeKey
_Sword.TreeKey_swigregister(TreeKeyPtr)

class TreeKeyIdx(TreeKey):
    __swig_setmethods__ = {}
    for _s in [TreeKey]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, TreeKeyIdx, name, value)
    __swig_getmethods__ = {}
    for _s in [TreeKey]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, TreeKeyIdx, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C TreeKeyIdx instance at %s>" % (self.this,)

class TreeKeyIdxPtr(TreeKeyIdx):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = TreeKeyIdx
_Sword.TreeKeyIdx_swigregister(TreeKeyIdxPtr)

class LocaleMgr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, LocaleMgr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, LocaleMgr, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_LocaleMgr,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_LocaleMgr):
        try:
            if self.thisown: destroy(self)
        except: pass
    def getLocale(*args): return apply(_Sword.LocaleMgr_getLocale,args)
    def getAvailableLocales(*args): return apply(_Sword.LocaleMgr_getAvailableLocales,args)
    def getDefaultLocaleName(*args): return apply(_Sword.LocaleMgr_getDefaultLocaleName,args)
    def setDefaultLocaleName(*args): return apply(_Sword.LocaleMgr_setDefaultLocaleName,args)
    __swig_getmethods__["systemLocaleMgr"] = lambda x: _Sword.LocaleMgr_systemLocaleMgr
    if _newclass:systemLocaleMgr = staticmethod(_Sword.LocaleMgr_systemLocaleMgr)
    def __repr__(self):
        return "<C LocaleMgr instance at %s>" % (self.this,)

class LocaleMgrPtr(LocaleMgr):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = LocaleMgr
_Sword.LocaleMgr_swigregister(LocaleMgrPtr)
LocaleMgr_systemLocaleMgr = _Sword.LocaleMgr_systemLocaleMgr


class SWFilterMgr(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWFilterMgr, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SWFilterMgr, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C SWFilterMgr instance at %s>" % (self.this,)

class SWFilterMgrPtr(SWFilterMgr):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWFilterMgr
_Sword.SWFilterMgr_swigregister(SWFilterMgrPtr)

class SWText(SWModule):
    __swig_setmethods__ = {}
    for _s in [SWModule]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWText, name, value)
    __swig_getmethods__ = {}
    for _s in [SWModule]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, SWText, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SWText,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_SWText):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SWText instance at %s>" % (self.this,)

class SWTextPtr(SWText):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWText
_Sword.SWText_swigregister(SWTextPtr)

class RawText(SWText):
    __swig_setmethods__ = {}
    for _s in [SWText]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, RawText, name, value)
    __swig_getmethods__ = {}
    for _s in [SWText]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, RawText, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_RawText,args)
        self.thisown = 1
    __swig_getmethods__["createModule"] = lambda x: _Sword.RawText_createModule
    if _newclass:createModule = staticmethod(_Sword.RawText_createModule)
    def __del__(self, destroy= _Sword.delete_RawText):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C RawText instance at %s>" % (self.this,)

class RawTextPtr(RawText):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = RawText
_Sword.RawText_swigregister(RawTextPtr)
RawText_createModule = _Sword.RawText_createModule


class zText(SWText):
    __swig_setmethods__ = {}
    for _s in [SWText]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, zText, name, value)
    __swig_getmethods__ = {}
    for _s in [SWText]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, zText, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_zText,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_zText):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["createModule"] = lambda x: _Sword.zText_createModule
    if _newclass:createModule = staticmethod(_Sword.zText_createModule)
    def __repr__(self):
        return "<C zText instance at %s>" % (self.this,)

class zTextPtr(zText):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = zText
_Sword.zText_swigregister(zTextPtr)
zText_createModule = _Sword.zText_createModule


class SWCom(SWModule):
    __swig_setmethods__ = {}
    for _s in [SWModule]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWCom, name, value)
    __swig_getmethods__ = {}
    for _s in [SWModule]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, SWCom, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C SWCom instance at %s>" % (self.this,)

class SWComPtr(SWCom):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWCom
_Sword.SWCom_swigregister(SWComPtr)

class RawCom(SWCom):
    __swig_setmethods__ = {}
    for _s in [SWCom]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, RawCom, name, value)
    __swig_getmethods__ = {}
    for _s in [SWCom]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, RawCom, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C RawCom instance at %s>" % (self.this,)

class RawComPtr(RawCom):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = RawCom
_Sword.RawCom_swigregister(RawComPtr)

class zCom(SWCom):
    __swig_setmethods__ = {}
    for _s in [SWCom]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, zCom, name, value)
    __swig_getmethods__ = {}
    for _s in [SWCom]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, zCom, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C zCom instance at %s>" % (self.this,)

class zComPtr(zCom):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = zCom
_Sword.zCom_swigregister(zComPtr)

class SWGenBook(SWModule):
    __swig_setmethods__ = {}
    for _s in [SWModule]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWGenBook, name, value)
    __swig_getmethods__ = {}
    for _s in [SWModule]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, SWGenBook, name)
    def __init__(self): raise RuntimeError, "No constructor defined"
    def __repr__(self):
        return "<C SWGenBook instance at %s>" % (self.this,)

class SWGenBookPtr(SWGenBook):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWGenBook
_Sword.SWGenBook_swigregister(SWGenBookPtr)

class RawGenBook(SWGenBook):
    __swig_setmethods__ = {}
    for _s in [SWGenBook]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, RawGenBook, name, value)
    __swig_getmethods__ = {}
    for _s in [SWGenBook]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, RawGenBook, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_RawGenBook,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_RawGenBook):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["createModule"] = lambda x: _Sword.RawGenBook_createModule
    if _newclass:createModule = staticmethod(_Sword.RawGenBook_createModule)
    def __repr__(self):
        return "<C RawGenBook instance at %s>" % (self.this,)

class RawGenBookPtr(RawGenBook):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = RawGenBook
_Sword.RawGenBook_swigregister(RawGenBookPtr)
RawGenBook_createModule = _Sword.RawGenBook_createModule


class SWLD(SWModule):
    __swig_setmethods__ = {}
    for _s in [SWModule]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWLD, name, value)
    __swig_getmethods__ = {}
    for _s in [SWModule]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, SWLD, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SWLD,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_SWLD):
        try:
            if self.thisown: destroy(self)
        except: pass
    def __repr__(self):
        return "<C SWLD instance at %s>" % (self.this,)

class SWLDPtr(SWLD):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWLD
_Sword.SWLD_swigregister(SWLDPtr)

class RawLD(SWLD):
    __swig_setmethods__ = {}
    for _s in [SWLD]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, RawLD, name, value)
    __swig_getmethods__ = {}
    for _s in [SWLD]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, RawLD, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_RawLD,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_RawLD):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["createModule"] = lambda x: _Sword.RawLD_createModule
    if _newclass:createModule = staticmethod(_Sword.RawLD_createModule)
    def __repr__(self):
        return "<C RawLD instance at %s>" % (self.this,)

class RawLDPtr(RawLD):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = RawLD
_Sword.RawLD_swigregister(RawLDPtr)
RawLD_createModule = _Sword.RawLD_createModule


class RawLD4(SWLD):
    __swig_setmethods__ = {}
    for _s in [SWLD]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, RawLD4, name, value)
    __swig_getmethods__ = {}
    for _s in [SWLD]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, RawLD4, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_RawLD4,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_RawLD4):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["createModule"] = lambda x: _Sword.RawLD4_createModule
    if _newclass:createModule = staticmethod(_Sword.RawLD4_createModule)
    def __repr__(self):
        return "<C RawLD4 instance at %s>" % (self.this,)

class RawLD4Ptr(RawLD4):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = RawLD4
_Sword.RawLD4_swigregister(RawLD4Ptr)
RawLD4_createModule = _Sword.RawLD4_createModule


class zLD(SWLD):
    __swig_setmethods__ = {}
    for _s in [SWLD]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, zLD, name, value)
    __swig_getmethods__ = {}
    for _s in [SWLD]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, zLD, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_zLD,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_zLD):
        try:
            if self.thisown: destroy(self)
        except: pass
    __swig_getmethods__["createModule"] = lambda x: _Sword.zLD_createModule
    if _newclass:createModule = staticmethod(_Sword.zLD_createModule)
    def __repr__(self):
        return "<C zLD instance at %s>" % (self.this,)

class zLDPtr(zLD):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = zLD
_Sword.zLD_swigregister(zLDPtr)
zLD_createModule = _Sword.zLD_createModule


class SWCompress(_object):
    __swig_setmethods__ = {}
    __setattr__ = lambda self, name, value: _swig_setattr(self, SWCompress, name, value)
    __swig_getmethods__ = {}
    __getattr__ = lambda self, name: _swig_getattr(self, SWCompress, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_SWCompress,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_SWCompress):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Buf(*args): return apply(_Sword.SWCompress_Buf,args)
    def zBuf(*args): return apply(_Sword.SWCompress_zBuf,args)
    def GetChars(*args): return apply(_Sword.SWCompress_GetChars,args)
    def SendChars(*args): return apply(_Sword.SWCompress_SendChars,args)
    def Encode(*args): return apply(_Sword.SWCompress_Encode,args)
    def Decode(*args): return apply(_Sword.SWCompress_Decode,args)
    def __repr__(self):
        return "<C SWCompress instance at %s>" % (self.this,)

class SWCompressPtr(SWCompress):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = SWCompress
_Sword.SWCompress_swigregister(SWCompressPtr)

class LZSSCompress(SWCompress):
    __swig_setmethods__ = {}
    for _s in [SWCompress]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, LZSSCompress, name, value)
    __swig_getmethods__ = {}
    for _s in [SWCompress]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, LZSSCompress, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_LZSSCompress,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_LZSSCompress):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Encode(*args): return apply(_Sword.LZSSCompress_Encode,args)
    def Decode(*args): return apply(_Sword.LZSSCompress_Decode,args)
    def __repr__(self):
        return "<C LZSSCompress instance at %s>" % (self.this,)

class LZSSCompressPtr(LZSSCompress):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = LZSSCompress
_Sword.LZSSCompress_swigregister(LZSSCompressPtr)

class ZipCompress(SWCompress):
    __swig_setmethods__ = {}
    for _s in [SWCompress]: __swig_setmethods__.update(_s.__swig_setmethods__)
    __setattr__ = lambda self, name, value: _swig_setattr(self, ZipCompress, name, value)
    __swig_getmethods__ = {}
    for _s in [SWCompress]: __swig_getmethods__.update(_s.__swig_getmethods__)
    __getattr__ = lambda self, name: _swig_getattr(self, ZipCompress, name)
    def __init__(self,*args):
        self.this = apply(_Sword.new_ZipCompress,args)
        self.thisown = 1
    def __del__(self, destroy= _Sword.delete_ZipCompress):
        try:
            if self.thisown: destroy(self)
        except: pass
    def Encode(*args): return apply(_Sword.ZipCompress_Encode,args)
    def Decode(*args): return apply(_Sword.ZipCompress_Decode,args)
    def __repr__(self):
        return "<C ZipCompress instance at %s>" % (self.this,)

class ZipCompressPtr(ZipCompress):
    def __init__(self,this):
        self.this = this
        if not hasattr(self,"thisown"): self.thisown = 0
        self.__class__ = ZipCompress
_Sword.ZipCompress_swigregister(ZipCompressPtr)


