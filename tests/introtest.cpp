#include <swmgr.h>
#include <iostream.h>
#include <versekey.h>

int main(int argc, char **argv) {
	SWMgr mymgr;

	SWModule *mhc = mymgr.Modules["MHC"];

	if (mhc) {
		VerseKey vk;
		vk.Headings(1);
		vk.AutoNormalize(0);
		vk.Persist(1);
		vk = "jas 0:0";
		cout << vk << ":\n";
		mhc->SetKey(vk);
		cout << (const char *) mhc->Key() << ":\n";
		cout << (const char *) *mhc << "\n";
	}
	return 0;
}
