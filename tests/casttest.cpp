#include <iostream.h>
#include <versekey.h>
#include <swmodule.h>
int main(int argc, char **argv) {
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
	return 0;
}
