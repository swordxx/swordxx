#include <iostream.h>
#include <versekey.h>
#include <treekeyidx.h>
#include <listkey.h>
int main(int argc, char **argv) {
//	VerseKey x("jas");
	TreeKeyIdx x("jas");
//	ListKey x("jas");
	SWKey *y = &x;
	TreeKeyIdx *v = &x;
//	VerseKey *v = &x;
//	ListKey *v = &x;

//	v = SWDYNAMIC_CAST(VerseKey, y);
	v = SWDYNAMIC_CAST(TreeKeyIdx, y);
//	v = SWDYNAMIC_CAST(ListKey, y);
	cout << endl;
	if (v)
		cout << (const char *)(*v);
	else
		cout << "cast failed\n";

	cout << endl;
	cout << endl;
	return 0;
}
