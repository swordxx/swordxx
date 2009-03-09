#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <swmgr.h>
#include <swmodule.h>
#include <markupfiltmgr.h>

using sword::SWMgr;
using sword::MarkupFilterMgr;
using sword::SWModule;
using sword::FMT_WEBIF;
using sword::FMT_HTMLHREF;
using sword::FMT_RTF;
using sword::ModMap;
using sword::AttributeTypeList;
using sword::AttributeList;
using sword::AttributeValue;

int main(int argc, char **argv)
{
	
	SWMgr manager(new MarkupFilterMgr(FMT_WEBIF));
//	SWMgr manager(new MarkupFilterMgr(FMT_HTMLHREF));
	SWModule *target;

	if (argc != 3) {
		fprintf(stderr, "\nusage: %s <modname> <\"lookup key\">\n"
							 "\tExample: lookup KJV \"James 1:19\"\n\n", argv[0]);
 
		exit(-1);
	}

	target = manager.getModule(argv[1]);
	if (!target) {
		fprintf(stderr, "Could not find module [%s].  Available modules:\n", argv[1]);
		ModMap::iterator it;
		for (it = manager.Modules.begin(); it != manager.Modules.end(); it++) {
			fprintf(stderr, "[%s]\t - %s\n", (*it).second->Name(), (*it).second->Description());
		}
		exit(-1);
	}

	// turn all filters to default values
	manager.setGlobalOption("Headings", "On");
	manager.setGlobalOption("Strong's Numbers", "On");
	manager.setGlobalOption("Lemmas", "Off");
//	manager.setGlobalOption("Greek Accents", "Off");

	target->setKey(argv[2]);

	target->RenderText();		// force an entry lookup to resolve key to something in the index

	std::cout << "==Raw=Entry===============\n";
	std::cout << target->getKeyText() << ":\n";
	std::cout << target->getRawEntry();
	std::cout << "\n";
	std::cout << "==Render=Entry============\n";
	std::cout << target->RenderText();
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
