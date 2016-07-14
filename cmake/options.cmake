SET(LIB_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    CACHE STRING "Object code library install directory. Defaults to \"\${CMAKE_INSTALL_PREFIX}/lib\"")

SET(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include"
    CACHE STRING "C Header files install directory. Defaults to \"\${CMAKE_INSTALL_PREFIX}/include\"")

SET(BINDIR "${CMAKE_INSTALL_PREFIX}/bin"
    CACHE STRING "Directory to install binary executable files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/bin\"")

SET(SYSCONF_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/etc"
    CACHE STRING "Directory to install global config files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/etc\"")

SET(SHARE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}/share"
    CACHE STRING "Directory to install global data files. Defaults to \"\${CMAKE_INSTALL_PREFIX}/share\"")
