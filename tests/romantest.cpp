#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <roman.h>

#ifndef NO_SWORD_NAMESPACE
using sword::from_rom;
#endif

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <roman_numeral>\n", *argv);
		exit(-1);
	}
	/* I don't think we need to_rom, do we? anyway, it isn't written
	char buf[127];
	if (isdigit(argv[1][0])) {
		to_rom(atoi(argv[1]), buf);
		std::cout << buf << std::endl;
	}
	*/
	else std::cout << from_rom(argv[1]) << std::endl;
	return 0;
}

