#include <iostream>
#include <thmlhtmlhref.h>
#include <thmlosis.h>
#include <gbfosis.h>
#include <thmlosis.h>
#include <versekey.h>
#include <swmgr.h>
#include <markupfiltmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using namespace std;

#define MAXBUF 30000
int main(int argc, char **argv) {
	SWMgr mgr(0, 0, true, new MarkupFilterMgr(FMT_RTF, ENC_RTF));
	mgr.setGlobalOption("Strong's Numbers", "on");
	mgr.setGlobalOption("Morphological Tags", "on");
	SWModule *module = mgr.Modules["KJV2003"];
	if (!module)
		module = mgr.Modules.begin()->second;

    //ThMLOSIS filter;
    module->Key() = ((argc > 1) ? argv[1] : "acts 2:1");
    /*
    char *buf = new char [ MAXBUF ];
    memset(buf, 0, MAXBUF);
//    strcpy(buf, "This is a verse reference: <scripRef>jas1:22,23-25;3;5:1;rom1-9</scripRef> with an <img src=\"/images/yoyo.jpg\">");
    module->getRawEntry();
    memcpy(buf, module->getRawEntry(), module->getEntrySize());
    std::cout << "Original:\n\n" << buf << "\n\n-------\n\n";
    filter.ProcessText(buf, MAXBUF - 3, *module, module);

    std::cout << buf << "\n\n+++++++\n";
    delete [] buf;
    */
    cout << *module << "\n";
    return 0;
}
