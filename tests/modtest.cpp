#include <swmgr.h>
#include <swtext.h>
#include <iostream>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	SWMgr mymgr;
	ModMap::iterator it;
	SWModule *module = mymgr.Modules["KJV"];
	VerseKey parser;
	ListKey lk = parser.ParseVerseList("mal4:6-rev", parser, true);
	lk.Persist(1);
	module->SetKey(lk);

	(*module) = TOP;
	std::cout << module->KeyText() << "\n";
	return 0;
}
