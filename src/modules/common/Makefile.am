commondir = $(top_srcdir)/src/modules/common

libsword_la_SOURCES += $(commondir)/rawstr.cpp
libsword_la_SOURCES += $(commondir)/rawstr4.cpp
libsword_la_SOURCES += $(commondir)/swcomprs.cpp
libsword_la_SOURCES += $(commondir)/lzsscomprs.cpp

if ZLIB
SWZLIB = $(commondir)/zipcomprs.cpp
else
SWZLIB =
endif
libsword_la_SOURCES += $(SWZLIB)
SPLITS = $(commondir)/rawverse.cpp
libsword_la_SOURCES += $(commondir)/swcipher.cpp
SPLITS += $(commondir)/zverse.cpp
libsword_la_SOURCES += $(commondir)/zstr.cpp
libsword_la_SOURCES += $(commondir)/entriesblk.cpp

if BUILDSPLITLIB
libsword_la_SOURCES += $(commondir)/zverse2.cpp
libsword_imp_la_SOURCES += $(SPLITS)
else
libsword_la_SOURCES += $(SPLITS)
endif

libsword_la_SOURCES += $(commondir)/sapphire.cpp

