#include <iostream.h>
#include <versekey.h>
int main(int argc, char **argv) {
	VerseKey x("jas");
	SWKey *y = &x;
	VerseKey *v = &x;

	v = SWDYNAMIC_CAST(VerseKey, y);
	if (v)
		cout << (const char *)(*v);
	else
		cout << "cast failed\n";

	return 0;
}
