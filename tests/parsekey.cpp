#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

#include <versekey.h>
#include <localemgr.h>

int main(int argc, char **argv) {
	if ((argc != 2) && (argc != 3)) {
		fprintf(stderr, "usage: %s <\"string to parse\"> [locale name]\n", *argv);
		exit(-1);
	}

	if (argc == 3)
		LocaleMgr::systemLocaleMgr.setDefaultLocaleName(argv[2]);

	VerseKey DefaultVSKey;

	DefaultVSKey = "jas3:1";
	
	ListKey &list = DefaultVSKey.ParseVerseList(argv[1], DefaultVSKey);

	while (!list.Error()) {
		cout << (const char *)list << "\n";
		list++;
	}
	return 0;
}
