root := ..
subdirs := 
pre-targets := 
bin := yes
targets := keytest mgrtest parsekey listtest casttest modtest compnone complzss localetest keycast introtest indextest configtest romantest testblocks filtertest swaptest
ifeq ($(icu),yes)
targets += icutest
endif
ifeq ($(zlib),yes)
targets += compzip
endif
include ${root}/Makefile.cfg

