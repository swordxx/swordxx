#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <versekey.h>
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

void percentUpdate(char percent, void *userData) {
	static char printed = 0;
	char maxHashes = *((char *)userData);
	
	while ((((float)percent)/100) * maxHashes > printed) {
		printf("=");
		printed++;
		fflush(stdout);
	}
/*
	std::cout << (int)percent << "% ";
*/
	fflush(stdout);
}


int main(int argc, char **argv)
{

	if (argc != 2) {
		fprintf(stderr, "usage: %s <modname>\n", argv[0]);
		exit(-1);
	}
	
	SWModule *target;
	ListKey listkey;
	VerseKey vk;
	ModMap::iterator it;

	SWMgr manager;
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

	printf("Deleting any existing framework...\n");
	target->deleteSearchFramework();
	printf("Building framework, please wait...\n");
	char lineLen = 70;
	char error = target->createSearchFramework(&percentUpdate, &lineLen);
	if (error) {
		fprintf(stderr, "%s: couldn't create search framework (permissions?)\n", *argv);
	}
}
