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

if BUILDNEWVKEY
zv2 = $(commondir)/zverse2.cpp
else
zv2 =
endif
libsword_la_SOURCES += $(SPLITS) $(zv2)

libsword_la_SOURCES += $(commondir)/sapphire.cpp

