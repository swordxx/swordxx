#include <iostream>
#include <versekey.h>
#include <localemgr.h>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using std::endl;
using std::cerr;
using std::cout;


int main(int argc, char **argv)
{
        if (argc < 2) {
        	cerr << argv[0] << " - parse verse reference to OSISRef markup\n";
	        cerr << "usage: "<< argv[0] << " <verse ref> [verse context] [locale]\n";
        	cerr << "\n\n";
	        exit(-1);
        }

        char * locale;

	if (argc > 3) {
		LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName(argv[3]);
	}

        VerseKey verseKey = (argc > 2) ? argv[2] : "Gen 1:1";

        std::cout << VerseKey::convertToOSIS(argv[1], &verseKey) << "\n";

	return 0;
}

