
root := ..
subdirs :=
pre-targets := 
targets :=

lib := libsword.a
ifeq (${buildshare},yes)
lib += libsword.so
endif

include ${root}/Makefile.cfg

libsword.so:
	$(CC) -fpic -shared -o ${root}/lib/libsword.so ${objdir}/*.so

include ${root}/Makefile.post
