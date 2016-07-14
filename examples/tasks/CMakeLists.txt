##################################################################################
# These are some examples.  If you want examples to be built, you need to define
# SWORDXX_BUILD_EXAMPLES="YES".  These will not be installed, so don't worry about
# that.
#

INCLUDE_DIRECTORIES("../../src/")

SwordxxHandleConditionalBuild(EXAMPLES FALSE)

SET(example_PROGRAMS
    parallelbibles
)

FOREACH(DEMO ${example_PROGRAMS})
    ADD_EXECUTABLE("${DEMO}" "${SWORDXX_EXAMPLES_EXCLUDE}" "${DEMO}.cpp")
    TARGET_LINK_LIBRARIES("${DEMO}" swordxx)
ENDFOREACH(DEMO ${example_PROGRAMS})

