keysdir = $(top_srcdir)/src/keys

libsword_la_SOURCES += $(keysdir)/swkey.cpp
libsword_la_SOURCES += $(keysdir)/listkey.cpp
libsword_la_SOURCES += $(keysdir)/strkey.cpp
libsword_la_SOURCES += $(keysdir)/treekey.cpp
libsword_la_SOURCES += $(keysdir)/treekeyidx.cpp

if BUILDNEWVKEY
versekey = $(keysdir)/versekey2.cpp
vkdef = -DVK2
else
versekey = $(keysdir)/versekey.cpp
vkdef =
endif

if BUILDSPLITLIB
libsword_imp_la_SOURCES += $(keysdir)/versekey.cpp
versekey = $(keysdir)/versekey2.cpp
vkdef =
endif

libsword_la_SOURCES += $(versekey)
AM_CPPFLAGS += $(vkdef)

