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
	OPTION(PREFIX "${SWORD_INSTALL_DIR}" CACHE STRING "Directory into which to install architecture-dependent files. Defaults to ${SWORD_INSTALL_DIR}")
ENDIF(CMAKE_INSTALL_PREFIX)

SET(LIBDIR "${PREFIX}/lib" CACHE STRING "Object code library install directory. Defaults to ${CMAKE_INSTALL_PREFIX}/lib")

SET(INCLUDEDIR "${PREFIX}/include" CACHE STRING "C Header files install directory. Defaults to ${CMAKE_INSTALL_PREFIX}/include.")
