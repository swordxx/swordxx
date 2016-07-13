################################################################################################
#
# This file will mess with the sources lists, as well as set variables, such as the WITH_* to be
# used later in the configure process.
#
# Written by Greg Hellings

MESSAGE(STATUS "ZLib: system ${ZLIB_LIBRARY}")
MESSAGE(STATUS "bzip2: system ${BZIP2_LIBRARY}")
MESSAGE(STATUS "xz: system ${XZ_LIBRARY}")
MESSAGE(STATUS "cURL: system ${CURL_LIBRARY} and ${CURL_INCLUDE_DIRS}")

# And for CLucene
IF(CLucene_FOUND AND NOT SWORD_NO_CLUCENE STREQUAL "Yes")
    MESSAGE(STATUS "CLucene: yes ${CLUCENE_LIBRARY} in ${CLUCENE_LIBRARY_DIR} and ${CLUCENE_INCLUDE_DIR}")
    SET(WITH_CLUCENE 1)
ELSE(CLucene_FOUND AND NOT SWORD_NO_CLUCENE STREQUAL "Yes")
    MESSAGE(STATUS "CLucene: no")
    SET(WITH_CLUCENE 0)
ENDIF(CLucene_FOUND AND NOT SWORD_NO_CLUCENE STREQUAL "Yes")

# ICU needs some lovin' too
IF(ICU_FOUND AND NOT SWORD_NO_ICU STREQUAL "No")
    MESSAGE(STATUS "ICU: yes ${ICU_LIBRARY} and ${ICU_INCLUDE_DIRS}")
    ADD_DEFINITIONS(-D_ICU_)
    SET(WITH_ICU 1)
ELSE(ICU_FOUND AND NOT SWORD_NO_ICU STREQUAL "No")
    MESSAGE(STATUS "ICU: no")
    SET(WITH_ICU 0)
ENDIF(ICU_FOUND AND NOT SWORD_NO_ICU STREQUAL "No")
