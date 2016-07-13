# This file started on 18 January 2010 by Gregory Hellings
# It is ceded to The SWORD Library developers and CrossWire under the terms
# of their own GPLv2 license and all copyright is transferred to them for
# all posterity and eternity, wherever such transfer is possible.  Where it is
# not, then this file is released under the GPLv2 by myself.
#
#

FILE(GLOB_RECURSE sword_C_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
FILE(GLOB_RECURSE sword_CXX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
SET(sword_SOURCES ${sword_C_SOURCES} ${sword_CXX_SOURCES})

# Headers
FILE(GLOB_RECURSE SWORD_INSTALL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
LIST(APPEND SWORD_INSTALL_HEADERS
    "${CMAKE_CURRENT_BINARY_DIR}/include/version.h")
