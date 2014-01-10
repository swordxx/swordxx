# TODO: write FindICU (icu-config only for 2.2 and up) -- currently taken from another CMake system
#       limit pkg-config version to >= 0.14, demo, utilities, doc, tests
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
#
#
# A CMake port of the SWORD build system... we hope
PROJECT(libsword CXX C)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.0)
SET(SWORD_VERSION 1.7.2)

# Make sure it's an out-of-stream build
IF(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})
	MESSAGE(FATAL_ERROR "Please invoke CMake from a different directory than the source.")
ENDIF(${CMAKE_CURRENT_BINARY_DIR} STREQUAL ${CMAKE_CURRENT_SOURCE_DIR})

MESSAGE(STATUS "Configuring your system to build libsword.")

###########################################################################################
# Here are some basic CMake variables we need to setup in order for things to work properly
#
# Our include directory, for our own internally created "FIND_PACKAGE" calls, like CLucene
SET(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
# User options
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/options.cmake")
# Source variables
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/sources.cmake")
# Our local includes
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}/include") # For swversion.h
INCLUDE_DIRECTORIES("${CMAKE_CURRENT_SOURCE_DIR}/include") # For everything else

###########################################################################################
# This will look for various libraries that libsword builds against.  There is no REQUIRED
# attribute added here, since all of the libraries here are not, in actuality, required.
#
MESSAGE(STATUS "\n-- SEARCHING FOR SYTEM PACKAGES")
# Find our packages
FIND_PACKAGE(ZLIB QUIET)
FIND_PACKAGE(ICU QUIET)
FIND_PACKAGE(CURL QUIET)
FIND_PACKAGE(CLucene QUIET)
FIND_PACKAGE(PkgConfig QUIET)
FIND_PACKAGE(Regex QUIET)

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
	# With MSVC, the .dll files also create .lib files of the same name
	# with the exported symbols.  Thus, we don't want the two to have the
	# same exact name.  In other systems, the .a and .so suffixes suffice
	# to keep them separate
	IF(NOT MSVC OR NOT LIBSWORD_LIBRARY_TYPE MATCHES ".*Shared.*")
		SET_TARGET_PROPERTIES(sword_static PROPERTIES 
			OUTPUT_NAME "sword")
		#MESSAGE(STATUS "Building Static library with name sword")
	ELSE(NOT MSVC OR NOT LIBSWORD_LIBRARY_TYPE MATCHES ".*Shared.*")
		#MESSAGE(STATUS "Building Static library with name
		#sword_static")
	ENDIF(NOT MSVC OR NOT LIBSWORD_LIBRARY_TYPE MATCHES ".*Shared.*")
	MESSAGE(STATUS "Building Static library.")
ENDIF(LIBSWORD_LIBRARY_TYPE MATCHES ".*Static.*" OR NOT LIBSWORD_LIBRARY_TYPE)

################################################################################################
# Some random user settings
#

IF(NOT SWORD_GLOBAL_CONF_DIR STREQUAL "")
	ADD_DEFINITIONS(-DGLOBCONFPATH="${SWORD_GLOBAL_CONF_DIR}/sword.conf")
ENDIF(NOT SWORD_GLOBAL_CONF_DIR STREQUAL "")


###############################################################################################
# This allows the user to set a SONAME for the library.  This allows packagers and those who
# care about that sort of thing to be happy and have all their SONAMES set properly.
#
IF(NOT LIBSWORD_SOVERSION AND BUILDING_SHARED)
        SET(SWORD_SOVERSION ${SWORD_VERSION})
ELSE(NOT LIBSWORD_SOVERSION AND BUILDING_SHARED)
        SET(SWORD_SOVERSION ${LIBSWORD_SOVERSION})
ENDIF(NOT LIBSWORD_SOVERSION AND BUILDING_SHARED)

IF(BUILDING_SHARED)
	SET_TARGET_PROPERTIES(sword
		PROPERTIES SOVERSION ${SWORD_SOVERSION})
	MESSAGE(STATUS "Setting SOVERSION to ${SWORD_SOVERSION}")
ENDIF(BUILDING_SHARED)

###############################################################################################
# The buildflags depend on the CMAKE_BUILD_TYPE supplied at the command line.
# For a full lists of different flags see http://cmake.org/Wiki/CMake_Useful_Variables
#
# Cmake has sane defaults for None aka '', DEBUG, RELEASE, RELWITHDEBINFO for known compilers.
#
# You can override these variables on the command-line or here.
# We provide our own defaults below
#
# Note the below two initialisations done for us by cmake:
#
#SET(CMAKE_C_FLAGS                  "${CMAKE_C_FLAGS}"   or "$ENV{CFLAGS}" or "" )
#SET(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS}" or "$ENV{CXXFLAGS}" or "" )

