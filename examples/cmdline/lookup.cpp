#include <stdio.h>
#include <iostream.h>
#include <stdlib.h>
#include <rawld.h>

int main(int argc, char **argv)
{
	RawLD eastons("../../modules/lexdict/rawld/eastons/eastons", "Eastons BD", "Easton's Bible Dictionary");
	RawLD vines("../../modules/lexdict/rawld/vines/vines", "Vines BD", "Vine's Bible Dictionary");
	SWModule *lexdict;

	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <\"lookup string\"> [1 - Use Vine's instead of Easton's ]\n";
		exit(1);
	}

	lexdict = (argc > 2) ? &vines : &eastons;

	lexdict->SetKey(argv[1]);

	(const char *)*lexdict;		// force an entry lookup to resolve key so we
						// get the idxbuf entry for the key
	cout << (const char *)(SWKey &)*lexdict << ":\n";
	lexdict->Display();
	cout << "\n";
	cout << "==========================\n";
	return 0;
}
