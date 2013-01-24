#include <swmgr.h>
#include <swmodule.h>
#include <iostream>

using namespace sword;

using std::cout;
using std::endl;

int main(int argc, char **argv) {

	SWBuf modName = (argc > 1) ? argv[2] : "WLC";
	SWBuf keyText = (argc > 2) ? argv[3] : "Gen.1.9";
	SWBuf searchText = (argc > 3) ? argv[4] : "מתחת";
	SWMgr library;
	SWModule *book = library.getModule(modName);
	StringList filters = library.getGlobalOptions();
	for (StringList::iterator it = filters.begin(); it != filters.end(); ++it) {
	    // blindly turn off all filters.  Some filters don't support "Off", but that's ok, we should just silently fail on those.
	    library.setGlobalOption(*it, "Off");
	}
	book->setKey(keyText.c_str());
	SWBuf entryStripped = book->stripText();
	cout << "Module: " << book->getDescription() << "\t Key: " << book->getKeyText() << "\n";
	cout << "RawEntry:\n" << book->getRawEntry() << "\n";
	cout << "StripText:\n" << entryStripped << "\n";
	cout << "Search Target: " << searchText << "\n";
	cout << "Search Target StripText: " << book->stripText(searchText) << "\n";
	cout << "Found: " << ((strstr(entryStripped.c_str(), book->stripText(searchText))) ? "true":"false") << endl; 

	return 0;
}
