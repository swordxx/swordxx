filtersdir = $(top_srcdir)/src/modules/filters

libsword_la_SOURCES += $(filtersdir)/swbasicfilter.cpp

libsword_la_SOURCES += $(filtersdir)/gbfhtml.cpp
libsword_la_SOURCES += $(filtersdir)/gbfhtmlhref.cpp
libsword_la_SOURCES += $(filtersdir)/gbfplain.cpp
libsword_la_SOURCES += $(filtersdir)/gbfrtf.cpp
libsword_la_SOURCES += $(filtersdir)/plainhtml.cpp
libsword_la_SOURCES += $(filtersdir)/rwphtml.cpp
libsword_la_SOURCES += $(filtersdir)/rwprtf.cpp
libsword_la_SOURCES += $(filtersdir)/cipherfil.cpp
libsword_la_SOURCES += $(filtersdir)/rtfhtml.cpp

libsword_la_SOURCES += $(filtersdir)/gbfstrongs.cpp
libsword_la_SOURCES += $(filtersdir)/gbffootnotes.cpp
libsword_la_SOURCES += $(filtersdir)/gbfheadings.cpp
libsword_la_SOURCES += $(filtersdir)/gbfmorph.cpp
libsword_la_SOURCES += $(filtersdir)/plainfootnotes.cpp
libsword_la_SOURCES += $(filtersdir)/thmlstrongs.cpp
libsword_la_SOURCES += $(filtersdir)/thmlfootnotes.cpp
libsword_la_SOURCES += $(filtersdir)/thmlheadings.cpp
libsword_la_SOURCES += $(filtersdir)/thmlmorph.cpp
libsword_la_SOURCES += $(filtersdir)/thmllemma.cpp
libsword_la_SOURCES += $(filtersdir)/thmlscripref.cpp
libsword_la_SOURCES += $(filtersdir)/thmlvariants.cpp

libsword_la_SOURCES += $(filtersdir)/gbfthml.cpp
libsword_la_SOURCES += $(filtersdir)/thmlgbf.cpp
libsword_la_SOURCES += $(filtersdir)/thmlrtf.cpp
libsword_la_SOURCES += $(filtersdir)/thmlhtml.cpp
libsword_la_SOURCES += $(filtersdir)/thmlhtmlhref.cpp
libsword_la_SOURCES += $(filtersdir)/thmlosis.cpp
libsword_la_SOURCES += $(filtersdir)/gbfosis.cpp
libsword_la_SOURCES += $(filtersdir)/thmlplain.cpp

libsword_la_SOURCES += $(filtersdir)/unicodertf.cpp
libsword_la_SOURCES += $(filtersdir)/latin1utf8.cpp
libsword_la_SOURCES += $(filtersdir)/latin1utf16.cpp
libsword_la_SOURCES += $(filtersdir)/utf8utf16.cpp
libsword_la_SOURCES += $(filtersdir)/utf16utf8.cpp
libsword_la_SOURCES += $(filtersdir)/scsuutf8.cpp
libsword_la_SOURCES += $(filtersdir)/utf8html.cpp
libsword_la_SOURCES += $(filtersdir)/utf8latin1.cpp

libsword_la_SOURCES += $(filtersdir)/thmlolb.cpp

libsword_la_SOURCES += $(filtersdir)/greeklexattribs.cpp

if ICU
ICUDEFS = -DICU
SWICUSRC = $(filtersdir)/utf8transliterator.cpp
SWICUSRC += $(filtersdir)/utf8nfc.cpp
SWICUSRC += $(filtersdir)/utf8nfkd.cpp
SWICUSRC += $(filtersdir)/utf8arshaping.cpp
SWICUSRC += $(filtersdir)/utf8bidireorder.cpp
else
SWICUSRC =
ICUDEFS = 
endif
libsword_la_SOURCES += $(SWICUSRC)
DEFS += $(ICUDEFS)

libsword_la_SOURCES += $(filtersdir)/utf8cantillation.cpp
libsword_la_SOURCES += $(filtersdir)/utf8hebrewpoints.cpp
libsword_la_SOURCES += $(filtersdir)/utf8greekaccents.cpp

