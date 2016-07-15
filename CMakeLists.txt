# TODO: write FindICU (icu-config only for 2.2 and up) -- currently taken from another CMake system
#       use BUILD_SHARED_LIBS
#
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
CMAKE_MINIMUM_REQUIRED(VERSION 3.3)
CMAKE_POLICY(SET CMP0017 NEW)

# Set versions
SET(SWORDXX_VERSION "2.0.0")
SET(SWORDXX_VERSION_STR "2.0.0_beta1")
SET(SWORDXX_SWORD_COMPAT_VERSION "1.7.4")

MESSAGE(STATUS "Configuring libswordxx version ${SWORDXX_VERSION}")

SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

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

# Calculate version strings for code:
FUNCTION(CalculateVersionForCode in out)
    SET(hexdigit "[0123456789abcdefABCDEF]")
    SET(hexnumber "0|[123456789abcdefABCDEF]${hexdigit}*")
    SET(hexversion "^(${hexnumber})\\.(${hexnumber})\\.(${hexnumber})$")
    STRING(REGEX MATCHALL "${hexversion}" r "${in}")
    IF(NOT r)
      MESSAGE(FATAL_ERROR "Invalid version string in \${SWORDXX_VERSION}!")
    ENDIF()
    UNSET(r)
    SET(major "${CMAKE_MATCH_1}")
    SET(minor "${CMAKE_MATCH_2}")
    SET(patch "${CMAKE_MATCH_3}")
    SET("${out}" "(((${major}*65536u)+${minor})*65536u)+${patch}" PARENT_SCOPE)
ENDFUNCTION()
CalculateVersionForCode("${SWORDXX_VERSION}" SWORDXX_VERSION_C)
CalculateVersionForCode("${SWORDXX_SWORD_COMPAT_VERSION}"
                        SWORDXX_SWORD_COMPAT_VERSION_C)

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

# Required packages:
FIND_PACKAGE(BZIP2 REQUIRED)
FIND_PACKAGE(XZ REQUIRED)
FIND_PACKAGE(CURL REQUIRED)
FIND_PACKAGE(ZLIB REQUIRED)

# Optional packages:
FIND_PACKAGE(CLucene)
IF(CLucene_FOUND AND NOT SWORDXX_NO_CLUCENE STREQUAL "Yes")
    MESSAGE(STATUS "Using CLucene: yes")
    SET(SWORDXX_WITH_CLUCENE 1)
ELSE()
    MESSAGE(STATUS "Using CLucene: no")
    SET(SWORDXX_WITH_CLUCENE 0)
ENDIF()

FIND_PACKAGE(ICU)
IF(ICU_FOUND AND NOT SWORDXX_NO_ICU STREQUAL "No")
    MESSAGE(STATUS "Using ICU: yes")
    SET(SWORDXX_WITH_ICU 1)
ELSE()
    MESSAGE(STATUS "Using ICU: no")
    SET(SWORDXX_WITH_ICU 0)
ENDIF()


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


################################################################################################
# This actually creates the build target that is the libswordxx building target to be generated.
# Most of the work for configuration is done above, already.
#
# I want to do this manually, there might be reason in the future

