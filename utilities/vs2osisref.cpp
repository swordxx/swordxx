// Compression on variable granularity
#include <fcntl.h>
#include <iostream>
#include <fstream>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <ztext.h>
#include <zld.h>
#include <zcom.h>
#include <swmgr.h>
#include <lzsscomprs.h>
#include <zipcomprs.h>
#include <versekey.h>
#include <thmlosis.h>
#include <stdio.h>
#include <markupfiltmgr.h>
#include <algorithm>

#ifndef NO_SWORD_NAMESPACE
using namespace sword;
#endif
using std::endl;
using std::cerr;
using std::cout;


int main(int argc, char **argv)
{
        if (argc < 2) {
        	cerr << argv[0] << " - a tool to convert verse references from English to OSIS\n";
	        cerr << "usage: "<< argv[0] << " <verse ref> [verse context]\n";
        	cerr << "\n\n";
	        exit(-1);
        }
        VerseKey verseKey;
        int i = strlen(argv[1]) + 1;
        char * verseString = new char[i];
	   strcpy (verseString, argv[1]);
        verseString[i - 1] = 0;
        
        if (argc > 2) {
                verseKey = argv[2];
        }
        else {
                verseKey = "Gen 1:1";
        }

        std::cout << VerseKey::convertToOSIS(verseString, &verseKey) << "\n";

	return 0;
}

