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
#include <iostream>
#include <string>

#ifndef O_BINARY
#define O_BINARY 0
#endif

#ifndef NO_SWORD_NAMESPACE
using sword::SWMgr;
using sword::RawText;
using sword::VerseKey;
using sword::SW_POSITION;
#endif

using std::string;

char readline(int fd, char **buf) {
	char ch;
	if (*buf)
		delete [] *buf;
	*buf = 0;
	int len;


	long index = lseek(fd, 0, SEEK_CUR);
	// clean up any preceding white space
	while ((len = read(fd, &ch, 1)) == 1) {
		if ((ch != 13) && (ch != ' ') && (ch != '\t'))
			break;
		else index++;
	}


	while (ch != 10) {
        if ((len = read(fd, &ch, 1)) != 1)
			break;
	}
	
	int size = (lseek(fd, 0, SEEK_CUR) - index) - 1;

	*buf = new char [ size + 1 ];

	if (size > 0) {
		lseek(fd, index, SEEK_SET);
		read(fd, *buf, size);
		read(fd, &ch, 1);   //pop terminating char
		(*buf)[size] = 0;

		// clean up any trailing junk on buf
		for (char *it = *buf+(strlen(*buf)-1); it > *buf; it--) {
			if ((*it != 10) && (*it != 13) && (*it != ' ') && (*it != '\t'))
				break;
			else *it = 0;
		}
	}
	else **buf = 0;
	return !len;
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
//		std::cerr << (const char*)vk << " == "  << (const char*)test << std::endl;
		return (vk == test);
	}
	else return true;	// no check if we're a heading... Probably bad.
}


int main(int argc, char **argv) {

	// Let's test our command line arguments
	if (argc < 3) {
//		fprintf(stderr, "usage: %s <vpl_file> </path/to/mod> [0|1 - file includes prepended verse references]\n", argv[0]);
		fprintf(stderr, "usage: %s </mod/files/path> <sourceFile> <verseBookPrefix_ie_Jn>\n\n", argv[0]);
		exit(-1);
	}

	// Let's see if we can open our input file
	int fd = open(argv[2], O_RDONLY|O_BINARY);
	if (fd < 0) {
		fprintf(stderr, "error: %s: couldn't open input file: %s \n", argv[0], argv[2]);
		exit(-2);
	}
/*
	// Try to initialize a default set of datafiles and indicies at our
	// datapath location passed to us from the user.
	if (RawText::createModule(argv[1])) {
		fprintf(stderr, "error: %s: couldn't create module at path: %s \n", argv[0], argv[1]);
		exit(-3);
	}

*/
	// Do some initialization stuff
	char *buffer = 0;
	int chapter  = 0;
	int verse    = 0;
	char tmpBuf[254];
	RawText mod(argv[1]);	// open our datapath with our RawText driver.
	VerseKey vk;
	vk.AutoNormalize(0);
	vk.Headings(1);	// turn on mod/testmnt/book/chap headings
	vk.Persist(1);

	mod.setKey(vk);

	// Loop through module from TOP to BOTTOM and set next line from
	// input file as text for this entry in the module
	mod = TOP;
	  
	int successive = 0;  //part of hack below
	string header = "";

	while ((!mod.Error()) && (!readline(fd, &buffer))) {
		string verseText = "";

		// chapter number
		if (!strncmp("$$$ ", buffer, 4)) {
			buffer[7] = 0;
			chapter = atoi(buffer+4);
			continue;
		}
		// header
		if (!strncmp("<TD COLSPAN=4 VALIGN=TOP><FONT SIZE=2><B>", buffer, 41)) {
			char *end = strstr(buffer+41, "</B>");
			*end = 0;
			header = buffer+41;
			continue;
		}
		// verse number
		if (!strncmp("<TD VALIGN=TOP ALIGN=RIGHT WIDTH=12><FONT SIZE=2 COLOR=RED><B><SUP>", buffer, 67)) {
			char *end = strstr(buffer+67, "</SUP>");
			*end = 0;
			verse = atoi(buffer+67);
			continue;
		}
		// Actual verse data
		if (!strncmp("<TD VALIGN=TOP><FONT SIZE=2>", buffer, 28)) {
			char *end = strstr(buffer+28, "</FONT>");
			*end = 0;
		}
		// extra
		else {
			continue;
		}

		verseText = buffer + 28;

		if (header.length()) {
			verseText = "<title type=\"section\" subtype=\"x-preverse\">" + header + "</title>" + verseText;
			header = "";
		}

		string vsbuf = argv[3];
		sprintf(tmpBuf, "%i", chapter);
		vsbuf += ((string)" ") + tmpBuf;
		sprintf(tmpBuf, "%i", verse);
		vsbuf += ((string)":") + tmpBuf;
		vk = vsbuf.c_str();
		if (vk.Error()) {
			std::cerr << "Error parsing key: " << vsbuf << "\n";
			exit(-5);
		}
		string orig = mod.getRawEntry();

		if (!isKJVRef(vsbuf.c_str())) {
			VerseKey origVK = vk;
			/* This block is functioning improperly -- problem with AutoNormalize???
			do {
				vk--;
			}
			while (!vk.Error() && !isKJVRef(vk)); */
			//hack to replace above:
			successive++;
			vk -= successive;
			orig = mod.getRawEntry();

			std::cerr << "Not a valid KJV ref: " << origVK << "\n";
			std::cerr << "appending to ref: " << vk << "\n";
			orig += " [ (";
			orig += origVK;
			orig += ") ";
			orig += verseText;
			orig += " ] ";
			verseText = orig.c_str();
		}
		else {
		  successive = 0;
		}

		if (orig.length() > 1)
			   std::cerr << "Warning, overwriting verse: " << vk << std::endl;
		  
		// ------------- End verse tests -----------------
		std::cout << "adding "<< vk << "\n";
		mod << verseText.c_str();	// save text to module at current position
	}

	// clear up our buffer that readline might have allocated
	if (buffer)
		delete [] buffer;
}
