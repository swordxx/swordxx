#include <swmgr.h>
#include <swtext.h>
#include <iostream.h>

main() {
	SWMgr mymgr;
	ModMap::iterator it;
	SWModule *module;

	for (it = mymgr.Modules.begin(); it != mymgr.Modules.end(); it++) {
		module = it->second;
		if (!strcmp(module->Type(), "Biblical Texts")) {
			cout << module->Name() << "\n";
			for (*module = TOP; (!module->Key().Error()); ((VerseKey *)&module->Key())->Book(((VerseKey *)&module->Key())->Book()+1)) {
				cout << module->KeyText() << "\n";
			}
		}
			
		if (!strcmp(module->Type(), "Commentaries")) {
			cout << module->Name() << "\n";
			for (*module = TOP; (!module->Error()); (*module)--) {
				cout << module->KeyText() << "\n";
			}
				
		}
	}
}
