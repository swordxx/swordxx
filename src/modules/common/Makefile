
root := ../../..
subdirs := 
pre-targets := 
targets := ${root}/lib/libsword.a
include ${root}/Makefile.cfg
UNIX=true

cpp += rawstr.cpp
cpp += swcomprs.cpp
cpp += rawverse.cpp
cpp += swcipher.cpp

DEFINES  += -DUNIX
cpp += sapphire.cpp

include ${root}/Makefile.post
