#include <stdio.h>
#include <iostream.h>
#include <versekey.h>
#include <rawtext.h>
#include <zcom.h>
#include <rawcom.h>
#include <rawgbf.h>
#include <rawfiles.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int loop;
	int max;
	RawFiles  personal("modules/comments/rawfiles/personal/", "MINE", "Personal Comments");
	VerseKey mykey;

	if (argc < 3) {
		fprintf(stderr, "usage: %s <\"comment\"> <\"verse\"> [count] [disable AutoNormalization]\n", argv[0]);
		exit(-1);
	}

	if (argc > 4)
		mykey.AutoNormalize(0);  // Turn off autonormalize if 3 args to allow for intros
				// This is kludgy but at lease you can try it
				// with something like: sword "Matthew 1:0" 1 1

	mykey = argv[2];
	mykey.Persist(1);
	personal.SetKey(mykey);

	max = (argc < 4) ? 1 : atoi(argv[3]);

	for (loop = 0; loop < max; loop++) {
		personal << argv[1];
		mykey++;
	}
	cout << "Added Comment\n";
	return 0;
}
