#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <swmgr.h>

int main(int argc, char **argv)
{
	SWMgr manager;
	SWModule *target;
	ModMap::iterator it;

	if (argc != 3) {
		fprintf(stderr, "usage: %s <modname> <\"lookup key\">\n", argv[0]);
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

	target->setKey(argv[2]);

	(const char *)*target;		// force an entry lookup to resolve key so we
						// get the idxbuf entry for the key
	std::cout << (const char *)(SWKey &)*target << ":\n";
	std::cout << target->StripText();
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
