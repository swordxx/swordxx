
root := ../../../..
subdirs := 
pre-targets := 
targets := ${root}/lib/libsword.a
clean-targets := none
include ${root}/Makefile.cfg

cpp += rawgbf.cpp

l = -lsword -lstdc++

gbfidx: gbfidx.o
	$(CC) $(LFLAGS) -o $@ gbfidx.o $(l) $(l)


include ${root}/Makefile.post
