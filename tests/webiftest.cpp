#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif
#ifndef O_BINARY
#define O_BINARY 0
#endif
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <thmlhtmlhref.h>
#include <unicodertf.h>
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
	SWMgr mgr(0, 0, true, new MarkupFilterMgr(FMT_WEBIF, ENC_UTF8));
	mgr.setGlobalOption("Strong's Numbers", "on");
	mgr.setGlobalOption("Morphological Tags", "on");

	SWModule *module = mgr.Modules["TSK"];
	if (!module) {
		module = mgr.Modules.begin()->second;
	}
	module->setKey("John");
	std::cout << module->RenderText() << std::endl<< std::endl<< std::endl;

	//------------------------

	SWMgr mgr2(0, 0, true, new MarkupFilterMgr(FMT_HTML, ENC_UTF8));
	mgr2.setGlobalOption("Strong's Numbers", "on");
	mgr2.setGlobalOption("Morphological Tags", "on");
	module = mgr2.Modules["TSK"];
	if (!module) {
		module = mgr2.Modules.begin()->second;
	}

	module->setKey("John");
	std::cout << module->RenderText() << std::endl;

	return 0;
}

