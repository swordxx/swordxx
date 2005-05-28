#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <swmgr.h>

#ifndef NO_SWORD_NAMESPACE
using sword::SWMgr;
using sword::SWModule;
using sword::ModMap;
using sword::SWKey;
using sword::AttributeTypeList;
using sword::AttributeValue;
using sword::AttributeList;
#endif

int main(int argc, char **argv)
{
	SWMgr manager(0, 0, true, 0, true);
	SWModule *target;
	ModMap::iterator it;

	if (argc != 3) {
		fprintf(stderr, "\nusage: %s <modname> <\"lookup key\">\n"
							 "\tExample: lookup KJV \"James 1:19\"\n\n", argv[0]);
 
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

	manager.setGlobalOption("Headings", "On");

	target = (*it).second;

	target->setKey(argv[2]);

	(const char *)*target;		// force an entry lookup to resolve key so we
						// get the idxbuf entry for the key
	std::cout << (const char *)(SWKey &)*target << ":\n";
//	std::cout << target->StripText();
	std::cout << target->getRawEntry();
	std::cout << "\n";
	std::cout << "==========================\n";
	std::cout << "Entry Attributes:\n\n";
	AttributeTypeList::iterator i1;
	AttributeList::iterator i2;
	AttributeValue::iterator i3;
	for (i1 = target->getEntryAttributes().begin(); i1 != target->getEntryAttributes().end(); i1++) {
		std::cout << "[ " << i1->first << " ]\n";
		for (i2 = i1->second.begin(); i2 != i1->second.end(); i2++) {
			std::cout << "\t[ " << i2->first << " ]\n";
			for (i3 = i2->second.begin(); i3 != i2->second.end(); i3++) {
				std::cout << "\t\t" << i3->first << " = " << i3->second << "\n";
			}
		}
	}
	std::cout << std::endl;
	return 0;
}
