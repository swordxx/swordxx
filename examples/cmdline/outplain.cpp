/******************************************************************************
 * Class SWMgr manages installed modules for a frontend.
 * SWMgr reads a mods.conf file to discover its information.
 * It then instantiates the correct decendent of SWModule for each
 * module entry in mods.conf
 * The developer may use this class to query what modules are installed
 * and to retrieve an (SWModule *) for any one of these modules
 *
 * SWMgr makes its modules available as an STL Map.
 * The Map definition is typedef'ed as ModMap
 * ModMap consists of: FIRST : SWBuf moduleName
 *                     SECOND: SWModule *module
 *
 */

#include <iostream>

#include <swmgr.h>
#include <swmodule.h>
#include <versekey.h>

using namespace sword;
using namespace std;

int main(int argc, char **argv) {
	SWMgr manager;		// create a default manager that looks in the current directory for mods.conf

	const char *bookName = (argc > 1) ? argv[1] : "WLC";
	SWModule *b = manager.getModule(bookName);
	if (!b) return -1;
	SWModule &book = *b;
	book.processEntryAttributes(false);
	VerseKey *vk = SWDYNAMIC_CAST(VerseKey, book.getKey());
	for (book = TOP; !book.Error() && !book.getRawEntryBuf().size(); book++);
	if (!book.getRawEntryBuf().size()) return -2; 	// empty module
	for (;!book.Error(); book++) {
		cout << "$$$";
		if (vk) cout << vk->getOSISRef();
		else	cout << book.getKeyText();
		cout << "\n" << book.StripText() << "\n\n";
	}
}
