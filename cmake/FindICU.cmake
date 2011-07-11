# Finds the International Components for Unicode (ICU) Library
#
#  ICU_FOUND          - True if ICU found.
#  ICU_I18N_FOUND     - True if ICU's internationalization library found.
#  ICU_INCLUDE_DIRS   - Directory to include to get ICU headers
#                       Note: always include ICU headers as, e.g., 
#                       unicode/utypes.h
#  ICU_LIBRARIES      - Libraries to link against for the common ICU
#  ICU_I18N_LIBRARIES - Libraries to link against for ICU internationaliation
#                       (note: in addition to ICU_LIBRARIES)

# Our first goal is to find the icu-config script, if possible
find_program(
  ICU_CONFIG_BIN
  icu-config
  HINTS "${ICU_CONFIG_BIN_PATH}"
)
find_program(
     ICU_GENRB
     genrb
)

if(ICU_CONFIG_BIN)
  MESSAGE(STATUS "[ICU] icu-config: Yes ${ICU_CONFIG_BIN}")
  MESSAGE(STATUS "[ICU] genrb: Yes ${ICU_GENRB}")

  # Get include directories
  execute_process(COMMAND "${ICU_CONFIG_BIN}" ${ICU_CONFIG_OPTS} "--cppflags"
    OUTPUT_VARIABLE ICU_INCLUDE_DIR
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  # Sanitize output
  string(REPLACE "-I" "" ICU_INCLUDE_DIR ${ICU_INCLUDE_DIR})
  string(REGEX REPLACE "^[ 	]+" "" ICU_INCLUDE_DIR ${ICU_INCLUDE_DIR})

     # Try to get the Libraries we need
  execute_process(COMMAND "${ICU_CONFIG_BIN}" ${ICU_CONFIG_OPTS} "--ldflags"
    OUTPUT_VARIABLE ICU_LIBRARY_RAW
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  
  execute_process(COMMAND "${ICU_CONFIG_BIN}" ${ICU_CONFIG_OPTS} "--ldflags-icuio"
     OUTPUT_VARIABLE ICU_LIBRARY_IO_RAW
     OUTPUT_STRIP_TRAILING_WHITESPACE
  )

     string(REGEX REPLACE "^[ 	]+" "" ICU_LIBRARY ${ICU_LIBRARY_RAW})
     string(REGEX REPLACE "^[ 	]+" "" ICU_LIBRARY_IO ${ICU_LIBRARY_IO_RAW})
     # Combine them
     set(ICU_LIBRARY "${ICU_LIBRARY} ${ICU_LIBRARY_IO_RAW}")
     
     # Get the version
     execute_process(COMMAND "${ICU_CONFIG_BIN}" ${ICU_CONFIG_OPTS} "--version"
	  OUTPUT_VARIABLE ICU_VERSION
	  OUTPUT_STRIP_TRAILING_WHITESPACE
     )
else(ICU_CONFIG_BIN)

  # Look for the header file.
  find_path(
    ICU_INCLUDE_DIR 
    NAMES unicode/utypes.h
    DOC "Include directory for the ICU library")
  mark_as_advanced(ICU_INCLUDE_DIR)

  # Look for the library.
  find_library(
    ICU_LIBRARY
    NAMES icuuc cygicuuc cygicuuc32
    DOC "Libraries to link against for the common parts of ICU")
  mark_as_advanced(ICU_LIBRARY)
endif(ICU_CONFIG_BIN)

# Copy the results to the output variables.
if(ICU_INCLUDE_DIR AND ICU_LIBRARY)
  set(ICU_FOUND 1)
  set(ICU_LIBRARIES ${ICU_LIBRARY})
  set(ICU_INCLUDE_DIRS ${ICU_INCLUDE_DIR})

  # Look for the ICU internationalization libraries
  find_library(
    ICU_I18N_LIBRARY
    NAMES icuin icui18n cygicuin cygicuin32
    DOC "Libraries to link against for ICU internationalization")
  mark_as_advanced(ICU_I18N_LIBRARY)
  if (ICU_I18N_LIBRARY)
    set(ICU_I18N_FOUND 1)
    set(ICU_I18N_LIBRARIES ${ICU_I18N_LIBRARY})
  else (ICU_I18N_LIBRARY)
    set(ICU_I18N_FOUND 0)
    set(ICU_I18N_LIBRARIES)
  endif (ICU_I18N_LIBRARY)
else(ICU_INCLUDE_DIR AND ICU_LIBRARY)
  set(ICU_FOUND 0)
  set(ICU_I18N_FOUND 0)
  set(ICU_LIBRARIES)
  set(ICU_I18N_LIBRARIES)
  set(ICU_INCLUDE_DIRS)
  MESSAGE(STATUS "ICU: No")
endif(ICU_INCLUDE_DIR AND ICU_LIBRARY)
