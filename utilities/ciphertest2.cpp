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

#include <cipherfil.h>
#include <versekey.h>
#include <rawtext.h>


int main(int argc, char **argv) {
	VerseKey key;
	RawText *rawdrv;
	
	if (argc != 3) {
		fprintf(stderr, "usage: %s <datapath> \"<key>\"\n", argv[0]);
		exit(1);
	}

	rawdrv = new RawText(argv[1]);
	rawdrv->AddRawFilter(new CipherFilter(argv[2]));

	printf("\n");

	key.AutoNormalize(0);
	key.Headings(1);
	for (key.Index(0); (!key.Error()); key++) {
		rawdrv->Key(key);
		printf("%s: %s\n", (const char *)key, (const char *)*rawdrv);
	}
	return 0;
}
