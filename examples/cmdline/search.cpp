#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <markupfiltmgr.h>
#include <regex.h> // GNU
#include <iostream>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

void percentUpdate(char percent, void *userData) {
	static char printed = 0;
	char maxHashes = *((char *)userData);
	
	while ((((float)percent)/100) * maxHashes > printed) {
		std::cout << "=";
		printed++;
		std::cout.flush();
	}
/*
	std::cout << (int)percent << "% ";
*/
	std::cout.flush();
}


int main(int argc, char **argv)
{
	SWMgr manager(0, 0, true, new MarkupFilterMgr(FMT_RTF, ENC_RTF));
//	SWMgr manager;
	SWModule *target;
	ListKey listkey;
	ListKey scope;
	VerseKey parser;
	ModMap::iterator it;

	if ((argc != 3) && (argc != 4)) {
		fprintf(stderr, "\nusage: %s <modname> <\"search string\"> [\"search_scope\"]\n"
							 "\tExample: search KJV \"swift hear slow speak\"\n\n", argv[0]);

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

	target = (*it).second;

	if (argc == 4) {			// if min / max specified
		scope = parser.ParseVerseList(argv[3], parser, true);
		scope.Persist(1);
		target->setKey(scope);
	}

	std::cout << "[0=================================50===============================100]\n ";
	char lineLen = 70;
	listkey = target->Search(argv[2], -2, /*SEARCHFLAG_MATCHWHOLEENTRY*/ REG_ICASE, 0, 0, &percentUpdate, &lineLen);
	std::cout << "\n";
	while (!listkey.Error()) {
		std::cout << (const char *)listkey << std::endl;
		listkey++;
	}

}
