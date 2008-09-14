#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>
#include <iostream>
#include <stdio.h>

#ifndef NO_SWORD_NAMESPACE
using sword::SWMgr;
using sword::VerseKey;
using sword::ModMap;
using sword::SWKey;
using sword::SWModule;
using sword::SW_POSITION;
#endif

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "usage: %s <Mod Name>\n", argv[0]);
		exit(-1);
	}

	SWMgr mgr;

	ModMap::iterator it = mgr.Modules.find(argv[1]);
	if (it == mgr.Modules.end()) {
		fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
		exit(-2);
	}

	SWModule *mod = it->second;

	SWKey *key = (*mod);
	VerseKey *vkey = 0;
	SWTRY {
		vkey = dynamic_cast<VerseKey *>(key);
	}
	SWCATCH (...) {}

	if (!vkey) {
		fprintf(stderr, "error: %s: %s module is not keyed to verses \n", argv[0], argv[1]);
		exit(-3);
	}

	vkey->Headings(1);	// turn on mod/testmnt/book/chap headings

	(*mod) = TOP;

	while (!mod->Error()) {
	  
	  if (vkey->Verse())
	    if (!strlen ((const char *)(*mod)))
	      std::cout << *vkey << std::endl;
	  (*mod)++;
	}
}
