#include <iostream>

#include <versekey.h>
#include <localemgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	if ((argc < 2) || (argc > 4)) {
		fprintf(stderr, "usage: %s <\"string to parse\"> [locale_name] [test-in-set-verse]\n", *argv);
		exit(-1);
	}

	if (argc > 2)
		LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(argv[2]);

	VerseKey DefaultVSKey;

	DefaultVSKey = "jas3:1";
	
	ListKey verses = DefaultVSKey.ParseVerseList(argv[1], DefaultVSKey, true);

	std::cout << verses.getOSISRefRangeText() << "\n";

	if (argc > 3) {
		verses.setText(argv[3]);
		std::cout << "Verse is" << ((verses.Error()) ? " NOT" : "") << " in set.\n\n";
	}
	
	return 0;
}
