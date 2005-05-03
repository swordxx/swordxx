# Microsoft Developer Studio Project File - Name="libsword" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libsword - Win32 Debug with ICU
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libsword.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libsword.mak" CFG="libsword - Win32 Debug with ICU"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libsword - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libsword - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "libsword - Win32 Debug with ICU" (based on "Win32 (x86) Static Library")
!MESSAGE "libsword - Win32 Release with ICU" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libsword - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "../../../apps/windoze/CBuilder5/InstallMgr/curl" /I "../../../include" /I "../../../src/utilfuns/win32" /I "../../../apps/windoze/CBuilder5/InstallMgr/curl/include" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "REGEX_MALLOC" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\libswordvc6.lib"

!ELSEIF  "$(CFG)" == "libsword - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "../../../include" /I "../../../src/utilfuns/win32" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "REGEX_MALLOC" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\libswordvc6d.lib"

!ELSEIF  "$(CFG)" == "libsword - Win32 Debug with ICU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libsword___Win32_Debug_with_ICU"
# PROP BASE Intermediate_Dir "libsword___Win32_Debug_with_ICU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ICUDebug"
# PROP Intermediate_Dir "ICUDebug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /Gm /GX /ZI /Od /I "../../../src/utilfuns/win32" /I "../../../include" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "REGEX_MALLOC" /D "__VISUALC__" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "../../../../icu-sword/source/common" /I "../../../../icu-sword/source/i18n" /I "../../../include" /I "../../../src/utilfuns/win32" /D "_DEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "REGEX_MALLOC" /D "_ICU_" /D "_ICUSWORD_" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\libswordvc6icud.lib"

!ELSEIF  "$(CFG)" == "libsword - Win32 Release with ICU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libsword___Win32_Release_with_ICU"
# PROP BASE Intermediate_Dir "libsword___Win32_Release_with_ICU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ICURelease"
# PROP Intermediate_Dir "ICURelease"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /GX /O2 /I "../../../include" /I "../../../src/utilfuns/win32" /D "NDEBUG" /D "WIN32" /D "_MBCS" /D "_LIB" /D "REGEX_MALLOC" /D "__VISUALC__" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "../../../../icu-sword/include" /I "../../../include" /I "../../../src/utilfuns/win32" /I "../../../apps/windoze/CBuilder5/InstallMgr/curl/include" /D "NDEBUG" /D "_ICU_" /D "WIN32" /D "_MBCS" /D "_LIB" /D "REGEX_MALLOC" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\libswordvc6icu.lib"

!ENDIF 

# Begin Target

# Name "libsword - Win32 Release"
# Name "libsword - Win32 Debug"
# Name "libsword - Win32 Debug with ICU"
# Name "libsword - Win32 Release with ICU"
# Begin Group "Sword Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\cipherfil.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\win32\dirent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\encfiltmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\entriesblk.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\filemgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\ftpparse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbffootnotes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfheadings.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfhtml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfhtmlhref.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfmorph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfosis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfplain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfredletterwords.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfrtf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfstrongs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfthml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\gbfwebif.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\keys\genarray.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\Greek2Greek.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\greeklexattribs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\comments\hrefcom\hrefcom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\installmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\latin1utf16.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\latin1utf8.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\keys\listkey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\localemgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\lzsscomprs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\markupfiltmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osisfootnotes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osisheadings.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osishtmlhref.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osislemma.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osismorph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osisplain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osisredletterwords.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osisrtf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osisscripref.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\osisstrongs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\plainfootnotes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\plainhtml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\comments\rawcom\rawcom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\comments\rawfiles\rawfiles.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\genbook\rawgenbook\rawgenbook.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\lexdict\rawld\rawld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\lexdict\rawld4\rawld4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\rawstr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\rawstr4.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\texts\rawtext\rawtext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\rawverse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\regex.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\roman.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\rtfhtml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\sapphire.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\scsuutf8.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\keys\strkey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\swbasicfilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\swbuf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\swcacher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\swcipher.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\comments\swcom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\swcomprs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\swconfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\frontend\swdisp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\swfiltermgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\genbook\swgenbook.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\keys\swkey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\lexdict\swld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\swlocale.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\frontend\swlog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\swmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\swmodule.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\swoptfilter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\swsearchable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\mgr\swsourcemgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\texts\swtext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\swunicod.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\swversion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlfootnotes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlgbf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlheadings.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlhtml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlhtmlhref.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmllemma.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlmorph.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlosis.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlplain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlrtf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlscripref.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlstrongs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlvariants.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\thmlwebif.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\keys\treekey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\keys\treekeyidx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\unicodertf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\untgz.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\url.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf16utf8.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8arshaping.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8bidireorder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8cantillation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8greekaccents.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8hebrewpoints.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8html.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8latin1.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8nfc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8nfkd.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8transliterator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\filters\utf8utf16.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\utilconf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\utilstr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\utilweb.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\utilxml.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\keys\versekey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\comments\zcom\zcom.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\zipcomprs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\lexdict\zld\zld.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\zstr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\texts\ztext\ztext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\src\utilfuns\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\modules\common\zverse.cpp
# End Source File
# End Group
# Begin Group "Sword Header Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\include\apocrypha.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\canon.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\cdsourcemgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\cipherfil.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\defs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\echomod.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\encfiltmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\entriesblk.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\femain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\filemgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\flatapi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ftpparse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbffootnotes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfheadings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfhtml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfhtmlhref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfmorph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfosis.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfplain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfredletterwords.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfrtf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfstrongs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfthml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\gbfwebif.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\Greek2Greek.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\GreekChars.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\greeklexattribs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\hebrewmcim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\hrefcom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\installmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\latin1utf16.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\latin1utf8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\listkey.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\localemgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\lzsscomprs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\markupfiltmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\multimapwdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\nullim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osisfootnotes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osisheadings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osishtmlhref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osislemma.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osismorph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osisplain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osisredletterwords.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osisrtf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osisscripref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\osisstrongs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\plainfootnotes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\plainhtml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawcom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawfiles.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawgenbook.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawld4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawstr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawstr4.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawtext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rawverse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\regex.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\remotesourcemgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\roman.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\rtfhtml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\sapphire.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\scsuutf8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\strkey.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swbasicfilter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swcacher.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swcipher.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swcom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swcomprs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swconfig.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swdisp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swdisprtf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swdisprtfchap.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swfilter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swfiltermgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swgenbook.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swinputmeth.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swinstallmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swkey.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swlocale.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swlog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swmacs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swmodule.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swobject.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swoptfilter.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swremotemgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swsearchable.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swsourcemgr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swtext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swunicod.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swversion.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\swwinlog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\sysdata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\tbdisp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlfootnotes.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlgbf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlheadings.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlhtml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlhtmlhref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmllemma.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlmorph.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlosis.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlplain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlrtf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlscripref.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlstrongs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlvariants.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\thmlwebif.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\treekey.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\treekeyidx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\unicodertf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\unixstr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\untgz.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\url.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf16utf8.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8arshaping.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8bidireorder.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8cantillation.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8greekaccents.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8hebrewpoints.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8html.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8latin1.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8nfc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8nfkd.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8transliterator.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utf8utf16.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utilconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utilfuns.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utilstr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utilweb.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\utilxml.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\versekey.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\zcom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\zconf.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\zipcomprs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\zld.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\zlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\zstr.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\ztext.h
# End Source File
# Begin Source File

SOURCE=..\..\..\include\zverse.h
# End Source File
# End Group
# End Target
# End Project
