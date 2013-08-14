# A list of the options that the library supports
SET(SWORD_PYTHON_INSTALL_DIR "" CACHE STRING "Directory where the Python bindings will be installed. Defaults to default Python path.")

# Installation options
IF(APPLE)
	SET(SWORD_INSTALL_DIR "/opt/local")
ELSEIF(MSVC)
	SET(SWORD_INSTALL_DIR "C:\\Program Files (x86)\\libsword\\")
ELSE(APPLE)
	SET(SWORD_INSTALL_DIR "/usr/local")
ENDIF(APPLE)

IF(CMAKE_INSTALL_PREFIX)
	SET(PREFIX "${CMAKE_INSTALL_PREFIX}" CACHE STRING "Directory into which to install architecture-dependent files. Defaults to ${SWORD_INSTALL_DIR}.")
ELSE(CMAKE_INSTALL_PREFIX)
	SET(PREFIX "${SWORD_INSTALL_DIR}" CACHE STRING "Directory into which to install architecture-dependent files. Defaults to ${SWORD_INSTALL_DIR}")
ENDIF(CMAKE_INSTALL_PREFIX)

SET(LIBDIR "${PREFIX}/lib" CACHE STRING "Object code library install directory. Defaults to ${SWORD_INSTALL_DIR}/lib")

SET(INCLUDEDIR "${PREFIX}/include" CACHE STRING "C Header files install directory. Defaults to ${SWORD_INSTALL_DIR}/include.")

SET(BINDIR "${PREFIX}/bin" CACHE STRING "Directory to install binary executable files. Defaults to ${SWORD_INSTALL_DIR}/bin.")

# Post-processing of variables
MACRO(PROCESS_VERSION LEVEL VALUE)
    SET(SWORD_VERSION_${LEVEL} ${VALUE})
    IF(${VALUE} LESS 10)
        SET(${LEVEL} "00${VALUE}")
    ELSEIF(${VALUE} LESS 100)
        SET(${LEVEL} "0${VALUE}")
    ELSE()
        SET(${LEVEL} "${VALUE}")
    ENDIF()
ENDMACRO()

STRING(REGEX MATCHALL "^([0-9]+)\\.([0-9]+)\\.([0-9]+)\\.?([0-9]+)?$"
    SWORD_VERSION_PARTS "${SWORD_VERSION}")
# We don't always have a nano version
IF("${CMAKE_MATCH_4}" STREQUAL "")
    SET(CMAKE_MATCH_4 "0")
ENDIF("${CMAKE_MATCH_4}" STREQUAL "")

SET(SWORD_VERSION_MAJOR ${CMAKE_MATCH_1}) # No post-processing on this, so it's not octal
PROCESS_VERSION("MINOR" ${CMAKE_MATCH_2})
PROCESS_VERSION("MICRO" ${CMAKE_MATCH_3})
PROCESS_VERSION("NANO"  ${CMAKE_MATCH_4})

SET(SWORD_VERSION_STR "${SWORD_VERSION}")
SET(SWORD_VERSION_NUM "${SWORD_VERSION_MAJOR}${MINOR}${MICRO}${NANO}")

MESSAGE(STATUS "SWORD Version ${SWORD_VERSION_NUM}")
