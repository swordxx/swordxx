#include <localemgr.h>

main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s localetest <locale_name> <text>", *argv);
		exit(-1);
	}

	LocaleMgr lm;

	printf("%s\n", lm.translate(argv[1], argv[2]));
}
