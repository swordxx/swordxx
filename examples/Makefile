
root := ..
subdirs := windoze X11 classes cmdline
pre-targets := 
include ${root}/Makefile.cfg


s = $(c) $(cpp)
d = $(c:%.c=%.d) $(cpp:%.cpp=%.d)
o = $(c:%.c=%.o) $(cpp:%.cpp=%.o)

ifneq ($(nodeps),yes)
-include $(d)
endif
