
#include <iostream>

#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {

	const char *modName = "HunKar";

	SWMgr library;

	SWModule *book = library.getModule(modName);

	if (!book) {
		cerr << "Can't find module: " << modName << endl;
		return -1;
	}

	VerseKey *key = (VerseKey *)book->CreateKey();

	key->setPosition(TOP);

	cout << *key << endl;

	key->Headings(true);

	key->setPosition(TOP);

	cout << *key << endl;

	delete key;

	return 0;
}
