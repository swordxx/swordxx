root := ../..
subdirs := 
pre-targets := 
bin := yes
targets := lookup search threaded_search
include ${root}/Makefile.cfg

ifneq ($(nodeps),yes)
-include $(d)
endif

LIBS += -lpthread