commondir = $(top_srcdir)/src/modules/common

lib_libsword_la_SOURCES += $(commondir)/rawstr.cpp
lib_libsword_la_SOURCES += $(commondir)/rawstr4.cpp
lib_libsword_la_SOURCES += $(commondir)/swcomprs.cpp
lib_libsword_la_SOURCES += $(commondir)/lzsscomprs.cpp

if ZLIB
SWZLIB = $(commondir)/zipcomprs.cpp
else
SWZLIB =
endif
lib_libsword_la_SOURCES += $(SWZLIB)
lib_libsword_la_SOURCES += $(commondir)/rawverse.cpp
lib_libsword_la_SOURCES += $(commondir)/swcipher.cpp
lib_libsword_la_SOURCES += $(commondir)/zverse.cpp
lib_libsword_la_SOURCES += $(commondir)/zstr.cpp
lib_libsword_la_SOURCES += $(commondir)/entriesblk.cpp

DEFS  += -DUNIX
lib_libsword_la_SOURCES += $(commondir)/sapphire.cpp

