LDADD = $(top_builddir)/lib/libsword.la

# Rules for the test code (use `make check` to execute)
TESTS = LibSword
check_PROGRAMS = $(TESTS)
LibSword_SOURCES = main.cpp url_test.cpp versekey_test.cpp
LibSword_CXXFLAGS = $(CPPUNIT_CFLAGS)
LibSword_LDFLAGS = -lcppunit -ldl

swcppunitpdir = $(top_srcdir)/tests/cppunit

all: check