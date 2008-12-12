#include <stdio.h>
#include <rawtext.h>
#include <swmgr.h>
#include <versekey.h>
#include <markupfiltmgr.h>
#include <regex.h> // GNU
#include <iostream>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

  /*
	 *			>=0 - regex
	 *			-1  - phrase
	 *			-2  - multiword
	 *			-3  - entryAttrib (eg. Word//Lemma/G1234/)
	 *			-4  - Lucene
   */
char SEARCH_TYPE=-2;

char printed = 0;
void percentUpdate(char percent, void *userData) {
	char maxHashes = *((char *)userData);
	
	while ((((float)percent)/100) * maxHashes > printed) {
		std::cerr << "=";
		printed++;
		std::cerr.flush();
	}
/*
	std::cout << (int)percent << "% ";
*/
	std::cout.flush();
}


int main(int argc, char **argv)
{
//	SWMgr manager(0, 0, true, new MarkupFilterMgr(FMT_RTF, ENC_RTF));
	SWMgr manager;
	SWModule *target;
	ListKey listkey;
	ListKey scope;
	VerseKey parser;
	ModMap::iterator it;

	if ((argc < 3) || (argc > 5)) {
		fprintf(stderr, "\nusage: %s <modname> <\"search string\"> [\"search_scope\"] [\"search again for string in previous result set\"]\n"
							 "\tExample: search KJV \"swift hear slow speak\"\n\n", argv[0]);

		exit(-1);
	}


	SWBuf searchTerm = argv[2];
	manager.setGlobalOption("Greek Accents", "Off");
	manager.setGlobalOption("Strong's Numbers", "On");
	manager.filterText("Greek Accents", searchTerm);

	it = manager.Modules.find(argv[1]);
	if (it == manager.Modules.end()) {
		fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
		}
		exit(-1);
	}

	target = (*it).second;

	if (argc > 3) {			// if min / max specified
		scope = parser.ParseVerseList(argv[3], parser, true);
		scope.Persist(1);
		target->setKey(scope);
	}

	std::cerr << "[0=================================50===============================100]\n ";
	char lineLen = 70;
	listkey = target->Search(searchTerm.c_str(), SEARCH_TYPE, /*SEARCHFLAG_MATCHWHOLEENTRY*/ REG_ICASE, 0, 0, &percentUpdate, &lineLen);
	std::cerr << std::endl;
	if (argc > 4) {			// if min / max specified
		scope = listkey;
		scope.Persist(1);
		target->setKey(scope);
		printed = 0;
		std::cerr << " ";
		listkey = target->Search(argv[4], SEARCH_TYPE, /*SEARCHFLAG_MATCHWHOLEENTRY*/ REG_ICASE, 0, 0, &percentUpdate, &lineLen);
		std::cerr << std::endl;
	}
	listkey.sort();
	while (!listkey.Error()) {
		std::cout << (const char *)listkey << std::endl;
		listkey++;
	}

}
