#include <stdio.h>
#include <fcntl.h>
#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif


char findbreak(int fd, long *offset, int *num1, int *num2, short *size);


main(int argc, char **argv)
{
	int fd, vfd, cfd, bfd;
	long pos, offset;
	short size, tmp;
	int num1, num2, curbook = 0, curchap = 0, curverse = 0;
	char buf[127];

	if (argc != 2) {
		fprintf(stderr, "usage: %s <file to process>\n", argv[0]);
		exit(1);
	}
#ifndef O_BINARY		// O_BINARY is for Borland to be happy.  If we're in GNU, just define it to a NULL mask
#define O_BINARY 0
#endif
	if ((fd = open(argv[1], O_RDONLY|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", argv[1]);
		exit(1);
	}

	sprintf(buf, "%s.vss", argv[1]);
	if ((vfd = open(buf, O_CREAT|O_WRONLY|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.cps", argv[1]);
	if ((cfd = open(buf, O_CREAT|O_WRONLY|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.bks", argv[1]);
	if ((bfd = open(buf, O_CREAT|O_WRONLY|O_BINARY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	pos  = 0;
	write(bfd, &pos, 4);  /* Book    offset for testament intros */
	pos = 4;
	write(cfd, &pos, 4);  /* Chapter offset for testament intro */


/*	Right now just zero out intros until parsing correctly */
	pos = 0;
	size = 0;
	write(vfd, &pos, 4);  /* Module intro */
	write(vfd, &size, 2);
	write(vfd, &pos, 4);  /* Testament intro */
	write(vfd, &size, 2);

	while (!findbreak(fd, &offset, &num1, &num2, &size)) {

		if (num2 == 1) {  		/* if we're at a new chapter */
			if (num1 == 1) {	/* if we're at a new book */
				pos = lseek(cfd, 0, SEEK_CUR);
				write(bfd, &pos, 4);
				pos = lseek(vfd, 0, SEEK_CUR); /* Book intro (cps) */
				write(cfd, &pos, 4);
				pos  = 0;
				tmp = 0;
				write(vfd, &pos, 4);  /* Book intro (vss) */
				write(vfd, &tmp, 2);
				curbook++;
				curchap = 0;
			}
			pos = lseek(vfd, 0, SEEK_CUR);
			write(cfd, &pos, 4);
			curverse = 1;
			pos  = 0;
			tmp = 0;
			write(vfd, &pos, 4);  /* Chapter intro */
			write(vfd, &tmp, 2);
			curchap++;
		}
		else curverse++;
	
		printf("%2d:%3d:%3d found at offset: %7ld\n", curbook, num1, num2, offset);

		if (num1 != curchap) {
			fprintf(stderr, "Error: Found chaptures out of sequence (%2d:%3d:%3d)\n", curbook, num1-1, num2);
			curchap = num1;
//			break;
		}
		if (num2 != curverse) {
			fprintf(stderr, "Error: Found verses out of sequence (%2d:%3d:%3d)\n", curbook, num1, num2-1);
//			break;
			tmp = 0;
			curverse = num2;
			write(vfd, &offset, 4);
			write(vfd, &tmp, 2);
		}
		write(vfd, &offset, 4);
		write(vfd, &size, 2);
	}
	
	close(vfd);
	close(cfd);
	close(bfd);
	close(fd);
}


char findbreak(int fd, long *offset, int *num1, int *num2, short *size)
{
	char buf[8];
	char buf2[7];
	char loop, len;

	memset(buf, ' ', 7);
	buf[7] = 0;

	while (1) {

		memmove(buf, &buf[1], 6);
		if (read(fd, &buf[6], 1) != 1)
			return 1;

		if ((buf[0] == 10) && (isdigit(buf[1]))) {
			sscanf(buf, "%d %s", num2, buf2);
			if (!strncmp(buf2, "KAP", 3)) {
				*num1 = *num2;
				continue;
			}
			sprintf(buf, "%d", *num2);
			(*offset)  = lseek(fd, 0, SEEK_CUR);
			(*offset) -= (5-strlen(buf));
			for (len = 1; len == 1; len = read(fd, &loop, 1)) {
				if ((loop == 10) || (loop == 13))
					break;
			}

			*size = (short)(lseek(fd, 0, SEEK_CUR) - *offset) - 1;
			lseek(fd, -1, SEEK_CUR);
			break;
		}
	}
	return 0;
}

