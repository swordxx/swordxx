
root := ../..
targets := ${root}/lib/libsword.a
clean-targets := none
include ${root}/Makefile.cfg

CFLAGS += -DGLOBCONFPATH=\"${globalconfdir}/sword.conf\"

cpp += swconfig.cpp
cpp += swmgr.cpp
cpp += swfiltermgr.cpp
cpp += encfiltmgr.cpp
cpp += markupfiltmgr.cpp
cpp += filemgr.cpp
cpp += swlocale.cpp
cpp += localemgr.cpp

include ${root}/Makefile.post
