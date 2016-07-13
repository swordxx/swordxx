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
ENDIF(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})


###########################################################################################
# Here are some basic CMake variables we need to setup in order for things to work properly
#
# Our include directory, for our own internally created "FIND_PACKAGE" calls, like CLucene
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# User options
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")
# Our local includes
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}/include") # For generated version.h


###########################################################################################
# This will look for various libraries that libsword builds against.  There is no REQUIRED
# attribute added here, since all of the libraries here are not, in actuality, required.
#

# Find our packages
FIND_PACKAGE(BZIP2 REQUIRED QUIET)
FIND_PACKAGE(XZ REQUIRED QUIET)
FIND_PACKAGE(ICU QUIET)
FIND_PACKAGE(CURL REQUIRED QUIET)
FIND_PACKAGE(CLucene QUIET)
FIND_PACKAGE(ZLIB REQUIRED QUIET)


################################################################################
# The buildflags depend on the CMAKE_BUILD_TYPE supplied at the command line.

ADD_COMPILE_OPTIONS(
  "-Wall" "-Wextra" "-O2"
  "-DSWORDXX_VERSION_H_FILE=\"${CMAKE_CURRENT_BINARY_DIR}/include/version.h\""
  "$<$<COMPILE_LANGUAGE:C>:-std=c99>"
  "$<$<COMPILE_LANGUAGE:CXX>:-std=c++14>"
  "$<$<CONFIG:Release>:-DNDEBUG>"
  "$<$<NOT:$<CONFIG:Release>>:-ggdb>"
  "$<$<NOT:$<CONFIG:Release>>:-fno-omit-frame-pointer>"
)

###########################################################################################
# Based on user input and the results of the above tests, we may need to mux with the source
# files to use an internal version of ZLib, cURL-like stuff, or CLucene replacements.  These
# lines below will modify the source files directories so that the required files are only
# included if the option is needed.
#
# Modify the source variables and set necessary definitions, this is a rather long segment,
# so I have sorted it out into its own file
INCLUDE(${CMAKE_CURRENT_SOURCE_DIR}/cmake/muxsources.cmake)

################################################################################################
# This actually creates the build target that is the libsword building target to be generated.
# Most of the work for configuration is done above, already.
#
# I want to do this manually, there might be reason in the future

