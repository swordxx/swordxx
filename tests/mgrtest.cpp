#include <swmgr.h>
#include <iostream.h>

main() {
	SWMgr mymgr;
	ModMap::iterator it;

	for (it = mymgr.Modules.begin(); it != mymgr.Modules.end(); it++) {
		if ((!strcmp((*it).second->Type(), "Biblical Texts")) || (!strcmp((*it).second->Type(), "Commentaries"))) {
			(*it).second->SetKey("James 1:19");
			cout << "[" << (*it).second->Name() << "] [" << (*it).second->Description() << "]\n";
			cout << (char *) *(*it).second << "\n\n";
		}
	}
}
