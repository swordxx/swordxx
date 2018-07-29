# NOTES: Defaults to build type of Shared
#        Forces out-of-source tree build
#
#
# This file started on 18 January 2010 by Gregory Hellings
# It is ceded to The SWORD Library developers and CrossWire under the terms
# of their own GPLv2 license and all copyright is transferred to them for
# all posterity and eternity, wherever such transfer is possible.  Where it is
# not, then this file is released under the GPLv2 by myself.
PROJECT(swordxx CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
CMAKE_POLICY(SET CMP0017 NEW)

# Set versions
SET(SWORDXX_VERSION "2.0.0")
SET(SWORDXX_VERSION_STR "2.0.0_beta1")
SET(SWORDXX_SWORD_COMPAT_VERSION "1.7.4")

MESSAGE(STATUS "Configuring libswordxx version ${SWORDXX_VERSION}")
MESSAGE(STATUS "Installation prefix: ${CMAKE_INSTALL_PREFIX}")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/helpers.cmake")
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

CalculateVersionForCode("${SWORDXX_VERSION}" SWORDXX_VERSION_C)
CalculateVersionForCode("${SWORDXX_SWORD_COMPAT_VERSION}"
                        SWORDXX_SWORD_COMPAT_VERSION_C)

# User options
SET(SWORDXX_LIB_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/lib"
    CACHE STRING "Object code library install directory. Defaults to \"\${CMAKE_INSTALL_PREFIX}/lib\"")

SET(SWORDXX_INCLUDE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/include"
    CACHE STRING "C Header files install directory. Defaults to \"\${CMAKE_INSTALL_PREFIX}/include\"")

SET(SWORDXX_BIN_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/bin"
    CACHE STRING "Directory to install binary executable files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/bin\"")

SET(SWORDXX_SYSCONF_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/etc"
    CACHE STRING "Directory to install global config files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/etc\"")

SET(SWORDXX_SHARE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/share"
    CACHE STRING "Directory to install global data files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/share\"")


# If SWORDXX_BUILDCONFIG set, load the file and fail on error. Otherwise, try to
# include either "config.cmake" under either the build directory or the source
# directory, whichever file first exists, if at all.
IF(DEFINED SWORDXX_BUILDCONFIG)
  IF(SWORDXX_BUILDCONFIG_IS_RELATIVE)
      SET(SWORDXX_BUILDCONFIG
          "${CMAKE_CURRENT_BINARY_DIR}/${SWORDXX_BUILDCONFIG}")
  ENDIF()
  INCLUDE("${SWORDXX_BUILDCONFIG}" OPTIONAL RESULT_VARIABLE r)
  IF(r)
    MESSAGE(STATUS "Included \"${SWORDXX_BUILDCONFIG}\"")
    UNSET(r)
  ELSE()
    MESSAGE(FATAL_ERROR
      "Failed to include SWORDXX_BUILDCONFIG from \"${SWORDXX_BUILDCONFIG}\"!")
  ENDIF()
ELSE()
  INCLUDE("${CMAKE_CURRENT_BINARY_DIR}/config.cmake" OPTIONAL RESULT_VARIABLE r)
  IF(r)
    MESSAGE(STATUS "Included \"${r}\"")
  ELSE()
    INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/config.cmake"
            OPTIONAL RESULT_VARIABLE r)
    IF(r)
      MESSAGE(STATUS "Included \"${r}\"")
    ENDIF()
  ENDIF()
  UNSET(r)
ENDIF()

