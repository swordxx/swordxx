# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

#SET(NDK_DEBUG 0)
SET(CMAKE_C_FLAGS "-D__unix__ ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-D_FTPLIB_NO_COMPAT ${CMAKE_C_FLAGS}")

SET(CMAKE_C_FLAGS "-Wno-invalid-source-encoding ${CMAKE_C_FLAGS}")


SET(CMAKE_C_FLAGS "-DOS_ANDROID ${CMAKE_C_FLAGS}")

SET(CMAKE_C_FLAGS "-fvisibility=hidden ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-DSTDC_HEADERS ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-Dunix ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-DANDROID ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-DEXCLUDEBZIP2 ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-DEXCLUDEXZ ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS "-DEXCLUDEXZ ${CMAKE_C_FLAGS}")

#SET(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS}")
#SET(CMAKE_C_FLAGS " ${CMAKE_C_FLAGS}")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")

add_library( # Sets the name of the library.
        sword

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        src/main/cpp/jni/swordstub.cpp
        ../../../../src/utilfuns/swversion.cpp
        ../../../../src/modules/comments/zcom/zcom.cpp
        ../../../../src/modules/comments/zcom4/zcom4.cpp
        ../../../../src/modules/comments/rawfiles/rawfiles.cpp
        ../../../../src/modules/comments/rawcom4/rawcom4.cpp
        ../../../../src/modules/comments/rawcom/rawcom.cpp
        ../../../../src/modules/comments/swcom.cpp
        ../../../../src/modules/comments/hrefcom/hrefcom.cpp
        ../../../../src/modules/swmodule.cpp
        ../../../../src/modules/tests/echomod.cpp
        ../../../../src/modules/genbook/swgenbook.cpp
        ../../../../src/modules/genbook/rawgenbook/rawgenbook.cpp
        ../../../../src/modules/lexdict/swld.cpp
        ../../../../src/modules/lexdict/rawld4/rawld4.cpp
        ../../../../src/modules/lexdict/zld/zld.cpp
        ../../../../src/modules/lexdict/rawld/rawld.cpp
        ../../../../src/modules/texts/rawtext/rawtext.cpp
        ../../../../src/modules/texts/rawtext4/rawtext4.cpp
        ../../../../src/modules/texts/swtext.cpp
        ../../../../src/modules/texts/ztext/ztext.cpp
        ../../../../src/modules/texts/ztext4/ztext4.cpp
        ../../../../src/modules/common/rawstr4.cpp
        ../../../../src/modules/common/lzsscomprs.cpp
        ../../../../src/modules/common/zipcomprs.cpp
        ../../../../src/modules/common/rawverse4.cpp
        ../../../../src/modules/common/swcipher.cpp
        ../../../../src/modules/common/swcomprs.cpp
        ../../../../src/modules/common/rawverse.cpp
        ../../../../src/modules/common/sapphire.cpp
        ../../../../src/modules/common/zstr.cpp
        ../../../../src/modules/common/entriesblk.cpp
        ../../../../src/modules/common/zverse.cpp
        ../../../../src/modules/common/zverse4.cpp
        ../../../../src/modules/common/rawstr.cpp
        ../../../../src/modules/filters/gbfwordjs.cpp
        ../../../../src/modules/filters/utf8latin1.cpp
        ../../../../src/modules/filters/utf8greekaccents.cpp
        ../../../../src/modules/filters/utf16utf8.cpp
        ../../../../src/modules/filters/gbfwebif.cpp
        ../../../../src/modules/filters/utf8transliterator.cpp
        ../../../../src/modules/filters/gbfstrongs.cpp
        ../../../../src/modules/filters/thmlhtmlhref.cpp
        ../../../../src/modules/filters/thmlxhtml.cpp
        ../../../../src/modules/filters/thmlgbf.cpp
        ../../../../src/modules/filters/utf8utf16.cpp
        ../../../../src/modules/filters/utf8cantillation.cpp
        ../../../../src/modules/filters/utf8arshaping.cpp
        ../../../../src/modules/filters/cipherfil.cpp
        ../../../../src/modules/filters/thmlheadings.cpp
        ../../../../src/modules/filters/thmlscripref.cpp
        ../../../../src/modules/filters/latin1utf8.cpp
        ../../../../src/modules/filters/gbfhtml.cpp
        ../../../../src/modules/filters/thmlosis.cpp
        ../../../../src/modules/filters/utf8nfkd.cpp
        ../../../../src/modules/filters/thmlstrongs.cpp
        ../../../../src/modules/filters/osisenum.cpp
        ../../../../src/modules/filters/osisfootnotes.cpp
        ../../../../src/modules/filters/osisglosses.cpp
        ../../../../src/modules/filters/osisheadings.cpp
        ../../../../src/modules/filters/osishtmlhref.cpp
        ../../../../src/modules/filters/osislemma.cpp
        ../../../../src/modules/filters/osismorph.cpp
        ../../../../src/modules/filters/osismorphsegmentation.cpp
        ../../../../src/modules/filters/osisosis.cpp
        ../../../../src/modules/filters/osisplain.cpp
        ../../../../src/modules/filters/osisredletterwords.cpp
        ../../../../src/modules/filters/osisrtf.cpp
        ../../../../src/modules/filters/osisscripref.cpp
        ../../../../src/modules/filters/osisstrongs.cpp
        ../../../../src/modules/filters/osisvariants.cpp
        ../../../../src/modules/filters/osiswebif.cpp
        ../../../../src/modules/filters/osiswordjs.cpp
        ../../../../src/modules/filters/osisxhtml.cpp
        ../../../../src/modules/filters/osisxlit.cpp
        ../../../../src/modules/filters/osisreferencelinks.cpp
        ../../../../src/modules/filters/thmlmorph.cpp
        ../../../../src/modules/filters/gbfplain.cpp
        ../../../../src/modules/filters/gbfhtmlhref.cpp
        ../../../../src/modules/filters/gbfxhtml.cpp
        ../../../../src/modules/filters/utf8html.cpp
        ../../../../src/modules/filters/utf8nfc.cpp
        ../../../../src/modules/filters/rtfhtml.cpp
        ../../../../src/modules/filters/gbfredletterwords.cpp
        ../../../../src/modules/filters/latin1utf16.cpp
        ../../../../src/modules/filters/thmlhtml.cpp
        ../../../../src/modules/filters/gbfthml.cpp
        ../../../../src/modules/filters/teihtmlhref.cpp
        ../../../../src/modules/filters/teixhtml.cpp
        ../../../../src/modules/filters/gbfrtf.cpp
        ../../../../src/modules/filters/gbfosis.cpp
        ../../../../src/modules/filters/teirtf.cpp
        ../../../../src/modules/filters/thmlwordjs.cpp
        ../../../../src/modules/filters/papyriplain.cpp
        ../../../../src/modules/filters/utf8bidireorder.cpp
        ../../../../src/modules/filters/gbfheadings.cpp
        ../../../../src/modules/filters/thmlrtf.cpp
        ../../../../src/modules/filters/swoptfilter.cpp
        ../../../../src/modules/filters/utf8arabicpoints.cpp
        ../../../../src/modules/filters/unicodertf.cpp
        ../../../../src/modules/filters/gbffootnotes.cpp
        ../../../../src/modules/filters/greeklexattribs.cpp
        ../../../../src/modules/filters/thmlfootnotes.cpp
        ../../../../src/modules/filters/thmlplain.cpp
        ../../../../src/modules/filters/utf8hebrewpoints.cpp
        ../../../../src/modules/filters/thmlwebif.cpp
        ../../../../src/modules/filters/thmlvariants.cpp
        ../../../../src/modules/filters/thmllemma.cpp
        ../../../../src/modules/filters/gbfmorph.cpp
        ../../../../src/modules/filters/teiplain.cpp
        ../../../../src/modules/filters/swbasicfilter.cpp
        ../../../../src/modules/filters/scsuutf8.cpp
        ../../../../src/modules/filters/gbflatex.cpp
        ../../../../src/modules/filters/thmllatex.cpp
        ../../../../src/modules/filters/teilatex.cpp
        ../../../../src/modules/filters/osislatex.cpp
        ../../../../src/mgr/stringmgr.cpp
        ../../../../src/mgr/swmgr.cpp
        ../../../../src/mgr/swsearchable.cpp
        ../../../../src/mgr/localemgr.cpp
        ../../../../src/mgr/swconfig.cpp
        ../../../../src/mgr/markupfiltmgr.cpp
        ../../../../src/mgr/encfiltmgr.cpp
        ../../../../src/mgr/swfiltermgr.cpp
        ../../../../src/mgr/swcacher.cpp
        ../../../../src/mgr/installmgr.cpp
        ../../../../src/mgr/swlocale.cpp
        ../../../../src/mgr/filemgr.cpp
        ../../../../src/mgr/versificationmgr.cpp
        ../../../../src/mgr/remotetrans.cpp
        ../../../../src/mgr/ftplibftpt.cpp
        ../../../../src/utilfuns/swobject.cpp
        ../../../../src/utilfuns/roman.cpp
        ../../../../src/utilfuns/swbuf.cpp
        ../../../../src/utilfuns/utilstr.cpp
        ../../../../src/utilfuns/ftplib.c
        ../../../../src/utilfuns/ftpparse.c
        ../../../../src/utilfuns/url.cpp
        ../../../../src/utilfuns/utilxml.cpp
#        ../../../../src/utilfuns/regex.c
        ../../../../src/keys/swkey.cpp
        ../../../../src/keys/versetreekey.cpp
        ../../../../src/keys/treekeyidx.cpp
        ../../../../src/keys/versekey.cpp
        ../../../../src/keys/strkey.cpp
        ../../../../src/keys/treekey.cpp
        ../../../../src/keys/listkey.cpp
        ../../../../src/frontend/swdisp.cpp
        ../../../../src/frontend/swlog.cpp
        ../../../../src/utilfuns/zlib/untgz.c

             )

include_directories(../../../../include)
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       sword

                       # Links the target library to the log library
                       # included in the NDK.
                       #${log-lib}
                       log
                       z
                       )