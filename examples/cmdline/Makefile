root := ../..
subdirs := 
pre-targets := 
targets := lookup search
bin := lookup search
include ${root}/Makefile.cfg

cpp += lookup.cpp
cpp += search.cpp

s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l = -lsword -lstdc++


lookup: $(o)
	$(CC) $(LFLAGS) -o $@ lookup.o $(l) $(l)

search: $(o)
	$(CC) $(LFLAGS) -o $@ search.o $(l) $(l)


ifneq ($(nodeps),yes)
-include $(d)
endif
