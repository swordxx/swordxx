##################################################################################
# These are some examples.  If you want examples to be built, you need to define
# SWORDXX_BUILD_EXAMPLES="YES".  These will not be installed, so don't worry about
# that.
#

SET(example_PROGRAMS
    listbiblebooknames
    parallelbibles
    simpleverselookup
)

FOREACH(DEMO ${example_PROGRAMS})
    ADD_EXECUTABLE("${DEMO}" "${SWORDXX_EXAMPLES_EXCLUDE}" "${DEMO}.cpp")
    TARGET_LINK_LIBRARIES("${DEMO}" Swordxx::swordxx)
ENDFOREACH(DEMO ${example_PROGRAMS})
