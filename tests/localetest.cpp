#include <localemgr.h>

int main(int argc, char **argv) {
	if (argc != 3) {
		fprintf(stderr, "usage: %s <locale_name> <text>\n", *argv);
		exit(-1);
	}

	LocaleMgr lm;

	printf("%s\n", lm.translate(argv[1], argv[2]));
}
