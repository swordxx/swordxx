
root := ../../..
subdirs := 
pre-targets := 
targets := ${root}/lib/libsword.a
clean-targets := none
include ${root}/Makefile.cfg
UNIX=true

cpp += rawstr.cpp
cpp += swcomprs.cpp
cpp += lzsscomprs.cpp
cpp += zipcomprs.cpp
cpp += rawverse.cpp
cpp += swcipher.cpp
cpp += zverse.cpp

DEFINES  += -DUNIX
cpp += sapphire.cpp

include ${root}/Makefile.post
