#include <iostream>
#include <versekey.h>
#include <rawtext.h>
//#include <zcom.h>
#include <rawcom.h>
//#include <rawgbf.h>
//#include <rawfiles.h>
#ifndef NO_SWORD_NAMESPACE
using sword::VerseKey;
using sword::RawText;
using sword::RawCom;
using sword::SWKey;
#endif
using std::cout;

int main(int argc, char **argv)
{
	int loop;
	int max;
	VerseKey mykey;
//	RawText text("src/modules/texts/rawtext/sve/", "Sven Text", "Sven Text");
	RawText text("src/modules/texts/rawtext/webster/", "Webster", "Webster Text");
//	RawText text("src/modules/texts/rawtext/orthjbc/", "Webster", "Webster Text");
//	RawGBF text("src/modules/texts/rawgbf/newtext/", "WEB", "World English Bible");
//	RawText text("src/modules/texts/rawtext/kjv/", "KJV", "KJV Text");
//	RawText text("src/modules/texts/rawtext/vnt/", "VNT", "Valera Spanish NT");
//	RawCom  commentary("src/modules/comments/rawcom/rwp/", "RWP", "Robertson's Word Pictures");
	RawCom  commentary("src/modules/comments/rawcom/mhc/", "MHC", "Matthew Henry's Commentary on the Whole Bible");
//	RawFiles  commentary("src/modules/comments/rawfiles/personal/", "MHC", "Matthew Henry's Commentary on the Whole Bible");


	if (argc > 3)
		mykey.AutoNormalize(0);  // Turn off autonormalize if 3 args to allow for intros
				// This is kludgy but at lease you can try it
				// with something like: sword "Matthew 1:0" 1 1


	mykey = (argc < 2) ? "James    1:19" : argv[1];
	mykey.Persist(1);
	text.setKey(mykey);
	commentary.setKey(mykey);


	max = (argc < 3) ? 1 : atoi(argv[2]);


	cout << "\n";


	for (loop = 0; loop < max; loop++) {
		cout << (SWKey &)text << ":\n";
		text.Display();
		cout << "\n";
		cout << "-------------\n";
		commentary.Display();
		cout << "\n";
		cout << "==========================\n";
		mykey++;
	}
	cout << "\n\n";
}
