#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <swmgr.h>
#include <swmodule.h>


int main(int argc, char **argv)
{
	sword::SWMgr manager(0, 0, true, 0, true);
	sword::SWModule *target;

	if (argc != 3) {
		fprintf(stderr, "\nusage: %s <modname> <\"lookup key\">\n"
							 "\tExample: lookup KJV \"James 1:19\"\n\n", argv[0]);
 
		exit(-1);
	}

	target = manager.getModule(argv[1]);
	if (!target) {
		fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
		sword::ModMap::iterator it;
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
		}
		exit(-1);
	}

	manager.setGlobalOption("Headings", "On");
	manager.setGlobalOption("Greek Accents", "Off");

	target->setKey(argv[2]);

	target->RenderText();		// force an entry lookup to resolve key to something in the index

	std::cout << target->getKeyText() << ":\n";
	std::cout << target->getRawEntry();
	std::cout << "\n";
	std::cout << "==========================\n";
	std::cout << "Entry Attributes:\n\n";
	sword::AttributeTypeList::iterator i1;
	sword::AttributeList::iterator i2;
	sword::AttributeValue::iterator i3;
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
