#include <iostream.h>
#include <thmlhtmlhref.h>
#include <thmlosis.h>
#include <swmgr.h>

#define MAXBUF 30000
int main(int argc, char **argv) {
	SWMgr mgr;
	SWModule *module = mgr.Modules.begin()->second;
//    ThMLHTMLHREF filter;
    ThMLOSIS filter;
    char *buf = new char [ MAXBUF ];
    memset(buf, 0, MAXBUF);
    strcpy(buf, "This is a verse reference: <scripRef>James 1:19</scripRef> with an <img src=\"/images/yoyo.jpg\">");
    filter.ProcessText(buf, MAXBUF - 3, *module, module);

    cout << buf << "\n";

    return 0;
}
