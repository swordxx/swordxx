root := ..
subdirs := 
pre-targets := 
bin := yes
targets := keytest mgrtest parsekey listtest casttest modtest compnone complzss localetest keycast
ifeq ($(zlib),yes)
targets += compzip
endif
include ${root}/Makefile.cfg