IF(SWORD_ENABLE_WARNINGS STREQUAL "Yes")
	SET(CMAKE_C_FLAGS "-Werror ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "-Werror ${CMAKE_CXX_FLAGS}")
ENDIF(SWORD_ENABLE_WARNINGS STREQUAL "Yes")

IF(SWORD_ENABLE_PROFILE STREQUAL "Yes")
	SET(CMAKE_C_FLAGS   "-pg ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "-pg ${CMAKE_CXX_FLAGS}")
ENDIF(SWORD_ENABLE_PROFILE STREQUAL "Yes")

IF(SWORD_ENABLE_PROFILEFN STREQUAL "Yes")
	SET(CMAKE_C_FLAGS   "-g -finstrument-functions ${CMAKE_C_FLAGS}")
	SET(CMAKE_CXX_FLAGS "-g -finstrument-functions ${CMAKE_CXX_FLAGS}")
	TARGET_LINK_LIBRARIES(libsword fnccheck)
ENDIF(SWORD_ENABLE_PROFILEFN STREQUAL "Yes")

SET(CMAKE_C_FLAGS_DEBUG            "-g3 -Wall -O0 ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS_RELEASE          "-O3 ${CMAKE_C_FLAGS}")
SET(CMAKE_C_FLAGS_RELWITHDEBINFO   "-O3 -g ${CMAKE_C_FLAGS}")
SET(CMAKE_CXX_FLAGS_DEBUG          "-g3 -Wall -O0 ${CMAKE_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS_RELEASE        "-O3 ${CMAKE_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g ${CMAKE_CXX_FLAGS}")

##############################################################################################
# Setting libraries and includes
#

IF(WITH_ZLIB)
	INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIR})
	SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${ZLIB_LIBRARY})
ENDIF(WITH_ZLIB)
IF(WITH_CURL)
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
ENDIF(WITH_CURL)
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
IF(REGEX_FOUND AND CROSS_COMPILE_MINGW32)
	SET(SWORD_LINK_LIBRARIES ${SWORD_LINK_LIBRARIES} ${REGEX_LIBRARY})
ENDIF(REGEX_FOUND AND CROSS_COMPILE_MINGW32)

IF(BUILDING_SHARED)
	TARGET_LINK_LIBRARIES(sword ${SWORD_LINK_LIBRARIES})
ENDIF(BUILDING_SHARED)

IF(BUILDING_STATIC)
	TARGET_LINK_LIBRARIES(sword_static ${SWORD_LINK_LIBRARIES})
ENDIF(BUILDING_STATIC)

MESSAGE(STATUS "Setting link libraries to ${SWORD_LINK_LIBRARIES}")

##############################################################################################
#########
### TODO: Not sure about these...
#########
##############################################################################################
ADD_DEFINITIONS(-D_FTPLIB_NO_COMPAT)

#############################################################################################
# Platform-specifc bits that I will eventually refactor out into their own files, once I am happy
# with the stuff that is here.
# 
IF(APPLE OR iPhone)
	ADD_DEFINITIONS(-Dunix)
ENDIF(APPLE OR iPhone)

##############################################################################################
# Our build test
#

ADD_EXECUTABLE(buildtest buildtest.cpp)
IF(BUILDING_STATIC)
	TARGET_LINK_LIBRARIES(buildtest sword_static)
ELSE(BUILDING_STATIC)
	TARGET_LINK_LIBRARIES(buildtest sword)
ENDIF(BUILDING_STATIC)

##############################################################################################
# Installing the library, headers, utilies, etc
# 

INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/cmake/install.cmake")

##############################################################################################
# Bindings are good, right?
#

IF(NOT SWORD_BINDINGS STREQUAL "" AND SWORD_BINDINGS)
	ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/bindings")
ENDIF(NOT SWORD_BINDINGS STREQUAL "" AND SWORD_BINDINGS)

##############################################################################################
# Utilities are hawt
# 

IF(NOT SWORD_BUILD_UTILS STREQUAL "No")
	ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/utilities")
ENDIF(NOT SWORD_BUILD_UTILS STREQUAL "No")

##############################################################################################
# Demos are also hawt
# 

IF(SWORD_BUILD_EXAMPLES STREQUAL "Yes")
	ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/examples/cmdline")
ENDIF(SWORD_BUILD_EXAMPLES STREQUAL "Yes")

##############################################################################################
# Tests, however, are not

IF(SWORD_BUILD_TESTS STREQUAL "Yes")
	ADD_SUBDIRECTORY("${CMAKE_CURRENT_SOURCE_DIR}/tests")
ENDIF(SWORD_BUILD_TESTS STREQUAL "Yes")
