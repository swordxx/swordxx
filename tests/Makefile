root := ..
subdirs := 
pre-targets := 
bin := yes
targets := keytest mgrtest parsekey listtest casttest modtest compnone complzss compzip
include ${root}/Makefile.cfg


l =  -lsword -lstdc++

keytest: keytest.o
	$(CC) $(LFLAGS) -o $@ keytest.o $(l) $(l)

mgrtest: mgrtest.o
	$(CC) $(LFLAGS) -o $@ mgrtest.o $(l) $(l)

parsekey: parsekey.o
	$(CC) $(LFLAGS) -o $@ parsekey.o $(l) $(l)

listtest: listtest.o
	$(CC) $(LFLAGS) -o $@ listtest.o $(l) $(l) -O0

casttest: casttest.o
	$(CC) $(LFLAGS) -o $@ casttest.o $(l) $(l) -O0

modtest: modtest.o
	$(CC) $(LFLAGS) -o $@ modtest.o $(l) $(l) -O0

compnone: compnone.o
	$(CC) $(LFLAGS) -o $@ compnone.o $(l) $(l) -O0

complzss: complzss.o
	$(CC) $(LFLAGS) -o $@ complzss.o $(l) $(l) -O0

compzip: compzip.o
	$(CC) $(LFLAGS) -o $@ compzip.o $(l) $(l) -O0


ifneq ($(nodeps),yes)
-include $(d)
endif
