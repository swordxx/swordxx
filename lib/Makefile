
root := ..
subdirs :=
pre-targets := 
targets :=
clean-targets := libsword.a libsword.so

lib := libsword.a
libshare += libsword.so

include ${root}/Makefile.cfg

libsword.so:
	$(CC) -fpic -shared -o ${root}/lib/libsword.so ${objdir}/*.so


include ${root}/Makefile.post
