######################################################################
# Basic utility programs and their joy.
#
# The general assumption is that each of these utilities is built from
# a single source file, which shares its name with the utility itself (appended with .cpp),
# and then linked against the SWORD++ library.
#
# This list will be built and installed, if so chosen
#
# These are in alphbetical order now - please keep them that way
# if you edit this list in the future.
#

FIND_PACKAGE(ICU REQUIRED COMPONENTS uc)

INCLUDE_DIRECTORIES(${SWORD_BUILD_INCLUDES})

SwordxxHandleConditionalBuild("utilities" UTILS TRUE TRUE)

SET(install_UTILITIES
    addld
    emptyvss
    imp2gbs
    imp2ld
    imp2vs
    installmgr
    mod2imp
    mod2osis
    mod2vpl
    tei2mod
    vpl2mod
    vs2osisref
    vs2osisreftxt
    xml2gbs
    osis2mod
)
SET(imp2gbs_EXTRA_LINK_LIBRARIES "ICU::uc")

SET(install_UTILITIES ${install_UTILITIES} mod2zmod)

######################################################################
# These utilities will be built, but they will not be installed
#
# Again, I have gone to great lengths of travail to make this list
# alphabetical.  If you add utilities to this list, please do so in a
# way that maintains this.
SET(noinstall_UTILITIES
    addgb
    addvs
    gbfidx
    genbookutil
    modwrite
    step2vpl
    stepdump
    treeidxutil
)

#####################################################################
# This will loop over both of the above utility lists and add build targets
# to the system for each one of them.
#

FOREACH(UTIL ${install_UTILITIES} ${noinstall_UTILITIES})
    ADD_EXECUTABLE("${UTIL}" "${SWORDXX_UTILS_EXCLUDE}" "${UTIL}.cpp")
    TARGET_LINK_LIBRARIES("${UTIL}"
        PRIVATE
            Swordxx::swordxx
            ${${UTIL}_EXTRA_LINK_LIBRARIES})
ENDFOREACH(UTIL ${install_UTILITIES})

SET_TARGET_PROPERTIES(imp2gbs PROPERTIES INCLUDE_DIRECTORIES
    "${SWORD_BUILD_INCLUDES};${ICU_INCLUDE_DIRS}")
SET_TARGET_PROPERTIES(imp2gbs PROPERTIES LINK_LIBRARIES
    "${ICU_LIBRARIES};swordxx")

####################################################################
# Just to be difficult, we have a single C file that is a utility
# and, therefore, needs its own treatment.
#

ADD_EXECUTABLE("lexdump" "${SWORDXX_UTILS_EXCLUDE}" "lexdump.cpp")
TARGET_LINK_LIBRARIES(lexdump Swordxx::swordxx)

####################################################################
# Install the utilities
#
# We do this here, rather than up in the main install file, because
# a) it fits more logically and b) the scope of ${install_UTILITIES}
# is limited to CMake files in this directory and below, so rather than
# replicate the above list in another place, we'll just handle our
# own install, since this whole directory is optional, anyway.
#

IF(SWORDXX_BUILD_UTILS)
    FOREACH(UTIL ${install_UTILITIES})
        INSTALL(TARGETS ${UTIL}
            RUNTIME DESTINATION "${SWORDXX_BIN_INSTALL_PREFIX}"
            COMPONENT utilities
        )
    ENDFOREACH(UTIL ${install_UTILITIES})
ENDIF()
