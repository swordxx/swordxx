# NOTES: Defaults to build type of Shared
#        Forces out-of-source tree build
#
#
# This file started on 18 January 2010 by Gregory Hellings
# It is ceded to The SWORD Library developers and CrossWire under the terms
# of their own GPLv2 license and all copyright is transferred to them for
# all posterity and eternity, wherever such transfer is possible.  Where it is
# not, then this file is released under the GPLv2 by myself.

CMAKE_MINIMUM_REQUIRED(VERSION 3.14)

PROJECT(swordxx VERSION 2.0.0 LANGUAGES CXX C)
SET(SWORDXX_VERSION "${PROJECT_VERSION}")
SET(SWORDXX_VERSION_STR "${SWORDXX_VERSION}_beta1")
SET(SWORDXX_SWORD_COMPAT_VERSION "1.7.4")

CMAKE_POLICY(SET CMP0017 NEW)

# Make sure it's an out-of-stream build
IF(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
    MESSAGE(FATAL_ERROR
            "Please invoke CMake from a different directory than the source.")
ENDIF()

MESSAGE(STATUS "Configuring libswordxx version ${SWORDXX_VERSION_STR}")

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/helpers.cmake")

CalculateVersionForCode("${SWORDXX_VERSION}" SWORDXX_VERSION_C)


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

# User options:
MESSAGE(STATUS "Installation prefix: ${CMAKE_INSTALL_PREFIX}")
MACRO(SwordxxDefineCacheVar name type default description)
    IF(NOT DEFINED "${name}")
        SET("${name}" "${default}" CACHE "${type}" "${description}")
    ENDIF()
    MESSAGE(STATUS "CACHE ${name}:${type} = \"${${name}}\"")
ENDMACRO()

# User options
SwordxxDefineCacheVar(SWORDXX_LIB_INSTALL_PREFIX STRING
    "${CMAKE_INSTALL_PREFIX}/lib"
    "Object code library install directory. Defaults to \"\${CMAKE_INSTALL_PREFIX}/lib\"")
SwordxxDefineCacheVar(SWORDXX_INCLUDE_INSTALL_PREFIX STRING
    "${CMAKE_INSTALL_PREFIX}/include"
    "C Header files install directory. Defaults to \"\${CMAKE_INSTALL_PREFIX}/include\"")
SwordxxDefineCacheVar(SWORDXX_BIN_INSTALL_PREFIX STRING
    "${CMAKE_INSTALL_PREFIX}/bin"
    "Directory to install binary executable files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/bin\"")
SwordxxDefineCacheVar(SWORDXX_SYSCONF_INSTALL_PREFIX STRING
    "${CMAKE_INSTALL_PREFIX}/etc"
    "Directory to install global config files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/etc\"")
SwordxxDefineCacheVar(SWORDXX_SHARE_INSTALL_PREFIX STRING
    "${CMAKE_INSTALL_PREFIX}/share"
    "Directory to install global data files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/share\"")
SwordxxDefineCacheVar(SWORDXX_MAN_INSTALL_PREFIX STRING
    "${SWORDXX_SHARE_INSTALL_PREFIX}/man"
    "Directory to install manual pages to. Defaults to \"\${SWORDXX_SHARE_INSTALL_PREFIX}/man\"")
SwordxxDefineCacheVar(SWORDXX_WITH_MAN_PAGES BOOL TRUE
    "Specifies whether man pages for Sword++ are generated and installed.")


################################################################################
# The buildflags depend on the CMAKE_BUILD_TYPE supplied at the command line.

ADD_COMPILE_OPTIONS(
  "-Wall" "-Wextra" "-O2"
  "$<$<CONFIG:Release>:-DNDEBUG>"
  "$<$<NOT:$<CONFIG:Release>>:-ggdb>"
  "$<$<NOT:$<CONFIG:Release>>:-fno-omit-frame-pointer>"
)
SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
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
CHECK_ADD_CXX_COMPILER_FLAG("-Werror=type-limits")
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

################################################################################
# Manual pages:
#
FILE(GLOB SWORDXX_MAN1_PAGES "${CMAKE_CURRENT_SOURCE_DIR}/man/*.1")
ADD_CUSTOM_TARGET("manfiles" SOURCES ${SWORDXX_MAN1_PAGES})
IF(SWORDXX_WITH_MAN_PAGES)
    INSTALL(FILES ${SWORDXX_MAN1_PAGES} DESTINATION "${SWORDXX_MAN_INSTALL_PREFIX}/man1")
ENDIF()
