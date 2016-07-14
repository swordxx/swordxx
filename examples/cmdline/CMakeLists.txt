##################################################################################
# These are some examples.  If you want examples to be built, you need to define
# SWORDXX_BUILD_EXAMPLES="YES".  These will not be installed, so don't worry about
# that.
#

SET(example_PROGRAMS
    listoptions
    lookup
    outplain
    outrender
    search
    simple
    stripaccents
    verserangeparse
)

FOREACH(DEMO ${example_PROGRAMS})
    ADD_EXECUTABLE("${DEMO}" "${SWORDXX_EXAMPLES_EXCLUDE}" "${DEMO}.cpp")
    TARGET_LINK_LIBRARIES("${DEMO}" swordxx)
ENDFOREACH(DEMO ${example_PROGRAMS})

##################################################################################
# This example uses pthreads explicity.  I don't know if it supports anything else,
# but until I find out, I will force it to find PTHREADS only
#

FIND_PACKAGE(Threads)

IF(CMAKE_USE_PTHREADS_INIT OR CMAKE_HP_PTHREADS_INIT)
    ADD_EXECUTABLE(threaded_search "${SWORDXX_EXAMPLES_EXCLUDE}" threaded_search.cpp)
    TARGET_LINK_LIBRARIES(threaded_search
        swordxx
        ${CMAKE_THREAD_LIBS_INIT}
    )
ENDIF(CMAKE_USE_PTHREADS_INIT OR CMAKE_HP_PTHREADS_INIT)
