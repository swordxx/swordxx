root := ..
subdirs := 
pre-targets := 
targets := cipherraw ciphertest ciphertest2 lexdump
include ${root}/Makefile.cfg

cpp += cipherraw.cpp
cpp += cipherrawld.cpp
cpp += ciphertest.cpp
cpp += ciphertest2.cpp

c += lexdump.c

s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l =  -lsword -lstdc++


cipherraw: cipherraw.o
	$(CC) $(LFLAGS) -o $@ cipherraw.o $(l) $(l) -O0

cipherrawld: cipherrawld.o
	$(CC) $(LFLAGS) -o $@ cipherrawld.o $(l) $(l) -O0

ciphertest: ciphertest.o
	$(CC) $(LFLAGS) -o $@ ciphertest.o $(l) $(l) -O0

ciphertest2: ciphertest2.o
	$(CC) $(LFLAGS) -o $@ ciphertest2.o $(l) $(l) -O0

lexdump: lexdump.o
	$(CC) $(LFLAGS) -o $@ lexdump.o $(l) $(l) -O0


ifneq ($(nodeps),yes)
-include $(d)
endif
