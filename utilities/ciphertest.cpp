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


int main(int argc, char **argv) {
	SWCipher *zobj;
	VerseKey key;
	RawVerse *rawdrv;
	long offset;
	unsigned short size;
	unsigned int len;
	char *tmpbuf;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s <datapath> \"<key>\"\n", argv[0]);
		exit(1);
	}

	rawdrv = new RawVerse(argv[1]);
	zobj = new SWCipher((unsigned char *)argv[2]);

#ifndef O_BINARY
#define O_BINARY 0
#endif


	printf("\n");

	key.AutoNormalize(0);
	key.Headings(1);
	for (key.Index(0); (!key.Error()); key++) {
		rawdrv->findoffset(key.Testament(), key.Index(), &offset, &size);
		tmpbuf = (char *) calloc(size + 1, 1);
		rawdrv->gettext(key.Testament(), offset, size, tmpbuf);
		len = size;
		zobj->cipherBuf(&len, tmpbuf);
		printf("%s\n", zobj->Buf());
				free(tmpbuf);
	}
	delete zobj;
	return 0;
}
