#include <ctype.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

#ifndef __GNUC__
#include <io.h>
#else
#include <unistd.h>
#endif

#include <swmgr.h>
#include <rawtext.h>
#include <iostream.h>
#include <string>

#ifndef O_BINARY
#define O_BINARY 0
#endif

char readline(int fd, char **buf) {
	char ch;
	if (*buf)
		delete [] *buf;
	*buf = 0;
	int len;


	long index = lseek(fd, 0, SEEK_CUR);
	// clean up any preceding white space
	while ((len = read(fd, &ch, 1)) == 1) {
		if ((ch != 10) && (ch != 13) && (ch != ' ') && (ch != '\t'))
			break;
		else index++;
	}


	while ((len = read(fd, &ch, 1)) == 1) {
		if (ch == 10)
			break;
	}
	
	int size = (lseek(fd, 0, SEEK_CUR) - index) - 1;

	if (size >= 0) {
		*buf = new char [ size + 1 ];

		lseek(fd, index, SEEK_SET);
		read(fd, *buf, size);
		(*buf)[size] = 0;

		// clean up any trailing junk on buf
		for (char *it = *buf+(strlen(*buf)-1); it > *buf; it--) {
			if ((*it != 10) && (*it != 13) && (*it != ' ') && (*it != '\t'))
				break;
			else *it = 0;
		}
		return 0;
	}
	return -1;
}


char *parseVReg(char *buf) {
	char stage = 0;

	while (*buf) {
		switch (stage) {
		case 0:
			if (isalpha(*buf))
				stage++;
			break;
		case 1:
			if (isdigit(*buf))
				stage++;
			break;
		case 2:
			if (*buf == ':')
				stage++;
			break;
		case 3:
			if (isdigit(*buf))
				stage++;
			break;
		case 4:
			if (*buf == ' ') {
				*buf = 0;
				return ++buf;
			}
			break;
		}
		buf++;
	}
	return 0;
}


bool isKJVRef(const char *buf) {
	VerseKey vk, test;
	vk.AutoNormalize(0);
	vk.Headings(1);	// turn on mod/testmnt/book/chap headings
	vk.Persist(1);
	// lets do some tests on the verse --------------
	vk = buf;
	test = buf;

	if (vk.Testament() && vk.Book() && vk.Chapter() && vk.Verse()) { // if we're not a heading
//		cout << (const char*)vk << " == "  << (const char*)test << endl;
		return (vk == test);
	}
	else return true;	// no check if we're a heading... Probably bad.
}



int main(int argc, char **argv) {

	// Let's test our command line arguments
	if (argc < 2) {
//		fprintf(stderr, "usage: %s <vpl_file> </path/to/mod> [0|1 - file includes prepended verse references]\n", argv[0]);
		fprintf(stderr, "usage: %s <vpl_file> </path/to/mod/> [0|1 - verse ref prepended to lines] [0|1 - NT only]\n", argv[0]);
		exit(-1);
	}

	// Let's see if we can open our input file
	int fd = open(argv[1], O_RDONLY|O_BINARY);
	if (fd < 0) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], argv[1]);
		exit(-2);
	}

	// Try to initialize a default set of datafiles and indicies at our
	// datapath location passed to us from the user.
	if (RawText::createModule(argv[2])) {
		fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[2]);
		exit(-3);
	}

	// not used yet, but for future support of a vpl file with each line
	// prepended with verse reference, eg. "Gen 1:1 In the beginning..."
	bool vref = false;
	if (argc > 2)
		vref = (argv[3][0] == '0') ? false : true;

	// if 'nt' is the 4th arg, our vpl file only has the NT
	bool ntonly = false;
	if (argc > 3)
                ntonly = (argv[4][0] == '0') ? false : true;
	
	// Do some initialization stuff
	char *buffer = 0;
	RawText mod(argv[2]);	// open our datapath with our RawText driver.
	VerseKey vk;
	vk.AutoNormalize(0);
	vk.Headings(1);	// turn on mod/testmnt/book/chap headings
	vk.Persist(1);

	mod.SetKey(vk);

	// Loop through module from TOP to BOTTOM and set next line from
	// input file as text for this entry in the module
	mod = TOP;
	if (ntonly) vk = "Matthew 1:1";
	  

	while ((!mod.Error()) && (!readline(fd, &buffer))) {
		if (*buffer == '|')	// comments, ignore line
			continue;
		if (vref) {
			const char *verseText = parseVReg(buffer);
			if (!verseText) {	// if we didn't find a valid verse ref
				cerr << "No valid verse ref found on line: " << buffer << "\n";
				exit(-4);
			}

			vk = buffer;
			if (!isKJVRef(buffer)) {
				VerseKey origVK = vk;
				do {
					vk--;
				}
				while (!vk.Error() && !isKJVRef(vk));
				cout << "Not a valid KJV ref: " << origVK << "\n";
				cout << "appending to ref: " << vk << "\n";
				string orig = mod.getRawEntry();
				orig += " [ (";
				orig += origVK;
				orig += ") ";
				orig += verseText;
				orig += " ] ";
				verseText = orig.c_str();
			}
			// ------------- End verse tests -----------------
			mod << verseText;	// save text to module at current position
		}
		else {
			mod << buffer;	// save text to module at current position
			mod++;	// increment module position
		}
	}

	// clear up our buffer that readline might have allocated
	if (buffer)
		delete [] buffer;
}
