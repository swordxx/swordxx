#include <iostream.h>
#include <thmlhtmlhref.h>
#include <thmlosis.h>
#include <versekey.h>
#include <swmgr.h>

#define MAXBUF 30000
int main(int argc, char **argv) {
	SWMgr mgr;
	SWModule *module = mgr.Modules["MHC"];
	if (!module)
		module = mgr.Modules.begin()->second;

	((VerseKey &)module->Key()).Normalize(0);
	((VerseKey &)module->Key()).Headings(1);
//    ThMLHTMLHREF filter;
    ThMLOSIS filter;
    module->SetKey((argc > 1) ? argv[1] : "jas1:1");
    char *buf = new char [ MAXBUF ];
    memset(buf, 0, MAXBUF);
//    strcpy(buf, "This is a verse reference: <scripRef>James 1:19</scripRef> with an <img src=\"/images/yoyo.jpg\">");
    strcpy(buf, module->getRawEntry());
    filter.ProcessText(buf, MAXBUF - 3, *module, module);

    cout << buf << "\n";
    delete [] buf;
    return 0;
}
