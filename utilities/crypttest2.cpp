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

#include <cryptfilt.h>
#include <versekey.h>
#include <rawtext.h>


main(int argc, char **argv)
{
	VerseKey key;
	RawText *rawdrv;
	int ofd[2], oxfd[2];
	long offset, loffset = 0, lzoffset = 0;
	unsigned short size, lsize = 0, lzsize;
	unsigned int len;
	char *tmpbuf;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s <datapath> \"<key>\"\n", argv[0]);
		exit(1);
	}

	rawdrv = new RawText(argv[1]);
	rawdrv->AddOptionFilter(new CryptFilter(argv[2]));

	printf("\n");

	key.AutoNormalize(0);
	key.Headings(1);
	for (key.Index(0); (!key.Error()); key++) {
		rawdrv->Key(key);
		printf("%s\n", (const char *)*rawdrv);
	}
}
