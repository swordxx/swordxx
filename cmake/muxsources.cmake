################################################################################################
#
# This file will mess with the sources lists, as well as set variables, such as the WITH_* to be
# used later in the configure process.
#
# Written by Greg Hellings
SET(sword_SOURCES ${sword_base_SOURCES})
# Check for if we've found ZLIB
# This one is a bit more unique, since we still allow compilation without
# a ZLIB at all, and allowing a user to disable it does not bring about use
# of some internal fall-back but just leaves the ability to read ZLIB files
# out of the library altogether
MESSAGE(STATUS "\n-- CONFIGURING SOURCE LIST")
IF(SWORD_NO_ZLIB STREQUAL "Yes")
	MESSAGE(STATUS "ZLib: excluded by use option")
	ADD_DEFINITIONS(-DEXCLUDEZLIB)
	SET(WITH_ZLIB 0)
ELSE(SWORD_NO_ZLIB STREQUAL "Yes")
	SET(sword_SOURCES ${sword_SOURCES} ${sword_zlib_used_SOURCES})
	IF(NOT ZLIB_FOUND OR SWORD_USE_INTERNAL_ZLIB STREQUAL "Yes")
		MESSAGE(STATUS "ZLib: internal")
		SET(sword_SOURCES ${sword_SOURCES} ${sword_zlib_nofound_SOURCES})
		SET(WITH_INTERNAL_ZLIB 1)
	ELSE(NOT ZLIB_FOUND OR SWORD_USE_INTERNAL_ZLIB STREQUAL "Yes")
		MESSAGE(STATUS "ZLib: system ${ZLIB_LIBRARY}")
		SET(WITH_ZLIB 1)
	ENDIF(NOT ZLIB_FOUND OR SWORD_USE_INTERNAL_ZLIB STREQUAL "Yes")
ENDIF(SWORD_NO_ZLIB STREQUAL "Yes")

# Check for if we've found cURL
IF(CURL_FOUND AND NOT SWORD_NO_CURL STREQUAL "Yes")
	MESSAGE(STATUS "cURL: system ${CURL_LIBRARY} and ${CURL_INCLUDE_DIRS}")
	ADD_DEFINITIONS(-DCURLAVAILABLE)
	SET(sword_SOURCES ${sword_SOURCES} ${sword_curl_found_SOURCES})
	SET(WITH_CURL 1)
ELSE(CURL_FOUND AND NOT SWORD_NO_CURL STREQUAL "Yes")
	MESSAGE(STATUS "cURL: no")
	SET(sword_SOURCES ${sword_SOURCES} ${sword_curl_nofound_SOURCES})
	SET(WITH_CURL 0)
ENDIF(CURL_FOUND AND NOT SWORD_NO_CURL STREQUAL "Yes")

# And for CLucene
IF(CLucene_FOUND AND NOT SWORD_NO_CLUCENE STREQUAL "Yes")
	MESSAGE(STATUS "CLucene: yes ${CLUCENE_LIBRARY} in ${CLUCENE_LIBRARY_DIR} and ${CLUCENE_INCLUDE_DIR}")
	SET(sword_SOURCES ${sword_SOURCES} ${sword_clucene_found_SOURCES})
	SET(WITH_CLUCENE 1)
ELSE(CLucene_FOUND AND NOT SWORD_NO_CLUCENE STREQUAL "Yes")
	MESSAGE(STATUS "CLucene: no")
	SET(sword_SOURCES ${sword_SOURCES} ${sword_clucene_nofound_SOURCES})
	SET(WITH_CLUCENE 0)
ENDIF(CLucene_FOUND AND NOT SWORD_NO_CLUCENE STREQUAL "Yes")

# Alert the user if PkgConfig is unavailalbe
IF(NOT PKG_CONFIG_FOUND)
	MESSAGE(STATUS "PkgConfig: no")
	SET(WITH_PKG_CONFIG 1)
ELSE(NOT PKG_CONFIG_FOUND)
	MESSAGE(STATUS "PkgConfig: yes")
	SET(WITH_PKG_CONFIG 0)
ENDIF(NOT PKG_CONFIG_FOUND)

# ICU needs some lovin' too
IF(ICU_FOUND AND NOT SWORD_NO_ICU STREQUAL "No")
	MESSAGE(STATUS "ICU: yes ${ICU_LIBRARY} and ${ICU_INCLUDE_DIRS}")
	ADD_DEFINITIONS(-D_ICU_)
	SET(sword_SOURCES ${sword_SOURCES} ${sword_icu_found_SOURCES})
	SET(WITH_ICU 1)
ELSE(ICU_FOUND AND NOT SWORD_NO_ICU STREQUAL "No")
	MESSAGE(STATUS "ICU: no")
	SET(WITH_ICU 0)
ENDIF(ICU_FOUND AND NOT SWORD_NO_ICU STREQUAL "No")

# Internal or external regex.h
IF(REGEX_FOUND)
	MESSAGE(STATUS "Regex.h: system ${REGEX_INCLUDE_DIR}")
	INCLUDE_DIRECTORIES(${REGEX_INCLUDE_DIR})
	SET(sword_SOURCES ${sword_SOURCES} ${sword_external_regex_SOURCES})
	SET(WITH_REGEX 1)
ELSE(REGEX_FOUND)
	MESSAGE(STATUS "Regex.h: internal")
	INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/internal/regex)
	SET(sword_SOURCES ${sword_SOURCES} ${sword_internal_regex_SOURCES})
	SET(WITH_REGEX 0)
ENDIF(REGEX_FOUND)
