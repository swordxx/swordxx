/*****************************************************************************
 * Bible dictionary index utility
 */


#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


char findbreak(int fd, long *offset, short *size)
{
	char buf[3];
	char rc = 1;
	long offset2;

	memset(buf, 0, sizeof(buf));

	while (read(fd, &buf[sizeof(buf)-1], 1) == 1) {
		if ((buf[0] == 10) && (isdigit(buf[1])) && (isdigit(buf[2]))) {
			if (read(fd, buf, 1) == 1) {
				*offset = lseek(fd, 0, SEEK_CUR) - 3;
				rc = 0;
				if (size) {
					if (!findbreak(fd, &offset2, 0))
						*size = offset2 - *offset;
					else *size = lseek(fd, 0, SEEK_END) - *offset;
					lseek(fd, *offset, SEEK_SET);
				}
				break;
			}
			break;
		}
		memmove(buf, &buf[1], sizeof(buf)-1);
	}
	return rc;
}


void main(int argc, char **argv)
{
	int   fd, ifd;
	long  offset;
	short size;
	char  *buf;
	char  entbuf[6];

	if (argc < 2) {
		fprintf(stderr, "usage: %s <file to process (no .dat)>\n", argv[0]);
		exit(1);
	}

	buf = (char *) calloc(strlen(argv[1]) + 5, 1);

	sprintf(buf, "%s.dat", argv[1]);
	fd = open(buf, O_RDONLY);

	sprintf(buf, "%s.idx", argv[1]);
	ifd = open(buf, O_CREAT|O_WRONLY);

	offset = 0;				/* write offset for intro */
	write(ifd, &offset, 4);
	findbreak(fd, &offset, 0);
	lseek(fd, 0L, SEEK_SET);
	size = offset - 12;
	write(ifd, &size, 2);

	entbuf[5] = 0;				/* delimit string for read below */

	while(!findbreak(fd, &offset, &size)) {
		write(ifd, &offset, 4);
		write(ifd, &size, 2);
		read(fd, entbuf, 5);
		printf("Found: %s...(%ld:%d)\n", entbuf, offset, size);
	}

	free(buf);

	close(ifd);
	close(fd);
}
