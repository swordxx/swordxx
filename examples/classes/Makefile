root := ../..
subdirs := 
pre-targets := 
targets := swmgrex getbook
bin := swmgrex getbook
include ${root}/Makefile.cfg

cpp += swmgrex.cpp
cpp += getbook.cpp


s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l = -lsword -lstdc++


swmgrex: $(o)
	$(CC) $(LFLAGS) -o $@ $@.o $(l) $(l)

getbook: $(o)
	$(CC) $(LFLAGS) -o $@ $@.o $(l) $(l)


ifneq ($(nodeps),yes)
-include $(d)
endif
