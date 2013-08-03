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

MESSAGE(STATUS "\n-- CONFIGURING LIBRARY TESTS")

SET(test_PROGRAMS
	bibliotest
	casttest
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
	parsekey
	rawldidxtest
	romantest
	striptest
	swaptest
	swbuftest
	testblocks
	utf8norm
	versekeytest
	vtreekeytest
	versemgrtest
	webiftest
	xmltest
)

IF(WITH_ICU)
	SET(test_PROGRAMS
		${test_PROGRAMS}
		icutest
#		tlitmgrtest
		translittest
	)
ENDIF(WITH_ICU)

IF(WITH_ZLIB OR WITH_INTERNAL_ZLIB)
	SET(test_PROGRAMS
		${test_PROGRAMS}
		compzip
	)
ENDIF(WITH_ZLIB OR WITH_INTERNAL_ZLIB)

FOREACH(TEST ${test_PROGRAMS})
	ADD_EXECUTABLE(${TEST} EXCLUDE_FROM_ALL	${TEST}.cpp)
	IF(BUILDING_SHARED)
		TARGET_LINK_LIBRARIES(${TEST}	sword)
	ELSE(BUILDING_SHARED)
		TARGET_LINK_LIBRARIES(${TEST} sword_static)
	ENDIF(BUILDING_SHARED)
ENDFOREACH(TEST ${test_PROGRAMS})

########################################################################################
# The following tests require extra libraries to run
# 
FOREACH(ICUTEST icutest translittest)
	TARGET_LINK_LIBRARIES(${ICUTEST} ${ICU_LIBRARIES} ${ICU_I18N_LIBRARIES})
ENDFOREACH(ICUTEST icutest translittest)

# Excluded until I know we have the tests working
ADD_SUBDIRECTORY(testsuite)
