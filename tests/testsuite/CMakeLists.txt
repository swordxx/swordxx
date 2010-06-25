#############################################################################
# This file will actually be responsible for running the tests
# 

ADD_CUSTOM_TARGET(
	tests_configure
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/*.sh ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/*.good ${CMAKE_CURRENT_BINARY_DIR}
	COMMAND echo \"[Install]\\nLocalePath=${CMAKE_CURRENT_SOURCE_DIR}/../../\" > ${CMAKE_CURRENT_BINARY_DIR}/sword.conf
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
