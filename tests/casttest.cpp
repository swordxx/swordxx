#include <iostream.h>
#include <versekey.h>
#include <swmodule.h>
main() {
	VerseKey x("jas");
	SWKey *y = &x;
	SWModule *yoyo;

	try {
		yoyo = dynamic_cast<SWModule *>(y);
		cout << "cast succeeded\n";
	}
	catch(...) {
		cout << "cast failed\n";
	}
}
