
root := ../../..
subdirs := 
pre-targets := 
targets := ${root}/lib/libsword.a
clean-targets := none
include ${root}/Makefile.cfg
UNIX=true

cpp += rawstr.cpp
cpp += rawstr4.cpp
cpp += swcomprs.cpp
cpp += lzsscomprs.cpp
ifeq ($(zlib),yes)
cpp += zipcomprs.cpp
endif
cpp += rawverse.cpp
cpp += swcipher.cpp
cpp += zverse.cpp
cpp += entriesblk.cpp

DEFINES  += -DUNIX
cpp += sapphire.cpp

include ${root}/Makefile.post
