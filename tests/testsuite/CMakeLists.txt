#############################################################################
# This file will actually be responsible for running the tests
#

ADD_CUSTOM_TARGET(
    tests_configure
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/*.sh ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/*.imp ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/*.xml ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/*.txt ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/*.good ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND echo "[Install]" > ${CMAKE_CURRENT_BINARY_DIR}/swordxx.conf
    COMMAND echo "LocalePath=${CMAKE_SOURCE_DIR}/" >> ${CMAKE_CURRENT_BINARY_DIR}/swordxx.conf
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
