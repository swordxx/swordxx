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

#include <swcomprs.h>


main(int argc, char **argv)
{
	SWCompress *zobj;
	VerseKey key;
	RawVerse rawdrv;
	int ofd[2], oxfd[2];
	long offset, loffset, lzoffset;
	short size, lsize, lzsize;
	char *tmpbuf;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <datapath>\n", argv[0]);
		exit(1);
	}

	zobj = new SWCompress();
	rawdrv = new RawVerse(argv[1]);

#ifndef O_BINARY
#define O_BINARY 0
#endif

	tmpbuf = new char [ strlen(argv[1]) + 11 ];
	sprintf(tmpbuf, "%sot.zzz", argv[1]);
	ofd[0] = open(tmpbuf, O_WRONLY|O_BINARY|O_CREAT);
	sprintf(tmpbuf, "%sot.zzz.vss", argv[1]);
	oxfd[0] = open(tmpbuf, O_WRONLY|O_BINARY|O_CREAT);
	sprintf(tmpbuf, "%snt.zzz", argv[1]);
	ofd[1] = open(tmpbuf, O_WRONLY|O_BINARY|O_CREAT);
	sprintf(tmpbuf, "%snt.zzz.vss", argv[1]);
	oxfd[1] = open(tmpbuf, O_WRONLY|O_BINARY|O_CREAT);

	delete [] tmpbuf;

	printf("\n");

	key.AutoNormalize(0);
	key.Index(0);
	while (!key.Error()) {
		findoffset(key.Testament(), key.Index(), &start, &size);
		if (read(ixfd, &offset, 4) != 4)
			break;
		if (read(ixfd, &size, 2) != 2)
			break;

		if ((offset == loffset) && (size == lsize)) {
			printf("using previous offset,size\n", size);
			write(oxfd, &lzoffset, 4);
			write(oxfd, &lzsize, 2);
		}
		else {
			printf("%d -> ", size);
			lsize   = size;
			loffset = offset;

			if (size) {
				tmpbuf = (char *) calloc(size + 1, 1);
				lseek(ifd, offset, SEEK_SET);
				read(ifd, tmpbuf, size);
				zobj->Buf(tmpbuf);
				zobj->zBuf(&size);
				free(tmpbuf);
			}
			offset = lseek(ofd, 0, SEEK_END);
			write(oxfd, &offset, 4);
			if (size) 
				write(ofd, zobj->zBuf(&size), size);
			lzoffset = offset;
			write(oxfd, &size, 2);
			lzsize = size;
			printf("%d \n", size);
		}
	}
	delete zobj;
}
