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
#include <swbuf.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	SWCipher *zobj;
	VerseKey key;
	RawVerse *rawdrv;
	long offset;
	unsigned short size;
	unsigned long len;
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
		rawdrv->findOffset(key.Testament(), key.Index(), &offset, &size);
		SWBuf tmpbuf;
		rawdrv->readText(key.Testament(), offset, size, tmpbuf);
		len = size;
		zobj->cipherBuf(&len, tmpbuf.c_str());
		printf("%s\n", zobj->Buf());
	}
	delete zobj;
	return 0;
}
