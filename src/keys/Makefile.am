keysdir = $(top_srcdir)/src/keys

libsword_la_SOURCES += $(keysdir)/swkey.cpp
libsword_la_SOURCES += $(keysdir)/listkey.cpp
libsword_la_SOURCES += $(keysdir)/strkey.cpp
libsword_la_SOURCES += $(keysdir)/treekey.cpp
libsword_la_SOURCES += $(keysdir)/treekeyidx.cpp


if BUILDNEWVKEY
versekey = $(keysdir)/versekey2.cpp $(keysdir)/versekey.cpp \
	$(keysdir)/refsys.cpp
else
versekey = $(keysdir)/versekey.cpp
endif

libsword_la_SOURCES += $(versekey)

