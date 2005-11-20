#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
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
#include <swmodule.h>
#include <markupfiltmgr.h>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using namespace std;

#define MAXBUF 30000

int main(int argc, char **argv) {

	const char* modName = (argc >= 2) ? argv[1] : "KJV";
	const char* keyName = (argc == 3) ? argv[2] : "John 1:1";

	SWMgr mgr(0, 0, true, new MarkupFilterMgr(FMT_WEBIF, ENC_UTF8));
	mgr.setGlobalOption("Strong's Numbers", "on");
	mgr.setGlobalOption("Morphological Tags", "on");

	SWModule *module = mgr.Modules[modName];
	if (!module) {
		module = mgr.Modules.begin()->second;
	}
	module->setKey(keyName);
	std::cout << module->RenderText() << std::endl<< std::endl<< std::endl;

	//------------------------

	SWMgr mgr2(0, 0, true, new MarkupFilterMgr(FMT_HTMLHREF, ENC_UTF8));
	mgr2.setGlobalOption("Strong's Numbers", "on");
	mgr2.setGlobalOption("Morphological Tags", "on");
	module = mgr2.Modules[modName];
	if (!module) {
		module = mgr2.Modules.begin()->second;
	}

	module->setKey(keyName);
	std::cout << module->RenderText() << std::endl;

	return 0;
}

