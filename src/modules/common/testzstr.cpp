#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <swcomprs.h>


main(int argc, char **argv)
{
	SWCompress *zobj;
	short size;
	char *tmp;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <\"test string\">\n", argv[0]);
		exit(1);
	}

	zobj = new SWCompress();
	
	zobj->Buf(argv[1]);
	zobj->zBuf(&size);
	tmp = new char [size];
	memcpy(tmp, zobj->zBuf(&size), size);
	zobj->zBuf(&size, tmp);
	delete [] tmp;

	cout << "->" << zobj->Buf() << "<-\n";

	delete zobj;
}