# Make sure it's an out-of-stream build
IF(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    MESSAGE(FATAL_ERROR "Please invoke CMake from a different directory than the source.")
ENDIF()


###########################################################################################
# This will look for various libraries that libswordxx builds against.  There is no REQUIRED
# attribute added here, since all of the libraries here are not, in actuality, required.
#

NewSList(SWORDXX_CFLAGS "-fPIC")
NewList(SWORDXX_DEFINITIONS
    "SWORDXX_SYSCONF_INSTALL_PREFIX=\"${SWORDXX_SYSCONF_INSTALL_PREFIX}\"")
NewList(SWORDXX_INCLUDE_DIRS)
NewSList(SWORDXX_LDFLAGS)
NewList(SWORDXX_LINK_LIBRARIES)

# Required packages:
FIND_PACKAGE(BZIP2 REQUIRED)
ListAppend(SWORDXX_INCLUDE_DIRS   ${BZIP2_INCLUDE_DIR})
ListAppend(SWORDXX_LINK_LIBRARIES ${BZIP2_LIBRARY})

FIND_PACKAGE(XZ REQUIRED)
ListAppend(SWORDXX_INCLUDE_DIRS   ${XZ_INCLUDE_DIR})
ListAppend(SWORDXX_LINK_LIBRARIES ${XZ_LIBRARY})

FIND_PACKAGE(CURL REQUIRED)
FIND_PROGRAM(CURL_CONFIG curl-config
    DOC "The curl-config executable path, used to determine SFTP support.")
# If we can find the config script, we will run it and attempt to parse out the
# availability of SFTP support. Otherwise, we will assume the library was built
# without it.
IF(CURL_CONFIG)
    EXECUTE_PROCESS(
        COMMAND ${CURL_CONFIG} --protocols
        COMMAND grep SFTP
        COMMAND wc -l
        OUTPUT_VARIABLE CURL_CONFIG_OUTPUT
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    IF(CURL_CONFIG_OUTPUT STREQUAL "1")
        ListAppend(SWORDXX_DEFINITIONS "SWORDXX_CURL_HAS_SFTP=1")
        MESSAGE(STATUS "cURL SFTP Support: Yes")
    ELSE()
        ListAppend(SWORDXX_DEFINITIONS "SWORDXX_CURL_HAS_SFTP=0")
        MESSAGE(STATUS "cURL SFTP Support: No")
    ENDIF()
ENDIF()
ListAppend(SWORDXX_INCLUDE_DIRS   ${CURL_INCLUDE_DIRS})
ListAppend(SWORDXX_LINK_LIBRARIES ${CURL_LIBRARY})

FIND_PACKAGE(ZLIB REQUIRED)
ListAppend(SWORDXX_INCLUDE_DIRS   ${ZLIB_INCLUDE_DIR})
ListAppend(SWORDXX_LINK_LIBRARIES ${ZLIB_LIBRARY})

# Optional packages:
FIND_PACKAGE(PkgConfig)

IF(PKG_CONFIG_FOUND)
    pkg_check_modules(CLucene libclucene-core>=2.0)
ENDIF()
IF(CLucene_FOUND AND NOT SWORDXX_NO_CLUCENE STREQUAL "Yes")
    MESSAGE(STATUS "Using CLucene >2.0: yes")
    ListToSList(CLucene_CFLAGS_OTHER)
    SListAppend(SWORDXX_CFLAGS      ${CLucene_CFLAGS_OTHER})
    ListAppend(SWORDXX_INCLUDE_DIRS   ${CLucene_INCLUDE_DIRS})
    ExpandLibs("${CLucene_LIBRARY_DIRS}" "${CLucene_LIBRARIES}" CLucene_LIBS)
    ListAppend(SWORDXX_LINK_LIBRARIES ${CLucene_LIBS})
    ListToSList(CLucene_LDFLAGS_OTHER)
    SListAppend(SWORDXX_LDFLAGS     ${CLucene_LDFLAGS_OTHER})
ELSE()
    MESSAGE(FATAL_ERROR "Required CLucene >2.0 not found!")
ENDIF()

FIND_PACKAGE(ICU REQUIRED COMPONENTS data i18n uc)
ListAppend(SWORDXX_INCLUDE_DIRS   ${ICU_INCLUDE_DIRS})
ListAppend(SWORDXX_LINK_LIBRARIES ${ICU_LIBRARIES})


################################################################################
# The buildflags depend on the CMAKE_BUILD_TYPE supplied at the command line.

ADD_COMPILE_OPTIONS(
  "-Wall" "-Wextra" "-O2"
  "$<$<COMPILE_LANGUAGE:C>:-std=c99>"
  "$<$<COMPILE_LANGUAGE:CXX>:-std=c++14>"
  "$<$<CONFIG:Release>:-DNDEBUG>"
  "$<$<NOT:$<CONFIG:Release>>:-ggdb>"
  "$<$<NOT:$<CONFIG:Release>>:-fno-omit-frame-pointer>"
)
INCLUDE(CheckCCompilerFlag)
FUNCTION(CHECK_ADD_C_COMPILER_FLAG flag)
    STRING(SUBSTRING "${flag}" 1 -1 FlagName)
    STRING(REPLACE "+" "--plus--" TestName "${FlagName}")
    CHECK_C_COMPILER_FLAG("${flag}" "${TestName}")
    IF(${TestName})
        ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:C>:${flag}>")
    ENDIF()
ENDFUNCTION()
CHECK_ADD_C_COMPILER_FLAG("-Weverything")
CHECK_ADD_C_COMPILER_FLAG("-Wlogical-op")
CHECK_ADD_C_COMPILER_FLAG("-Wno-float-equal")
CHECK_ADD_C_COMPILER_FLAG("-Wno-covered-switch-default")
CHECK_ADD_C_COMPILER_FLAG("-Wno-packed")
CHECK_ADD_C_COMPILER_FLAG("-Wno-padded")
INCLUDE(CheckCXXCompilerFlag)
FUNCTION(CHECK_ADD_CXX_COMPILER_FLAG flag)
    STRING(SUBSTRING "${flag}" 1 -1 FlagName)
    STRING(REPLACE "+" "--plus--" TestName "${FlagName}")
    CHECK_CXX_COMPILER_FLAG("${flag}" "${TestName}")
    IF(${TestName})
        ADD_COMPILE_OPTIONS("$<$<COMPILE_LANGUAGE:CXX>:${flag}>")
    ENDIF()
ENDFUNCTION()
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=extra-semi")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=format")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=inconsistent-missing-destructor-override")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=non-pod-varargs")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=non-virtual-dtor")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=return-type")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=shadow")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=shadow-field")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=shadow-field-in-constructor")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=suggest-override")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=vla")
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=weak-vtables")
CHECK_ADD_CXX_COMPILER_FLAG("-Weverything")
CHECK_ADD_CXX_COMPILER_FLAG("-Wlogical-op")
CHECK_ADD_CXX_COMPILER_FLAG("-Wno-c++98-compat")
CHECK_ADD_CXX_COMPILER_FLAG("-Wno-c++98-compat-pedantic")
CHECK_ADD_CXX_COMPILER_FLAG("-Wno-covered-switch-default")
CHECK_ADD_CXX_COMPILER_FLAG("-Wno-float-equal")
CHECK_ADD_CXX_COMPILER_FLAG("-Wno-packed")
CHECK_ADD_CXX_COMPILER_FLAG("-Wno-padded")
CHECK_ADD_CXX_COMPILER_FLAG("-Wsuggest-override")
CHECK_ADD_CXX_COMPILER_FLAG("-Wzero-as-null-pointer-constant")


##############################################################################################
# Subdirectories:
#

SET(SWORD_BUILD_INCLUDES
    "${CMAKE_CURRENT_BINARY_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/")

ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/src/swordxx")
ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/examples")
ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/tests")
ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/utilities")
