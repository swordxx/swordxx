#include <iostream>
#include <swbuf.h>
#include <osishtmlhref.h>
//#include <swmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using namespace std;


int main(int argc, char **argv) {
//	SWMgr mgr;
//	SWModule *module = mgr.getModule("KJV");
	OSISHTMLHREF filter;
	SWBuf buf;
	buf = "This is OSIS text with a <figure src=\"images/scroll.png\"/>";
	std::cout << "Original:\n\n" << buf << "\n\n-------\n\n";
	filter.processText(buf);
//	filter.processText(buf, module->getKey(), module);
	std::cout << buf << "\n\n+++++++\n";

	return 0;
}