# Sources:
FILE(GLOB_RECURSE SWORDXX_C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
FILE(GLOB_RECURSE SWORDXX_CXX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
SET(SWORDXX_SOURCES ${SWORDXX_C_SOURCES} ${SWORDXX_CXX_SOURCES})

IF((NOT DEFINED SWORDXX_INSTALL_SHARED) AND (NOT DEFINED SWORDXX_INSTALL_STATIC))
    SET(SWORDXX_INSTALL_SHARED TRUE)
ELSEIF((NOT SWORDXX_INSTALL_SHARED) AND (NOT SWORDXX_INSTALL_STATIC))
    MESSAGE(FATAL_ERROR "Neither of SWORDXX_INSTALL_SHARED and SWORDXX_INSTALL_STATIC is enabled!")
ENDIF()

ADD_LIBRARY(swordxx_objects OBJECT ${SWORDXX_SOURCES})
SET_TARGET_PROPERTIES(swordxx_objects PROPERTIES COMPILE_FLAGS "-fPIC")
SET(swordxx_objects_DEFINITIONS
    "GLOBCONFPATH=\"${SWORDXX_SYSCONF_INSTALL_PREFIX}/swordxx.conf\"")
IF(SWORDXX_WITH_CLUCENE)
    LIST(APPEND swordxx_objects_DEFINITIONS "SWORDXX_HAS_CLUCENE=1")
ENDIF()
IF(SWORDXX_WITH_ICU)
    LIST(APPEND swordxx_objects_DEFINITIONS "SWORDXX_HAS_ICU=1")
ENDIF()
SET_TARGET_PROPERTIES(swordxx_objects PROPERTIES COMPILE_DEFINITIONS
    "${swordxx_objects_DEFINITIONS}")
UNSET(swordxx_objects_DEFINITIONS)

IF(SWORDXX_INSTALL_STATIC)
    ADD_LIBRARY(swordxx_static STATIC $<TARGET_OBJECTS:swordxx_objects>)
    SET_TARGET_PROPERTIES(swordxx_static PROPERTIES OUTPUT_NAME "swordxx")
    SET(SWORDXX_PREFERRED_LIBRARY swordxx_static)
    MESSAGE(STATUS "Building static library: yes")
ELSE()
    MESSAGE(STATUS "Building static library: no")
ENDIF()

IF(SWORDXX_INSTALL_SHARED)
    ADD_LIBRARY(swordxx_dynamic SHARED $<TARGET_OBJECTS:swordxx_objects>)
    SET_TARGET_PROPERTIES(swordxx_dynamic PROPERTIES OUTPUT_NAME "swordxx")
    SET(SWORDXX_PREFERRED_LIBRARY swordxx_dynamic)
    MESSAGE(STATUS "Building shared library: yes")

    IF(DEFINED SWORDXX_SOVERSION_OVERRIDE)
      SET(LIBSWORDXX_SOVERSION "${SWORDXX_SOVERSION_OVERRIDE}")
    ELSE()
      SET(LIBSWORDXX_SOVERSION "${SWORDXX_VERSION}")
    ENDIF()
    SET_TARGET_PROPERTIES(swordxx_dynamic PROPERTIES SOVERSION "${LIBSWORDXX_SOVERSION}")
    MESSAGE(STATUS "Building shared library: yes (using SOVERSION \"${LIBSWORDXX_SOVERSION}\")")
ELSE()
    MESSAGE(STATUS "Building shared library: no")
ENDIF()


##############################################################################################
# Setting libraries and includes
#

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
SET(SWORDXX_LINK_LIBRARIES ${SWORDXX_LINK_LIBRARIES} ${ZLIB_LIBRARY})
INCLUDE_DIRECTORIES(${BZIP2_INCLUDE_DIR})
SET(SWORDXX_LINK_LIBRARIES ${SWORDXX_LINK_LIBRARIES} ${BZIP2_LIBRARY})
INCLUDE_DIRECTORIES(${XZ_INCLUDE_DIR})
SET(SWORDXX_LINK_LIBRARIES ${SWORDXX_LINK_LIBRARIES} ${XZ_LIBRARY})

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
        ADD_DEFINITIONS(-DCURLSFTPAVAILABLE)
        MESSAGE(STATUS "cURL SFTP Support: Yes")
    ELSE()
        MESSAGE(STATUS "cURL SFTP Support: No")
    ENDIF()
ENDIF()
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})
SET(SWORDXX_LINK_LIBRARIES ${SWORDXX_LINK_LIBRARIES} ${CURL_LIBRARY})

IF(SWORDXX_WITH_CLUCENE)
    INCLUDE_DIRECTORIES(${CLUCENE_INCLUDE_DIR})
    SET(SWORDXX_LINK_LIBRARIES ${SWORDXX_LINK_LIBRARIES} ${CLUCENE_LIBRARY})
    LINK_DIRECTORIES(${CLUCENE_LIBRARY_DIR})
    ADD_DEFINITIONS(-DUSELUCENE)
    IF(CLUCENE2)
        ADD_DEFINITIONS(-DCLUCENE2)
    ENDIF()
