filtersdir = $(top_srcdir)/src/modules/filters

lib_libsword_la_SOURCES += $(filtersdir)/swbasicfilter.cpp

lib_libsword_la_SOURCES += $(filtersdir)/gbfhtml.cpp
lib_libsword_la_SOURCES += $(filtersdir)/gbfhtmlhref.cpp
lib_libsword_la_SOURCES += $(filtersdir)/gbfplain.cpp
lib_libsword_la_SOURCES += $(filtersdir)/gbfrtf.cpp
lib_libsword_la_SOURCES += $(filtersdir)/plainhtml.cpp
lib_libsword_la_SOURCES += $(filtersdir)/rwphtml.cpp
lib_libsword_la_SOURCES += $(filtersdir)/rwprtf.cpp
lib_libsword_la_SOURCES += $(filtersdir)/cipherfil.cpp
lib_libsword_la_SOURCES += $(filtersdir)/rtfhtml.cpp

lib_libsword_la_SOURCES += $(filtersdir)/gbfstrongs.cpp
lib_libsword_la_SOURCES += $(filtersdir)/gbffootnotes.cpp
lib_libsword_la_SOURCES += $(filtersdir)/gbfheadings.cpp
lib_libsword_la_SOURCES += $(filtersdir)/gbfmorph.cpp
lib_libsword_la_SOURCES += $(filtersdir)/plainfootnotes.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlstrongs.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlfootnotes.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlheadings.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlmorph.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmllemma.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlscripref.cpp

lib_libsword_la_SOURCES += $(filtersdir)/gbfthml.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlgbf.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlrtf.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlhtml.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlhtmlhref.cpp
lib_libsword_la_SOURCES += $(filtersdir)/thmlplain.cpp

lib_libsword_la_SOURCES += $(filtersdir)/unicodertf.cpp
lib_libsword_la_SOURCES += $(filtersdir)/latin1utf8.cpp
lib_libsword_la_SOURCES += $(filtersdir)/latin1utf16.cpp
lib_libsword_la_SOURCES += $(filtersdir)/utf8utf16.cpp
lib_libsword_la_SOURCES += $(filtersdir)/utf16utf8.cpp
lib_libsword_la_SOURCES += $(filtersdir)/scsuutf8.cpp
lib_libsword_la_SOURCES += $(filtersdir)/utf8html.cpp
lib_libsword_la_SOURCES += $(filtersdir)/utf8latin1.cpp

lib_libsword_la_SOURCES += $(filtersdir)/thmlolb.cpp

if ICU
SWICUSRC = $(filtersdir)/utf8transliterator.cpp
SWICUSRC += $(filtersdir)/utf8nfc.cpp
SWICUSRC += $(filtersdir)/utf8nfkd.cpp
SWICUSRC += $(filtersdir)/utf8arshaping.cpp
SWICUSRC += $(filtersdir)/utf8bidireorder.cpp
else
SWICUSRC =
endif
lib_libsword_la_SOURCES += $(SWICUSRC)

lib_libsword_la_SOURCES += $(filtersdir)/utf8cantillation.cpp
lib_libsword_la_SOURCES += $(filtersdir)/utf8hebrewpoints.cpp
lib_libsword_la_SOURCES += $(filtersdir)/utf8greekaccents.cpp

