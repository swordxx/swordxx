#include <swmgr.h>
#include <swtext.h>
#include <iostream>
#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif

int main(int argc, char **argv) {
	SWMgr mymgr;
	ModMap::iterator it;
	SWModule *module;

	for (it = mymgr.Modules.begin(); it != mymgr.Modules.end(); it++) {
		module = it->second;
		if (!strcmp(module->Type(), "Biblical Texts")) {
			std::cout << module->Name() << "\n";
			for (*module = TOP; (!module->Key().Error()); ((VerseKey *)&module->Key())->Book(((VerseKey *)&module->Key())->Book()+1)) {
				std::cout << module->KeyText() << "\n";
			}
		}
			
		if (!strcmp(module->Type(), "Commentaries")) {
			std::cout << module->Name() << "\n";
			for (*module = TOP; (!module->Error()); (*module)--) {
				std::cout << module->KeyText() << "\n";
			}
				
		}
	}
	return 0;
}