ENDIF()
IF(SWORDXX_WITH_ICU)
    INCLUDE_DIRECTORIES(${ICU_INCLUDE_DIRS})
    ADD_DEFINITIONS(${ICU_DEFINITIONS})
    SET(SWORDXX_LINK_LIBRARIES ${SWORDXX_LINK_LIBRARIES} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES})
ENDIF()

IF(SWORDXX_INSTALL_SHARED)
    TARGET_LINK_LIBRARIES(swordxx_dynamic ${SWORDXX_LINK_LIBRARIES})
ENDIF()

IF(SWORDXX_INSTALL_STATIC)
    TARGET_LINK_LIBRARIES(swordxx_static ${SWORDXX_LINK_LIBRARIES})
ENDIF()


##############################################################################################
# Installing the library, headers, utilies, etc
#

IF(SWORDXX_INSTALL_SHARED)
    INSTALL(TARGETS swordxx_dynamic
        RUNTIME DESTINATION "${SWORDXX_BIN_INSTALL_PREFIX}"
        LIBRARY DESTINATION "${SWORDXX_LIB_INSTALL_PREFIX}"
        ARCHIVE DESTINATION "${SWORDXX_LIB_INSTALL_PREFIX}")
ENDIF()
IF(SWORDXX_INSTALL_STATIC)
    INSTALL(TARGETS swordxx_static
        RUNTIME DESTINATION "${SWORDXX_BIN_INSTALL_PREFIX}"
        LIBRARY DESTINATION "${SWORDXX_LIB_INSTALL_PREFIX}"
        ARCHIVE DESTINATION "${SWORDXX_LIB_INSTALL_PREFIX}")
ENDIF()
# Install the locales
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/locales.d
    DESTINATION "${SWORDXX_SHARE_INSTALL_PREFIX}/swordxx")

# Install the headers
INSTALL(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/src/"
        DESTINATION "${SWORDXX_INCLUDE_INSTALL_PREFIX}"
        FILES_MATCHING PATTERN "*.h")

# Generate and install the sysconf file:
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/swordxx.conf.in"
               "${CMAKE_CURRENT_BINARY_DIR}/swordxx.conf" @ONLY)
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/swordxx.conf"
    DESTINATION "${SWORDXX_SYSCONF_INSTALL_PREFIX}")
# Create the mods.d
INSTALL(DIRECTORY DESTINATION "${SWORDXX_SHARE_INSTALL_PREFIX}/swordxx/mods.d")

MESSAGE(STATUS "Destination: ${CMAKE_INSTALL_PREFIX}")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/swordxx.pc.in ${CMAKE_CURRENT_BINARY_DIR}/swordxx.pc @ONLY)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/swordxx/version.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/include/swordxx/version.h @ONLY)
ADD_DEFINITIONS(
    "-DSWORDXX_VERSION_H_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/include/swordxx/version.h\"")
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/src/swordxx/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/include/swordxx/config.h @ONLY)

INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/swordxx.pc
    DESTINATION "${SWORDXX_LIB_INSTALL_PREFIX}/pkgconfig")


##############################################################################################
# Subdirectories:
#

SET(SWORD_BUILD_INCLUDES
    "${CMAKE_CURRENT_BINARY_DIR}/include/"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/")

MACRO(SwordxxHandleConditionalBuild cond defaultValue)
    IF(NOT (DEFINED "SWORDXX_BUILD_${cond}"))
        SET("SWORDXX_BUILD_${cond}" "${defaultValue}")
    ENDIF()
    IF(SWORDXX_BUILD_${cond})
        SET("SWORDXX_${cond}_EXCLUDE" "")
    ELSE()
        SET("SWORDXX_${cond}_EXCLUDE" EXCLUDE_FROM_ALL)
    ENDIF()
ENDMACRO()

ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/examples")
ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/tests")
ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/utilities")
