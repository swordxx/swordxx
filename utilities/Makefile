root := ..
subdirs := 
pre-targets := 
targets := cryptraw crypttest crypttest2
bin := cryptraw
include ${root}/Makefile.cfg

cpp += cryptraw.cpp
cpp += crypttest.cpp
cpp += crypttest2.cpp

s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l =  -lsword -lstdc++


cryptraw: cryptraw.o
	$(CC) $(LFLAGS) -o $@ cryptraw.o $(l) $(l) -O0

crypttest: crypttest.o
	$(CC) $(LFLAGS) -o $@ crypttest.o $(l) $(l) -O0

crypttest2: crypttest2.o
	$(CC) $(LFLAGS) -o $@ crypttest2.o $(l) $(l) -O0


ifneq ($(nodeps),yes)
-include $(d)
endif
