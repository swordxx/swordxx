#include <stdio.h>
#include <fcntl.h>
#include <versekey.h>


char findbreak(int fp, int *offset, int *num1, int *num2, short *size);


main(int argc, char **argv)
{
	int fp, vfp, cfp, bfp;
	long pos;
	short size, tmp;
	int num1, num2, offset, curbook = 0, curchap = 0, curverse = 0;
	char buf[127];
	VerseKey mykey;

	if ((argc < 2) || (argc > 3)) {
		fprintf(stderr, "usage: %s <file to process> [nt]\n", argv[0]);
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

	mykey = (argc == 3) ? "Matthew 1:1" : "Genesis 1:1";

	while (!findbreak(fp, &offset, &num1, &num2, &size)) {
		num1 = mykey.Chapter();
		num2 = mykey.Verse();
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
			fprintf(stderr, "Error: Found chaptures out of sequence\n");
			break;
		}
		if (num2 != curverse) {
			fprintf(stderr, "Error: Found verses out of sequence\n");
			break;
		}
		write(vfp, &offset, 4);
		write(vfp, &size, 2);
		mykey++;
	}
	
	close(vfp);
	close(cfp);
	close(bfp);
	close(fp);
}


char findbreak(int fp, int *offset, int *num1, int *num2, short *size)
{
	char buf[17];
	char buf2[7];
	char buf3[7];
	char loop;
	char offadj, inquotes, sizeadj;
	int offset2, ch2, vs2;
	
	strcpy (buf3, "\\par  ");
	buf3[5] = 10;
	memset(buf, ' ', 17);

	while (1) {
		offadj = -100;
		inquotes = 0;
		sizeadj = 0;
  		if (!memcmp(buf, "\\par FIN DEL NUEVO TESTAMENTO", 16)) {
			offadj = -11;
//			inquotes = 1;
			sizeadj = -7;
		}
		
  		if ((!memcmp(buf, buf3, 6)) && (!size)) {
			offadj = -11;
//			inquotes = 1;
			sizeadj = -7;
		}
  		if (!memcmp(buf, "\\par  ", 6)) {
			if (isdigit(buf[6])) {
				for (loop = 7; loop < 10; loop++) {
					if (!isdigit(buf[loop]))
						break;
				}
				offadj = -(11 - (loop - 6));
	//			inquotes = 1;
				sizeadj = -7;
			}
		}
/*
  		if (!memcmp(buf, "\\fi200\\widctlpar", 16)) {
			offadj = -1;
//			inquotes = 1;
			sizeadj = -18;
		}
*/
		if (offadj > -100) {
			*offset = lseek(fp, 0, SEEK_CUR) + offadj;
			if (size) {
				(*offset)++;
				while (inquotes) {
					while (read(fp, buf2, 1) == 1) {
						if (*buf2 == '}')
							break;
						(*offset)++;
					}
					inquotes--;
				}
				if (findbreak(fp, &offset2, &ch2, &vs2, 0)) {
					*size = (short) (lseek(fp, 0, SEEK_END) - (*offset));
				}
				else {
					*size = (offset2 - (*offset));
				}
				lseek(fp, *offset-sizeadj, SEEK_SET);
			}
			else (*offset) += sizeadj;
			return 0;
		}
		memmove(buf, &buf[1], 16);
		if (read(fp, &buf[16], 1) != 1)
			return 1;
	}
}

