#include <swmgr.h>
#include <versekey.h>
#include <iostream.h>


int main(int argc, char **argv) {

	if (argc < 2) {
		fprintf(stderr, "usage: %s <Mod Name> [0|1 - prepend verse reference]\n", argv[0]);
		exit(-1);
	}

	SWMgr mgr;

	ModMap::iterator it = mgr.Modules.find(argv[1]);
	if (it == mgr.Modules.end()) {
		fprintf(stderr, "error: %s: couldn't find module: %s \n", argv[0], argv[1]);
		exit(-2);
	}

	bool vref = false;
	if (argc > 2)
		vref = (argv[2][0] == '0') ? false : true;


	SWModule *mod = it->second;

	SWKey *key = (*mod);
	VerseKey *vkey = 0;
	try {
		vkey = dynamic_cast<VerseKey *>(key);
	}
	catch (...) {}

	if (!vkey) {
		fprintf(stderr, "error: %s: %s module is not keyed to verses \n", argv[0], argv[1]);
		exit(-3);
	}

	vkey->Headings(1);	// turn on mod/testmnt/book/chap headings

	(*mod) = TOP;

	while (!mod->Error()) {
		if (vref)
			cout << (const char *)(*vkey) << " ";
		cout << (const char *) (*mod) << "\n";
		(*mod)++;
	}
}
