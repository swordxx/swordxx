root := ../../..
subdirs := 
pre-targets := 
targets := diatheke
bin := diatheke
include ${root}/Makefile.cfg

cpp = diatheke.cpp corediatheke.cpp diathekemgr.cpp

s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)
l = -lsword -lstdc++ -lz
ifeq ($(system),beos)
l = -lsword -lstdc++.r4 -lz
endif
ifeq ($(icu),yes)
l += -licuuc -licui18n
endif

diatheke: $(o)
	$(CC) $(LFLAGS) -o $@ $(o) $(l)

ifneq ($(nodeps),yes)
-include $(d)
endif





