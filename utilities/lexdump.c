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

#ifndef O_BINARY
#define O_BINARY 0
#endif

int main(int argc, char **argv) {
	char *tmpbuf;
	int idxfd, datfd;
	long offset;
	unsigned int size;
	char datbuf[255];

	if (argc != 3) {
		fprintf(stderr, "usage: %s <datapath/datafilebasename> <index>\n", argv[0]);
		exit(1);
	}

	tmpbuf = calloc(strlen(argv[1]) + 11,1);
	sprintf(tmpbuf, "%s.idx", argv[1]);
	idxfd = open(tmpbuf, O_RDONLY|O_BINARY);
	sprintf(tmpbuf, "%s.dat", argv[1]);
	datfd = open(tmpbuf, O_RDONLY|O_BINARY);
	free(tmpbuf);

	offset = atoi(argv[2]) * 6;
	lseek(idxfd, offset, SEEK_SET);
	read(idxfd, &offset, 4);
	read(idxfd, &size, 2);
	printf("offset: %ld; size: %d\n", offset, size);
	lseek(datfd, offset, SEEK_SET);
	read(datfd, datbuf, 40);
	datbuf[40] = 0;
	printf("%s\n", datbuf);
	close(datfd);
	close(idxfd);
	return 0;

}
