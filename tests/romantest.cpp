#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
extern "C" {
#include <roman.h>
}

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <roman_numeral>\n", *argv);
		exit(-1);
	}
	/* I don't think we need to_rom, do we? anyway, it isn't written
	char buf[127];
	if (isdigit(argv[1][0])) {
		to_rom(atoi(argv[1]), buf);
		cout << buf << "\n";
	}
	*/
	else cout << from_rom(argv[1]) << "\n";
	return 0;
}

