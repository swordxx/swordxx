#include <iostream>
#include <thmlhtmlhref.h>
#include <thmlosis.h>
#include <gbfosis.h>
#include <versekey.h>
#include <swmgr.h>

#define MAXBUF 30000
int main(int argc, char **argv) {
	SWMgr mgr;
	SWModule *module = mgr.Modules["KJV"];
	if (!module)
		module = mgr.Modules.begin()->second;

	((VerseKey *)(SWKey *)*module)->AutoNormalize(0);
	((VerseKey *)(SWKey *)*module)->Headings(1);
//    ThMLHTMLHREF filter;
    GBFOSIS filter;
    module->Key() = ((argc > 1) ? argv[1] : "jas1:1");
    char *buf = new char [ MAXBUF ];
    memset(buf, 0, MAXBUF);
//    strcpy(buf, "This is a verse reference: <scripRef>jas1:22,23-25;3;5:1;rom1-9</scripRef> with an <img src=\"/images/yoyo.jpg\">");
    module->getRawEntry();
    memcpy(buf, module->getRawEntry(), module->getEntrySize());
    std::cout << "Original:\n\n" << buf << "\n\n-------\n\n";
    filter.ProcessText(buf, MAXBUF - 3, *module, module);

    std::cout << buf << "\n\n+++++++\n";
    delete [] buf;
    return 0;
}
