mgrdir = $(top_srcdir)/src/mgr

if CONFDEF
globdef = -DGLOBCONFPATH=\"${globalconfdir}/sword.conf\"
else
globdef =
endif

if BUILDSPLITLIB
spdefs = -DSPLITLIB
else
spdefs =
endif

AM_CPPFLAGS += $(globdef) $(spdefs)

libsword_la_SOURCES += $(mgrdir)/swconfig.cpp
libsword_la_SOURCES += $(mgrdir)/swmgr.cpp
libsword_la_SOURCES += $(mgrdir)/swfiltermgr.cpp
libsword_la_SOURCES += $(mgrdir)/encfiltmgr.cpp
mgrsplits = $(mgrdir)/markupfiltmgr.cpp
libsword_la_SOURCES += $(mgrdir)/filemgr.cpp
libsword_la_SOURCES += $(mgrdir)/swlocale.cpp
libsword_la_SOURCES += $(mgrdir)/localemgr.cpp
libsword_la_SOURCES += $(mgrdir)/swcacher.cpp
libsword_la_SOURCES += $(mgrdir)/swsearchable.cpp
libsword_la_SOURCES += $(mgrdir)/installmgr.cpp
libsword_la_SOURCES += $(mgrdir)/stringmgr.cpp

if BUILDSPLITLIB
libsword_imp_la_SOURCES += $(mgrsplits)
else
libsword_la_SOURCES += $(mgrsplits)
endif

