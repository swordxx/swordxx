#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include <versekey.h>

int main(int argc, char **argv) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <\"string to parse\">\n", *argv);
		exit(-1);
	}
	VerseKey DefaultVSKey;

	DefaultVSKey = "jas3:1";
	
	ListKey &list = VerseKey::ParseVerseList(argv[1], DefaultVSKey);

	while (!list.Error()) {
		cout << (const char *)list << "\n";
		list++;
	}
	return 0;
}
