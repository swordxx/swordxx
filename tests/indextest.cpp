#include <swmgr.h>
#include <iostream.h>
#include <versekey.h>

int main(int argc, char **argv) {
	SWMgr mymgr;

	SWModule *bbe = mymgr.Modules["BBE"];

	if (bbe) {
		VerseKey vk;
		vk.Persist(1);
		bbe->SetKey(vk);
		for (; !bbe->Error(); (*bbe)++ ) {
			cout << vk.NewIndex() << endl; 
		}
	}
	return 0;
}
