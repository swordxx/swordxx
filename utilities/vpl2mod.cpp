#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <swmgr.h>
#include <rawtext.h>
#include <iostream.h>

#ifndef O_BINARY
#define O_BINARY 0
#endif

char readline(int fd, char **buf) {
	char ch;
	if (*buf)
		delete [] *buf;
	*buf = 0;
	int len;

	long index = lseek(fd, 0, SEEK_CUR);
	while ((len = read(fd, &ch, 1)) == 1) {
		if (ch == 10)
			break;
	}
	
	int size = (lseek(fd, 0, SEEK_CUR) - index) - 1;

	if (size >= 0) {
		*buf = new char [ size + 1 ];

		lseek(fd, index, SEEK_SET);
		read(fd, *buf, size);
		(*buf)[size] = 0;

		// clean up any trailing 13 on dosfs
		while (read(fd, &ch, 1) == 1) {
			if (ch != 13)
				break;
		}
		return 0;
	}
	return -1;
}


int main(int argc, char **argv) {

	if (argc < 2) {
//		fprintf(stderr, "usage: %s <vpl_file> </path/to/mod> [0|1 - file includes prepended verse references]\n", argv[0]);
		fprintf(stderr, "usage: %s <vpl_file> </path/to/mod>\n", argv[0]);
		exit(-1);
	}

	int fd = open(argv[1], O_RDONLY|O_BINARY);
	if (fd < 0) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], argv[1]);
		exit(-1);
	}

	if (RawVerse::CreateModule(argv[2])) {
		fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
		exit(-2);
	}

	bool vref = false;
	if (argc > 2)
		vref = (argv[2][0] == '0') ? false : true;

	char *buffer = 0;
	RawText mod(argv[2]);

	VerseKey *vkey = (VerseKey *)(SWKey *)mod;

	vkey->Headings(1);	// turn on mod/testmnt/book/chap headings

	mod = TOP;

	while ((!mod.Error()) && (!readline(fd, &buffer))) {
		mod << buffer;
		mod++;
	}

	if (buffer)
		delete [] buffer;
}
