#include <stdio.h>
#include <fcntl.h>


char findbreak(int fp, int *offset, int *num1, int *num2, short *size);


main(int argc, char **argv)
{
	int fp, vfp, cfp, bfp;
	long pos;
	short size, tmp;
	int num1, num2, offset, curbook = 0, curchap = 0, curverse = 0;
	char buf[127];

	if (argc != 2) {
		fprintf(stderr, "usage: %s <file to process>\n", argv[0]);
		exit(1);
	}

	if ((fp = open(argv[1], O_RDONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", argv[1]);
		exit(1);
	}

	sprintf(buf, "%s.vss", argv[1]);
	if ((vfp = open(buf, O_CREAT|O_WRONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.cps", argv[1]);
	if ((cfp = open(buf, O_CREAT|O_WRONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	sprintf(buf, "%s.bks", argv[1]);
	if ((bfp = open(buf, O_CREAT|O_WRONLY)) == -1) {
		fprintf(stderr, "Couldn't open file: %s\n", buf);
		exit(1);
	}

	pos  = 0;
	write(bfp, &pos, 4);  /* Book    offset for testament intros */
	pos = 4;
	write(cfp, &pos, 4);  /* Chapter offset for testament intro */


/*	Right now just zero out intros until parsing correctly */
	pos = 0;
	size = 0;
	write(vfp, &pos, 4);  /* Module intro */
	write(vfp, &size, 2);
	write(vfp, &pos, 4);  /* Testament intro */
	write(vfp, &size, 2);

	while (!findbreak(fp, &offset, &num1, &num2, &size)) {

		if (num2 == 1) {  		/* if we're at a new chapter */
			if (num1 == 1) {	/* if we're at a new book */
				pos = lseek(cfp, 0, SEEK_CUR);
				write(bfp, &pos, 4);
				pos = lseek(vfp, 0, SEEK_CUR); /* Book intro (cps) */
				write(cfp, &pos, 4);
				pos  = 0;
				tmp = 0;
				write(vfp, &pos, 4);  /* Book intro (vss) */
				write(vfp, &tmp, 2);
				curbook++;
				curchap = 0;
			}
			pos = lseek(vfp, 0, SEEK_CUR);
			write(cfp, &pos, 4);
			curverse = 1;
			pos  = 0;
			tmp = 0;
			write(vfp, &pos, 4);  /* Chapter intro */
			write(vfp, &tmp, 2);
			curchap++;
		}
		else curverse++;
	
		printf("%2d:%3d:%3d found at offset: %7d\n", curbook, num1, num2, offset);

		if (num1 != curchap) {
			fprintf(stderr, "Error: Found chaptures out of sequence\n", buf);
			break;
		}
		if (num2 != curverse) {
			fprintf(stderr, "Error: Found verses out of sequence\n", buf);
			break;
		}
		write(vfp, &offset, 4);
		write(vfp, &size, 2);
	}
	
	close(vfp);
	close(cfp);
	close(bfp);
	close(fp);
}


char findbreak(int fp, int *offset, int *num1, int *num2, short *size)
{
	char buf[7];
	char buf2[7];
	char loop;
	int offset2, ch2, vs2;
	
	memset(buf, ' ', 7);

	while (1) {
		if (buf[3] == ':') {
			memcpy(buf2, buf, 7);
			for (loop = 0; loop < 7; loop++) {
				if (!isdigit(buf2[loop]))
					buf2[loop] = ' ';
			}
			buf2[3] = 0;
			*num1 = atoi(buf2);
			*num2 = atoi(&buf2[4]);
			if (*num1 && *num2) {
				*offset = lseek(fp, 0, SEEK_CUR);
				sprintf(buf2, "%d", *num2);
				*offset -= 2 - strlen(buf2);
				if (size) {
					if (findbreak(fp, &offset2, &ch2, &vs2, 0)) {
						*size = (short) (lseek(fp, 0, SEEK_END) - (*offset));
					}
					else {
						sprintf(buf2, "%d:%d", ch2, vs2);
						*size = (offset2 - (*offset)) - (strlen(buf2) + 2);
					}
					lseek(fp, *offset, SEEK_SET);
				}
				return 0;
			}
		}
		memmove(buf, &buf[1], 6);
		if (read(fp, &buf[6], 1) != 1)
			return 1;
	}
}

