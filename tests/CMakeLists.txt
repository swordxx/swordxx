FIND_PACKAGE(ICU REQUIRED COMPONENTS i18n uc)

INCLUDE_DIRECTORIES(${SWORD_BUILD_INCLUDES})

SwordxxHandleConditionalBuild("tests" TESTS FALSE TRUE)

SET(test_PROGRAMS
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
    utf8norm
    versekeytest
    vtreekeytest
    versemgrtest
    webiftest
)

SET(test_PROGRAMS
    ${test_PROGRAMS}
    icutest
#        tlitmgrtest
    translittest
)
SET(icutest_EXTRA_LINK_LIBRARIES "ICU::i18n" "ICU::uc")
SET(translittest_EXTRA_LINK_LIBRARIES "ICU::i18n" "ICU::uc")

SET(test_PROGRAMS
    ${test_PROGRAMS}
    compzip
)

FOREACH(TEST ${test_PROGRAMS})
    ADD_EXECUTABLE(${TEST} "${SWORDXX_TESTS_EXCLUDE}" ${TEST}.cpp)
    TARGET_LINK_LIBRARIES("${TEST}"
        PRIVATE
            Swordxx::swordxx
            ${${TEST}_EXTRA_LINK_LIBRARIES})
ENDFOREACH(TEST ${test_PROGRAMS})


# Excluded until I know we have the tests working
ADD_SUBDIRECTORY(testsuite)


################################################################################
# Regular tests:
####

ENABLE_TESTING()
ADD_CUSTOM_TARGET("check" COMMAND "${CMAKE_CTEST_COMMAND}")
ADD_DEPENDENCIES("tests" "check")

FUNCTION(AddTest name)
    ADD_EXECUTABLE("testImpl_${name}" EXCLUDE_FROM_ALL
        "${CMAKE_CURRENT_SOURCE_DIR}/test${name}.cpp")
    TARGET_LINK_LIBRARIES("testImpl_${name}" Swordxx::swordxx)
    ADD_DEPENDENCIES("check" "testImpl_${name}")
    ADD_TEST(NAME "test_${name}" COMMAND "$<TARGET_FILE:testImpl_${name}>")
ENDFUNCTION()

AddTest("CharIsDigit")
AddTest("CharIsHexDigit")
AddTest("CharIsUpper")
AddTest("CharIsLower")
AddTest("CharIsAlpha")
AddTest("CharIsAlnum")
AddTest("CharIsSpace")
AddTest("CharToDecimal")
AddTest("CharToHexadecimal")
AddTest("CharToUpper")
AddTest("FormatXmlAttributeValue")
AddTest("ParseRomanNumeral")
AddTest("ShareRef")
AddTest("SimpleTokenizer")
AddTest("XmlBuilder")
