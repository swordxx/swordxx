
root := ..
subdirs :=
pre-targets := 
targets :=

lib := libsword.a
libshare += libsword.so

include ${root}/Makefile.cfg

libsword.so:
	$(CC) -fpic -shared -o ${root}/lib/libsword.so ${objdir}/*.so

include ${root}/Makefile.post
