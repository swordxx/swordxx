#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <swbuf.h>
#ifndef NO_SWORD_NAMESPACE
using sword::SWMgr;
using sword::SWModule;
using sword::ListKey;
using sword::VerseKey;
using sword::ModMap;
using sword::SWBuf;
#endif

int main(int argc, char **argv)
{
	SWMgr manager(new sword::MarkupFilterMgr(sword::FMT_HTMLHREF, sword::ENC_UTF16));
	SWModule &kjv = *(manager.Modules["KJV"]);
	for (kjv.setKey("Gen 1:1"); ((VerseKey *)kjv.getKey())->Chapter() == 1; kjv++) {
		SWBuf x = kjv.RenderText();
	}
	
	SWModule *target;
	ListKey listkey;
	VerseKey vk;
	ModMap::iterator it;

	if (argc != 2) {
		fprintf(stderr, "usage: %s <modname>\n", argv[0]);
		exit(-1);
	}

	it = manager.Modules.find(argv[1]);
	if (it == manager.Modules.end()) {
		fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
		}
		exit(-1);
	}
	target = it->second;

	if (!target->hasSearchFramework()) {
		fprintf(stderr, "%s: error: %s does not support a search framework.\n", *argv, it->second->Name());
		exit(-2);
	}

	printf("Building framework, please wait...\n");
	char error = target->createSearchFramework();
	if (error) {
		fprintf(stderr, "%s: couldn't create search framework (permissions?)\n", *argv);
	}
}
