
#include <stdio.h>
#include <stdlib.h>

main(int argc, char **argv)
{
	FILE *fp;
	char *buf;
	int size;

	if ((argc < 3) || (argc > 4)) {
		fprintf(stderr, "usage: %s <string> <substitute string> [filename]\n", *argv);
		exit(-1);
	}

	if (argc > 3)
		fp = fopen(argv[3], "r");
	else	fp = stdin;

	size = strlen(argv[1]);
	buf = (char *)calloc(size + 1, 1);

	while ((buf[size - 1] = fgetc(fp)) != EOF) {
		if (!strcmp(buf, argv[1])) {
			printf("\n%s", argv[2]);
			memset(buf, 0, size);
			continue;
		}
		if (*buf) {
			printf("%c", *buf);
		}
		memmove(buf, &buf[1], size);
	}
	buf[size - 1] = 0;
	printf("%s", buf);
}
