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
		if ((buf[0] == 10) && (buf[1] == '$') && (buf[2] == '$')) {
			while (read(fd, buf, 1) == 1) {
				if (*buf == 10) {
					if (read(fd, buf, 1) == 1) {
						*offset = lseek(fd, 0, SEEK_CUR);
						rc = 0;
						if (size) {
							if (!findbreak(fd, &offset2, 0))
								*size = offset2 - *offset - 13;
							else *size = lseek(fd, 0, SEEK_END) - *offset;
							lseek(fd, *offset, SEEK_SET);
						}
						break;
					}
				}
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

	if (argc < 2) {
		fprintf(stderr, "usage: %s <file to process (no .dat)>\n", argv[0]);
		exit(1);
	}

	buf = (char *) calloc(strlen(argv[1]) + 5, 1);

#ifndef O_BINARY		// O_BINARY is for Borland to be happy.  If we're in GNU, just define it to a NULL mask
#define O_BINARY 0
#endif
	sprintf(buf, "%s.dat", argv[1]);
	fd = open(buf, O_RDONLY|O_BINARY);

	sprintf(buf, "%s.idx", argv[1]);
	ifd = open(buf, O_CREAT|O_WRONLY|O_BINARY);

	offset = 0;				/* write offset for intro */
	write(ifd, &offset, 4);
	findbreak(fd, &offset, 0);
	lseek(fd, 0L, SEEK_SET);
	size = offset - 12;
	write(ifd, &size, 2);

	buf[3] = 0;				/* delimit string for read below */

	while(!findbreak(fd, &offset, &size)) {
		write(ifd, &offset, 4);
		write(ifd, &size, 2);
		read(fd, buf, 3);
		printf("Found: %s...(%ld:%d)\n", buf, offset, size);
	}

	free(buf);

	close(ifd);
	close(fd);
}
