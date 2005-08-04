#include <iostream>
#include <swbuf.h>
#include <papyriplain.h>
//#include <swmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using namespace std;


int main(int argc, char **argv) {
//	SWMgr mgr;
//	SWModule *module = mgr.getModule("KJV");
	PapyriPlain filter;
	SWBuf buf;
	buf = "This is t<e>xt which has papy-\nri markings in it.\n  L[et's be] sure it gets--\n cleaned up well for s(earching)";
	std::cout << "Original:\n\n" << buf << "\n\n-------\n\n";
	filter.processText(buf);
//	filter.processText(buf, module->getKey(), module);
	std::cout << buf << "\n\n+++++++\n";

	return 0;
}
