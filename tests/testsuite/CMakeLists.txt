#############################################################################
# This file will actually be responsible for running the tests
#

FILE(WRITE "${CMAKE_CURRENT_BINARY_DIR}/sword.conf" "[Install]\nLocalePath=${CMAKE_CURRENT_SOURCE_DIR}/../../")

ADD_CUSTOM_TARGET(
	tests_configure
	COMMAND cp "${CMAKE_CURRENT_SOURCE_DIR}/*.{sh,good,imp,txt,xml}" "${CMAKE_CURRENT_BINARY_DIR}"
	DEPENDS ${test_PROGRAMS}
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_CUSTOM_TARGET(
	tests
	COMMAND ./runall.sh
	WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_DEPENDENCIES(
	tests
	tests_configure
)

MESSAGE(STATUS "Testing configured.")