# Sources:
FILE(GLOB_RECURSE sword_C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
FILE(GLOB_RECURSE sword_CXX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
SET(sword_SOURCES ${sword_C_SOURCES} ${sword_CXX_SOURCES})

# Headers:
FILE(GLOB_RECURSE SWORD_INSTALL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
LIST(APPEND SWORD_INSTALL_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/include/version.h")


IF(LIBSWORD_LIBRARY_TYPE MATCHES ".*Shared.*" OR NOT LIBSWORD_LIBRARY_TYPE)
    ADD_LIBRARY(sword SHARED ${sword_SOURCES})
    MESSAGE(STATUS "Building Shared library.")
    SET(BUILDING_SHARED 1)
    SET(SWORD_LINK_NAME sword)
ENDIF(LIBSWORD_LIBRARY_TYPE MATCHES ".*Shared.*" OR NOT LIBSWORD_LIBRARY_TYPE)

IF(LIBSWORD_LIBRARY_TYPE MATCHES ".*Static.*" OR NOT LIBSWORD_LIBRARY_TYPE)
    ADD_LIBRARY(sword_static STATIC ${sword_SOURCES})
    SET_TARGET_PROPERTIES(sword_static PROPERTIES COMPILE_FLAGS "-fPIC")
    SET(BUILDING_STATIC 1)
    # Defaults to linking against the shared if it is also being built
    IF(NOT BUILDING_SHARED)
        SET(SWORD_LINK_NAME sword_static)
    ENDIF(NOT BUILDING_SHARED)
    SET_TARGET_PROPERTIES(sword_static PROPERTIES OUTPUT_NAME "sword")
    MESSAGE(STATUS "Building Static library.")
ENDIF(LIBSWORD_LIBRARY_TYPE MATCHES ".*Static.*" OR NOT LIBSWORD_LIBRARY_TYPE)

################################################################################################
# Some random user settings
#
ADD_DEFINITIONS(-DGLOBCONFPATH="${SYSCONF_INSTALL_DIR}/sword.conf")
SET(CONFFILE "[Install]
DataPath=${SHARE_INSTALL_PREFIX}/sword")
FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/sword.conf" "${CONFFILE}")


###############################################################################################
# This allows the user to set a SONAME for the library.  This allows packagers and those who
# care about that sort of thing to be happy and have all their SONAMES set properly.
#

IF(BUILDING_SHARED)
  IF(LIBSWORDXX_SOVERSION_OVERRIDE)
    SET(LIBSWORDXX_SOVERSION "${LIBSWORDXX_SOVERSION_OVERRIDE}")
  ELSE()
    SET(LIBSWORDXX_SOVERSION "${SWORDXX_VERSION}")
  ENDIF()
  SET_TARGET_PROPERTIES(sword PROPERTIES SOVERSION "${LIBSWORDXX_SOVERSION}")
  MESSAGE(STATUS "Using SOVERSION \"${LIBSWORDXX_SOVERSION}\"")
ENDIF(BUILDING_SHARED)

##############################################################################################
# Setting libraries and includes
#

INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${ZLIB_LIBRARY})
INCLUDE_DIRECTORIES(${BZIP2_INCLUDE_DIR})
SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${BZIP2_LIBRARY})
INCLUDE_DIRECTORIES(${XZ_INCLUDE_DIR})
SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${XZ_LIBRARY})

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
    ELSE(CURL_CONFIG_OUTPUT STREQUAL "1")
        MESSAGE(STATUS "cURL SFTP Support: No")
    ENDIF(CURL_CONFIG_OUTPUT STREQUAL "1")
ENDIF(CURL_CONFIG)
INCLUDE_DIRECTORIES(${CURL_INCLUDE_DIRS})
SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${CURL_LIBRARY})

IF(WITH_CLUCENE)
    INCLUDE_DIRECTORIES(${CLUCENE_INCLUDE_DIR})
    SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${CLUCENE_LIBRARY})
    LINK_DIRECTORIES(${CLUCENE_LIBRARY_DIR})
    ADD_DEFINITIONS(-DUSELUCENE)
    IF(CLUCENE2)
        ADD_DEFINITIONS(-DCLUCENE2)
    ENDIF(CLUCENE2)
ENDIF(WITH_CLUCENE)
IF(WITH_ICU)
    INCLUDE_DIRECTORIES(${ICU_INCLUDE_DIRS})
    ADD_DEFINITIONS(${ICU_DEFINITIONS})
    SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES})
ENDIF(WITH_ICU)

IF(BUILDING_SHARED)
    TARGET_LINK_LIBRARIES(sword ${SWORD_LINK_LIBRARIES})
ENDIF(BUILDING_SHARED)

IF(BUILDING_STATIC)
    TARGET_LINK_LIBRARIES(sword_static ${SWORD_LINK_LIBRARIES})
ENDIF(BUILDING_STATIC)

MESSAGE(STATUS "Setting link libraries to ${SWORD_LINK_LIBRARIES}")


##############################################################################################
# Installing the library, headers, utilies, etc
#

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/install.cmake")


##############################################################################################
# Utilities are hawt
#

IF(NOT (DEFINED SWORD_BUILD_UTILS))
  SET(SWORD_BUILD_UTILS TRUE)
ENDIF()
IF(NOT SWORD_BUILD_UTILS)
    ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/utilities")
ENDIF()

##############################################################################################
# Demos are also hawt
#

IF(SWORD_BUILD_EXAMPLES)
    ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/examples/cmdline")
    ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/examples/tasks")
ENDIF()

##############################################################################################
# Tests, however, are not

IF(SWORD_BUILD_TESTS)
    ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/tests")
ENDIF()
