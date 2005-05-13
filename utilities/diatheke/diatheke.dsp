# Microsoft Developer Studio Project File - Name="diatheke" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=diatheke - Win32 Debug with ICU
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "diatheke.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "diatheke.mak" CFG="diatheke - Win32 Debug with ICU"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "diatheke - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "diatheke - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "diatheke - Win32 Debug with ICU" (based on "Win32 (x86) Console Application")
!MESSAGE "diatheke - Win32 Release with ICU" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "diatheke - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "../../include" /I "../../src/utilfuns/win32" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libswordvc6.lib /nologo /subsystem:console /machine:I386 /libpath:"../../lib"

!ELSEIF  "$(CFG)" == "diatheke - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "../../include" /I "../../src/utilfuns/win32" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libswordvc6d.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../lib"

!ELSEIF  "$(CFG)" == "diatheke - Win32 Debug with ICU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "diatheke___Win32_Debug_with_ICU"
# PROP BASE Intermediate_Dir "diatheke___Win32_Debug_with_ICU"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ICUDebug"
# PROP Intermediate_Dir "ICUDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /Gm /GX /ZI /Od /I "../../include" /I "../../src/utilfuns/win32" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__VISUALC__" /FR /YX /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GX /ZI /Od /I "../../include" /I "../../src/utilfuns/win32" /I "../../../icu-sword/include" /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "_ICU_" /D "_ICUSWORD_" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libsword.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../lib/vcppmake/vc6/Debug"
# ADD LINK32 icuin.lib icuuc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libswordvc6icud.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"../../../icu-sword/lib" /libpath:"../../lib"

!ELSEIF  "$(CFG)" == "diatheke - Win32 Release with ICU"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "diatheke___Win32_Release_with_ICU"
# PROP BASE Intermediate_Dir "diatheke___Win32_Release_with_ICU"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ICURelease"
# PROP Intermediate_Dir "ICURelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /w /W0 /GX /O2 /I "../../include" /I "../../src/utilfuns/win32" /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "__VISUALC__" /FR /YX /FD /c
# ADD CPP /nologo /MD /w /W0 /GX /O2 /I "../../include" /I "../../src/utilfuns/win32" /I "../../../icu-sword/include" /D "NDEBUG" /D "_ICU_" /D "_ICUSWORD_" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libsword.lib /nologo /subsystem:console /machine:I386 /libpath:"../../lib/vcppmake/vc6/Release"
# ADD LINK32 icuin.lib icuuc.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib libswordvc6icu.lib /nologo /subsystem:console /machine:I386 /libpath:"../../../icu-sword/lib" /libpath:"../../lib"

!ENDIF 

# Begin Target

# Name "diatheke - Win32 Release"
# Name "diatheke - Win32 Debug"
# Name "diatheke - Win32 Debug with ICU"
# Name "diatheke - Win32 Release with ICU"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\corediatheke.cpp
# End Source File
# Begin Source File

SOURCE=.\diafiltmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\diatheke.cpp
# End Source File
# Begin Source File

SOURCE=.\diathekemgr.cpp
# End Source File
# Begin Source File

SOURCE=.\gbfcgi.cpp
# End Source File
# Begin Source File

SOURCE=.\osiscgi.cpp
# End Source File
# Begin Source File

SOURCE=.\thmlcgi.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\corediatheke.h
# End Source File
# Begin Source File

SOURCE=.\diafiltmgr.h
# End Source File
# Begin Source File

SOURCE=.\diathekemgr.h
# End Source File
# Begin Source File

SOURCE=.\gbfcgi.h
# End Source File
# Begin Source File

SOURCE=.\osiscgi.h
# End Source File
# Begin Source File

SOURCE=.\thmlcgi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
