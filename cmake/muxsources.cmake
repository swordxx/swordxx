################################################################################################
#
# This file will mess with the sources lists, as well as set variables, such as the WITH_* to be
# used later in the configure process.
#
# Written by Greg Hellings
SET(sword_SOURCES ${sword_base_SOURCES})

MESSAGE(STATUS "\n-- CONFIGURING SOURCE LIST")

SET(sword_SOURCES ${sword_SOURCES} ${sword_zlib_used_SOURCES})
MESSAGE(STATUS "ZLib: system ${ZLIB_LIBRARY}")

SET(sword_SOURCES ${sword_SOURCES} ${sword_bzip2_used_SOURCES})
MESSAGE(STATUS "bzip2: system ${BZIP2_LIBRARY}")

SET(sword_SOURCES ${sword_SOURCES} ${sword_xz_used_SOURCES})
MESSAGE(STATUS "xz: system ${XZ_LIBRARY}")

# Check for if we've found cURL
MESSAGE(STATUS "cURL: system ${CURL_LIBRARY} and ${CURL_INCLUDE_DIRS}")
SET(sword_SOURCES ${sword_SOURCES} ${sword_curl_found_SOURCES})

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
