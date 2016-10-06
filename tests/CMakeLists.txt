#########################################################################################
# A test suite - that is awesome.
#
# I should probably learn the proper usage of such things as CTest for this, but for the
# time being, I'll see what I can do.
#
#
# I need some help deciphering tests/cppunit/Makefile.am.  I'm not quite sure what it is
# trying to do in there.
# Also, the contents of the tests/testsuite directory are slightly beyond my knowledge
# level.  It looks like I can do a very simple interface to this all, but I need to know
# more about how these tests work.
#

INCLUDE_DIRECTORIES(${SWORD_BUILD_INCLUDES})

SwordxxHandleConditionalBuild("tests" TESTS FALSE TRUE)

SET(test_PROGRAMS
    bibliotest
    ciphertest
    complzss
    compnone
    configtest
    filtertest
    introtest
    indextest
    keycast
    keytest
    lextest
    listtest
    localetest
    mgrtest
    modtest
    osistest
    ldtest
    parsekey
    rawldidxtest
    romantest
    striptest
    swaptest
    testblocks
    utf8norm
    versekeytest
    vtreekeytest
    versemgrtest
    webiftest
    xmltest
)

IF(SWORDXX_WITH_ICU)
    SET(test_PROGRAMS
        ${test_PROGRAMS}
        icutest
#        tlitmgrtest
        translittest
    )
ENDIF(SWORDXX_WITH_ICU)

SET(test_PROGRAMS
    ${test_PROGRAMS}
    compzip
)

FOREACH(TEST ${test_PROGRAMS})
    ADD_EXECUTABLE(${TEST} "${SWORDXX_TESTS_EXCLUDE}" ${TEST}.cpp)
    TARGET_LINK_LIBRARIES(${TEST} swordxx)
ENDFOREACH(TEST ${test_PROGRAMS})

########################################################################################
# The following tests require extra libraries to run
#
FOREACH(ICUTEST icutest translittest)
    TARGET_LINK_LIBRARIES(${ICUTEST} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES})
ENDFOREACH(ICUTEST icutest translittest)

# Excluded until I know we have the tests working
ADD_SUBDIRECTORY(testsuite)
