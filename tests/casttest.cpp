#include <iostream>
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
	std::cout << std::endl;
	if (v)
		std::cout << (const char *)(*v);
	else
		std::cout << "cast failed\n";

	std::cout << std::endl;
	std::cout << std::endl;
	return 0;
}
