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


const char *convertToOSIS(const char *inRef, const SWKey *key) {
	static SWBuf outRef;

	outRef = "";

	VerseKey defLanguage;
	ListKey verses = defLanguage.ParseVerseList(inRef, (*key), true);
	const char *startFrag = inRef;
	for (int i = 0; i < verses.Count(); i++) {
		VerseKey *element = SWDYNAMIC_CAST(VerseKey, verses.GetElement(i));
		char buf[5120];
		char frag[800];
		char preJunk[800];
		char postJunk[800];
		memset(buf, 0, 5120);
		memset(frag, 0, 800);
		memset(preJunk, 0, 800);
		memset(postJunk, 0, 800);
		while ((*startFrag) && (strchr(" {};,()[].", *startFrag))) {
			outRef += *startFrag;
			startFrag++;
		}
		if (element) {
			memmove(frag, startFrag, ((const char *)element->userData - startFrag) + 1);
			frag[((const char *)element->userData - startFrag) + 1] = 0;
			int j;
			for (j = strlen(frag)-1; j && (strchr(" {};,()[].", frag[j])); j--);
			if (frag[j+1])
				strcpy(postJunk, frag+j+1);
			frag[j+1]=0;
			startFrag += ((const char *)element->userData - startFrag) + 1;
			sprintf(buf, "<reference osisRef=\"%s-%s\">%s</reference>%s", element->LowerBound().getOSISRef(), element->UpperBound().getOSISRef(), frag, postJunk);
		}
		else {
			memmove(frag, startFrag, ((const char *)verses.GetElement(i)->userData - startFrag) + 1);
			frag[((const char *)verses.GetElement(i)->userData - startFrag) + 1] = 0;
			int j;
			for (j = strlen(frag)-1; j && (strchr(" {};,()[].", frag[j])); j--);
			if (frag[j+1])
				strcpy(postJunk, frag+j+1);
			frag[j+1]=0;
			startFrag += ((const char *)verses.GetElement(i)->userData - startFrag) + 1;
			sprintf(buf, "<reference osisRef=\"%s\">%s</reference>%s", VerseKey(*verses.GetElement(i)).getOSISRef(), frag, postJunk);
		}
		outRef+=buf;
	}
	if (startFrag < (inRef + strlen(inRef)))
		outRef+=startFrag;
	return outRef.c_str();
}

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

        std::cout << convertToOSIS(verseString, &verseKey) << "\n";

	return 0;
}

