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

#include <swcipher.h>
#include <versekey.h>
#include <rawverse.h>


main(int argc, char **argv)
{
	SWCipher *zobj;
	VerseKey key;
	RawVerse *rawdrv;
	int ofd[2], oxfd[2];
	long tmpoff = 0, offset, loffset = 0, lzoffset = 0;
	unsigned short size, lsize = 0, lzsize;
	char *tmpbuf;
	
	if (argc != 3) {
		printf("%d %d\n", sizeof(long), sizeof(unsigned short));
		fprintf(stderr, "usage: %s <datapath> \"<key>\"\n", argv[0]);
		exit(1);
	}

	rawdrv = new RawVerse(argv[1]);
	zobj = new SWCipher((unsigned char *)argv[2]);

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
	write(oxfd[0], &lzoffset, 4);
	write(oxfd[0], &lzsize, 2);
	write(oxfd[1], &lzoffset, 4);
	write(oxfd[1], &lzsize, 2);

	key.AutoNormalize(0);
	key.Headings(1);
	for (key.Index(0); (!key.Error()); key++) {
		rawdrv->findoffset(key.Testament(), key.Index(), &offset, &size);
		printf("%s: OLD offset: %d; size: %d\n", (const char *)key, offset, size);

		if ((offset == loffset) && (size == lsize)) {
			printf("using previous offset,size\n", size);
			offset = lseek(oxfd[key.Testament() - 1], 0, SEEK_CUR);
			printf("%ld %ld %d \n", offset, lzoffset, lzsize);
			write(oxfd[key.Testament() - 1], &lzoffset, 4);
			write(oxfd[key.Testament() - 1], &lzsize, 2);
		}
		else {
			lsize   = size;
			loffset = offset;

			if (size) {
				tmpbuf = (char *) calloc(size + 1, 1);
				rawdrv->gettext(key.Testament(), offset, size + 1, tmpbuf);
				zobj->Buf(tmpbuf);
				zobj->cipherBuf((unsigned int *)&size);
				free(tmpbuf);
			}
			offset = lseek(ofd[key.Testament() - 1], 0, SEEK_CUR);
			tmpoff = lseek(oxfd[key.Testament() - 1], 0, SEEK_CUR);
			printf("%s: (%ld) NEW offset: %ld; size: %d\n", (const char *)key, tmpoff, offset, size);
			write(oxfd[key.Testament() - 1], &offset, 4);
			if (size) 
				write(ofd[key.Testament() - 1], zobj->cipherBuf((unsigned int *)&size), size);
			lzoffset = offset;
			write(oxfd[key.Testament() - 1], &size, 2);
			lzsize = size;
		}
	}
	delete zobj;
	close(ofd[0]);
	close(oxfd[0]);
	close(ofd[1]);
	close(oxfd[1]);
}
