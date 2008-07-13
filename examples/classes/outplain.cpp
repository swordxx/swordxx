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

using namespace sword;
using namespace std;

int main(int argc, char **argv) {
	SWMgr manager;		// create a default manager that looks in the current directory for mods.conf

	const char *bookName = (argc > 1) ? argv[1] : "WLC";
	SWModule &book = *manager.getModule(bookName);
	for (book = TOP; !book.Error(); book++) {
		cout << book.StripText() << "\n";
	}
}
