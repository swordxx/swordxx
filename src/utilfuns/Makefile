
root := ../..
targets := ${root}/lib/libsword.a
clean-targets := none
include ${root}/Makefile.cfg

cpp += Greek2Greek.cpp
cpp += utilstr.cpp
cpp += unixstr.cpp

ifeq (${need_local_regex}, yes)
c += regex.c
endif

c += utilstr.c

include ${root}/Makefile.post
