root := ..
subdirs := 
pre-targets := 
targets := keytest mgrtest parsekey listtest casttest modtest
bin := keytest mgrtest parsekey listtest modtest
include ${root}/Makefile.cfg

cpp += keytest.cpp
cpp += mgrtest.cpp
cpp += parsekey.cpp
cpp += listtest.cpp
cpp += casttest.cpp
cpp += modtest.cpp

s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l =  -lsword -lstdc++


keytest: keytest.o
	$(CC) $(LFLAGS) -o $@ keytest.o $(l) $(l)

mgrtest: mgrtest.o
	$(CC) $(LFLAGS) -o $@ mgrtest.o $(l) $(l)

parsekey: parsekey.o
	$(CC) $(LFLAGS) -o $@ parsekey.o $(l) $(l)

listtest: listtest.o
	$(CC) $(LFLAGS) -o $@ listtest.o $(l) $(l) -O0

casttest: casttest.o
	$(CC) $(LFLAGS) -o $@ casttest.o $(l) $(l) -O0

modtest: modtest.o
	$(CC) $(LFLAGS) -o $@ modtest.o $(l) $(l) -O0


ifneq ($(nodeps),yes)
-include $(d)
endif
