#include <swmgr.h>
#include <installmgr.h>
#include <iostream>

using namespace sword;
using std::cout;

void usage(const char *progName) {
	fprintf(stderr, "usage: %s <option>\nOptions:\n\t-l\t\tlist installed modules\n\t-u <modName>\tuninstall module\n", progName);
	exit(-1);
}

int main(int argc, char **argv) {
	if (argc < 2)
		usage(*argv);

	SWMgr mgr;
	SWModule *module = 0;
	if (argv[1][1] == 'l') {
		for (ModMap::iterator it = mgr.Modules.begin(); it != mgr.Modules.end(); it++) {
			module = it->second;
			cout << "[" << module->Name() << "]  \t- " << module->Description() << "\n";
		}
	}
	else if (argv[1][1] == 'u') {
		if (argc < 3)
			usage(*argv);

		ModMap::iterator it = mgr.Modules.find(argv[2]);
		if (it == mgr.Modules.end()) {
			fprintf(stderr, "Couldn't find module [%s] to remove\n", argv[2]);
			exit(-2);
		}
		module = it->second;
		removeModule(&mgr, module->Name());
		cout << "Removed module: [" << module->Name() << "]\n";
	}
	else usage(*argv);

	return 0;
}
