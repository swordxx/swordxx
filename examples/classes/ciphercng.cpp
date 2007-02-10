/******************************************************************************
 *
 * This example demonstrates how to change the cipher key of a module
 *	The change is only in effect for this run.  This DOES NOT change the
 *	cipherkey in the module's .conf file.
 *
 */

#include <iostream>

#include <swmgr.h>
#include <swmodule.h>


using namespace sword;
using namespace std;

int main(int argc, char **argv) {

	if (argc != 2) {
		fprintf(stderr, "usage: %s <modName>\n", *argv);
		exit(-1);
	}

	SWMgr manager;		// create a default manager that looks in the current directory for mods.conf
	ModMap::iterator it;
	it = manager.Modules.find(argv[1]);

	if (it == manager.Modules.end()) {
		fprintf(stderr, "%s: couldn't find module: %s\n", *argv, argv[1]);
		exit(-1);
	}

	SWModule *module = (*it).second;
	string key;

	cout << "\nPress [CTRL-C] to end\n\n";
	while (true) {
		cout << "\nModule text:\n";
		module->setKey("1jn 1:9");
		cout << "[ " << module->KeyText() << " ]\n";
		cout << (const char *)*module;
		cout << "\n\nEnter new cipher key: ";
		cin >> key;
		cout << "\nSetting key to: " << key;
		manager.setCipherKey(argv[1], key.c_str());
	}


}
