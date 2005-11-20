#include <swmgr.h>
#include <iostream>
#include <versekey.h>
#include <swmodule.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	SWMgr mymgr;

	SWModule *bbe = mymgr.Modules["BBE"];

	if (bbe) {
		VerseKey vk;
		vk.Persist(1);
		bbe->setKey(vk);
		for (; !bbe->Error(); (*bbe)++ ) {
			std::cout << vk.NewIndex() << std::endl; 
		}
	}
	return 0;
}
