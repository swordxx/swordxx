#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <iostream>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {

	SWMgr library;
	SWModule *kjv = library.getModule("KJV");

	kjv->setKey(argc > 1 ? argv[1] : "Jn.3.16");

	VerseKey *key = (VerseKey *)kjv->getKey();

	int curVerse = key->getVerse();
	int curChapter = key->getChapter();
	int curBook = key->getBook();

	for (key->setVerse(1);
		(key->getBook()    == curBook)    &&
		(key->getChapter() == curChapter) &&
		!kjv->popError();
		(*kjv)++) {
		if (key->getVerse() == curVerse) cout << "* ";
		cout << key->getVerse() << " " << kjv->stripText() << "\n";

	/* ... lots of hacking based on current key ... */
	}

	return 0;
}
